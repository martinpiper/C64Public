// Harmless warning about browse information
#pragma warning (disable : 4786)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#include "CompressU.h"
#include "CompressUInternals.h"

typedef struct _CompressionU_match
{
	unsigned short mLength;
	unsigned short mOffset;
} CompressionU_match;

typedef struct _CompressionU_arrival
{
	int mCost;

	unsigned int mFromIndex : 17;
	int mFromSlot : 11;

	unsigned int mRepIndex;

	unsigned int mRepPos : 17;
	unsigned int mMatchLen : 14;

	int mNumLiterals;
	int mScore;
} CompressionU_arrival;

typedef int CompressionU_visited;

typedef struct _CompressionU_compressor
{
	unsigned long long* mIntervals;
	unsigned long long* mPosData;
	CompressionU_match* mMatch;
	CompressionU_match* mBestMatch;
	CompressionU_arrival* mArrival;
	int* mFirstIndexForByte;
	int* mNextIndexForPos;
	int* mIndexCache;
	int mBlockSize;
	int mMaxOffset;
	int mMaxArrivalsByIndex;
	const unsigned char* mpData;
} CompressionU_compressor;


const int CompressionU_kMaxVisitedTimes = 4;
const int CompressionU_kConsecutiveMatches = 3;

// Fill the match tables with data that identifies groups of matches at the start and end of match spans.
// The middle of the spans are deliberately ignored as these are entirely superfluous. It will always be optimal to consider the start or end of match spans.
static int CompressionU_findMatchesAtExhaustive(CompressionU_compressor* pCompressor, const int index, CompressionU_match* pMatches, const int maxMatches)
{
	int* visitedTimes = (int*)pCompressor->mIntervals;
	const unsigned char* mpData = pCompressor->mpData;
	int maxSize = pCompressor->mBlockSize;
	const CompressionU_match* pMaxMatch = pMatches + maxMatches - 2;
	const unsigned int nMaxOffset = (const unsigned int)pCompressor->mMaxOffset;
	CompressionU_match* matchptr = pMatches;

	// There can't be any matches for the first mOffset
	if (index == 0)
	{
		return 0;
	}

	// Descending check
	int lastMatchPos = -2;
	int consecutiveMatches = 0;

	for (int pos = index - 1; pos >= 0; pos--)
	{
		// Ignore offsets that have already been added multiple times
		if (visitedTimes[pos] > CompressionU_kMaxVisitedTimes)
		{
			continue;
		}

		// Exhaustively look for matches
		int matchLen;
		for (matchLen = 0; matchLen < (maxSize - pos) && matchLen < MAX_BLOCK_MATCH_LEN; matchLen++)
		{
			if (mpData[index + matchLen] != mpData[pos + matchLen])
			{
				break;
			}
		}

		const unsigned int matchOffset = (const unsigned int)(index - pos);
		if (matchLen > 0)
		{
			if (matchOffset <= nMaxOffset)
			{
				if (pos == lastMatchPos - 1)
				{
					consecutiveMatches++;
				}
				else
				{
					consecutiveMatches = 0;
				}

				lastMatchPos = pos;

				if (consecutiveMatches < CompressionU_kConsecutiveMatches)
				{
					matchptr->mLength = matchLen;
					matchptr->mOffset = matchOffset;
					matchptr++;

					for (int i = 0; i < matchLen; i++)
					{
						visitedTimes[pos + i]++;
					}
				}
			}
			else
			{
				break;
			}
		}

		if (matchptr >= pMaxMatch)
		{
			printf("Out of match slots descending at $%x\n", index);
			break;
		}

	}

	// Now for ascending matches
	lastMatchPos = -2;
	consecutiveMatches = 0;
	//    pMaxMatch = pMatches + (maxMatches / 2);
	//    pMaxMatch = pMatches + maxMatches;

	for (int pos = 0; pos < index - 1; pos++)
	{
		// Ignore offsets that have already been added multiple times
		if (visitedTimes[pos] > CompressionU_kMaxVisitedTimes)
		{
			continue;
		}

		// Exhaustively look for matches
		int matchLen;
		for (matchLen = 0; matchLen < (maxSize - pos) && matchLen < MAX_BLOCK_MATCH_LEN; matchLen++)
		{
			if (mpData[index + matchLen] != mpData[pos + matchLen])
			{
				break;
			}
		}
		const unsigned int matchOffset = (const unsigned int)(index - pos);
		if (matchLen > 0)
		{
			if (matchOffset <= nMaxOffset)
			{
				if (pos == lastMatchPos + 1)
				{
					consecutiveMatches++;
				}
				else
				{
					consecutiveMatches = 0;
				}

				lastMatchPos = pos;

				if (consecutiveMatches < CompressionU_kConsecutiveMatches)
				{
					matchptr->mLength = matchLen;
					matchptr->mOffset = matchOffset;
					matchptr++;

					for (int i = 0; i < matchLen; i++)
					{
						visitedTimes[pos + i]++;
					}
				}
				else
				{
					if (matchLen > 4)
					{
						pos += matchLen - 3;
					}
				}

			}
			else
			{
				break;
			}
		}

		if (matchptr >= pMaxMatch)
		{
			printf("Out of match slots ascending at $%x\n", index);
			break;
		}

	}

	return (int)(matchptr - pMatches);
}

void CompressionU_findAllMatches(CompressionU_compressor* pCompressor, const int matchesPerIndex, const int endIndex)
{
	CompressionU_match* pMatch = pCompressor->mMatch;
	int i;
	int totalMatches = 0;
	int largestMatches = 0;

	// Reuse this for our information
	memset(pCompressor->mIntervals, 0, sizeof(int) * endIndex);

	for (i = 0; i < endIndex; i++)
	{
		const int nMatches = CompressionU_findMatchesAtExhaustive(pCompressor, i, pMatch, matchesPerIndex);

		totalMatches += nMatches;
		if (nMatches > largestMatches)
		{
			largestMatches = nMatches;
		}

		if (nMatches < matchesPerIndex)
		{
			memset(pMatch + nMatches, 0, (matchesPerIndex - nMatches) * sizeof(CompressionU_match));
		}

		pMatch += matchesPerIndex;
	}

	printf("Completed findAllMatches with %d matches and %d largestMatches\n", totalMatches, largestMatches);
}

static int CompressionU_packedValuePreShiftBits(int i)
{
	i |= (i >> 1);
	i |= (i >> 2);
	i |= (i >> 4);
	i |= (i >> 8);
	i |= (i >> 16);
	i = (i - (i >> 1));
	return i;
}

static int CompressionU_getPackedValueSize(const int value)
{
	if (value == 0)
	{
		return -1;
	}
	int i = CompressionU_packedValuePreShiftBits(value);
	int nBits = 0;

	while ((i >>= 1) > 0)
	{
		nBits++;
		nBits++;
	}

	nBits++;

	return nBits;
}

static int CompressionU_commonPackedBitUpdatePre(unsigned char* pOutData, int nOutOffset, const int nMaxOutDataSize, int* nCurBitsOffset, int* nCurBitShift)
{
	if (nOutOffset >= 0)
	{
		if ((*nCurBitShift) == -1)
		{
			// Need a new byte for bits
			if (nOutOffset >= nMaxOutDataSize) return -1;
			(*nCurBitsOffset) = nOutOffset;
			(*nCurBitShift) = 7;
			pOutData[nOutOffset++] = 0;
		}

	}
	return nOutOffset;
}
static void CompressionU_commonPackedBitUpdatePost(int* nCurBitShift)
{
	(*nCurBitShift)--;
}

static int CompressionU_writeZeroBit(unsigned char* pOutData, int nOutOffset, const int nMaxOutDataSize, int* nCurBitsOffset, int* nCurBitShift)
{
	nOutOffset = CompressionU_commonPackedBitUpdatePre(pOutData, nOutOffset, nMaxOutDataSize, nCurBitsOffset, nCurBitShift);
	CompressionU_commonPackedBitUpdatePost(nCurBitShift);
	return nOutOffset;
}

static int CompressionU_writeOneBit(unsigned char* pOutData, int nOutOffset, const int nMaxOutDataSize, int* nCurBitsOffset, int* nCurBitShift)
{
	nOutOffset = CompressionU_commonPackedBitUpdatePre(pOutData, nOutOffset, nMaxOutDataSize, nCurBitsOffset, nCurBitShift);
	pOutData[(*nCurBitsOffset)] |= 1 << (*nCurBitShift);
	CompressionU_commonPackedBitUpdatePost(nCurBitShift);
	return nOutOffset;
}

static int CompressionU_writeDataBit(unsigned char* pOutData, int nOutOffset, const int nMaxOutDataSize, const int value, int* nCurBitsOffset, int* nCurBitShift)
{
	if (value == 0)
	{
		return CompressionU_writeZeroBit(pOutData, nOutOffset, nMaxOutDataSize, nCurBitsOffset, nCurBitShift);
	}
	return CompressionU_writeOneBit(pOutData, nOutOffset, nMaxOutDataSize, nCurBitsOffset, nCurBitShift);
}

static int CompressionU_writeNormalPackedValue(unsigned char* pOutData, int nOutOffset, const int nMaxOutDataSize, const int value, int* nCurBitsOffset, int* nCurBitShift)
{
	int i = CompressionU_packedValuePreShiftBits(value);

	while ((i >>= 1) > 0)
	{
		nOutOffset = CompressionU_writeZeroBit(pOutData, nOutOffset, nMaxOutDataSize, nCurBitsOffset, nCurBitShift);
		nOutOffset = CompressionU_writeDataBit(pOutData, nOutOffset, nMaxOutDataSize, (value & i) ? 1 : 0, nCurBitsOffset, nCurBitShift);
	}

	return CompressionU_writeOneBit(pOutData, nOutOffset, nMaxOutDataSize, nCurBitsOffset, nCurBitShift);
}

static int CompressionU_writeSplitPackedValue(unsigned char* pOutData, int nOutOffset, const int nMaxOutDataSize, const int value, int* nCurBitsOffset, int* nCurBitShift)
{
	int i = CompressionU_packedValuePreShiftBits(value);

	i >>= 1;
	nOutOffset = CompressionU_writeDataBit(pOutData, nOutOffset, nMaxOutDataSize, (value & i) ? 1 : 0, nCurBitsOffset, nCurBitShift);

	while ((i >>= 1) > 0)
	{
		nOutOffset = CompressionU_writeZeroBit(pOutData, nOutOffset, nMaxOutDataSize, nCurBitsOffset, nCurBitShift);
		nOutOffset = CompressionU_writeDataBit(pOutData, nOutOffset, nMaxOutDataSize, (value & i) ? 1 : 0, nCurBitsOffset, nCurBitShift);
	}

	return CompressionU_writeOneBit(pOutData, nOutOffset, nMaxOutDataSize, nCurBitsOffset, nCurBitShift);
}

static inline int CompressionU_getLiteralsLengthSize(const int length)
{
	if (length == 0)
	{
		return 0;
	}
	return TOKEN_SIZE + CompressionU_getPackedValueSize(length);
}

#define CompressionU_getMatchLenSizeNoRep(__length) CompressionU_getPackedValueSize((__length) - 1)

#define CompressionU_getMatchLenSizeRep(__length) CompressionU_getPackedValueSize(__length)

// Uses snippets originally from: https://github.com/emmanuel-marty/salvador/blob/main/src/shrink.c
static void CompressionU_insertForwardMatch(CompressionU_compressor* pCompressor, const unsigned char* mpData, const int i, const int matchOffset, const int endIndex, const int nDepth)
{
	const CompressionU_arrival* mArrival = pCompressor->mArrival + (i * pCompressor->mMaxArrivalsByIndex);
	const int* rle_len = (const int*)pCompressor->mIntervals;
	CompressionU_visited* visited = ((CompressionU_visited*)pCompressor->mPosData);
	int j;

	for (j = 0; j < NINITIAL_ARRIVALS_PER_POSITION && mArrival[j].mFromSlot; j++)
	{
		if (mArrival[j].mNumLiterals)
		{
			const int nRepOffset = mArrival[j].mRepIndex;

			if (matchOffset != nRepOffset)
			{
				const int nRepPos = mArrival[j].mRepPos;

				if (nRepPos >= 0 && nRepPos < endIndex && visited[nRepPos] != matchOffset)
				{
					visited[nRepPos] = matchOffset;

					CompressionU_match* pFwdMatch = pCompressor->mMatch + (nRepPos * NMATCHES_PER_INDEX);

					if (pFwdMatch[NMATCHES_PER_INDEX - 1].mLength == 0)
					{
						if (nRepPos >= matchOffset)
						{
							const unsigned char* pInWindowStart = mpData + nRepPos;

							if (pInWindowStart[0] == pInWindowStart[-matchOffset])
							{
								if (nRepOffset)
								{
									unsigned int nMaxRepLen = endIndex - nRepPos;

									if (nMaxRepLen > LCP_MAX)
										nMaxRepLen = LCP_MAX;
									const unsigned char* pInWindowMax = pInWindowStart + nMaxRepLen;

									const int nLen0 = rle_len[nRepPos - matchOffset];
									const int nLen1 = rle_len[nRepPos];
									unsigned int nMinLen = (nLen0 < nLen1) ? nLen0 : nLen1, r;

									for (r = 0; pFwdMatch[r].mLength; r++)
									{
										if (pFwdMatch[r].mOffset == matchOffset)
										{
											if (nMinLen < pFwdMatch[r].mLength)
												nMinLen = pFwdMatch[r].mLength;
											break;
										}
									}

									const unsigned char* pInWindowAtRepOffset = pInWindowStart + nMinLen;

									if (pInWindowAtRepOffset > pInWindowMax)
									{
										pInWindowAtRepOffset = pInWindowMax;
									}

									while ((pInWindowAtRepOffset + 8) < pInWindowMax && !memcmp(pInWindowAtRepOffset, pInWindowAtRepOffset - matchOffset, 8))
									{
										pInWindowAtRepOffset += 8;
									}
									while ((pInWindowAtRepOffset + 4) < pInWindowMax && !memcmp(pInWindowAtRepOffset, pInWindowAtRepOffset - matchOffset, 4))
									{
										pInWindowAtRepOffset += 4;
									}
									while (pInWindowAtRepOffset < pInWindowMax && pInWindowAtRepOffset[0] == pInWindowAtRepOffset[-matchOffset])
									{
										pInWindowAtRepOffset++;
									}

									const unsigned short curRepLen = (const unsigned short)(pInWindowAtRepOffset - pInWindowStart);

									if (!pFwdMatch[r].mLength)
									{
										pFwdMatch[r].mLength = curRepLen;
										pFwdMatch[r].mOffset = matchOffset;

										if (nDepth < 59)
										{
											CompressionU_insertForwardMatch(pCompressor, mpData, nRepPos, matchOffset, endIndex, nDepth + 1);
										}
									}
									else
									{
										if (pFwdMatch[r].mLength < curRepLen)
										{
											pFwdMatch[r].mLength = curRepLen;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

static void CompressionU_optimizeForward(CompressionU_compressor* pCompressor, const unsigned char* mpData, const int endIndex, const int nInsertForwardReps, const int* nCurRepMatchOffset, const int nArrivalsPerPosition)
{
	const int nMaxArrivalsPerPosition = pCompressor->mMaxArrivalsByIndex;
	CompressionU_arrival* mArrival = pCompressor->mArrival;
	const int* rle_len = (const int*)pCompressor->mIntervals;
	CompressionU_arrival* cur_arrival;
	int i;

	if ((endIndex) > pCompressor->mBlockSize) return;

	for (i = 0; i != ((endIndex + 1) * nMaxArrivalsPerPosition); i += nMaxArrivalsPerPosition)
	{
		int j;

		memset(mArrival + i, 0, sizeof(CompressionU_arrival) * nMaxArrivalsPerPosition);

		for (j = 0; j < nMaxArrivalsPerPosition; j++)
		{
			mArrival[i + j].mCost = 0x40000000;
		}
	}

	mArrival[nMaxArrivalsPerPosition].mCost = 0;
	mArrival[nMaxArrivalsPerPosition].mFromSlot = -1;
	mArrival[nMaxArrivalsPerPosition].mRepIndex = *nCurRepMatchOffset;

	if (nInsertForwardReps)
	{
		CompressionU_visited* visited = ((CompressionU_visited*)pCompressor->mPosData);

		memset(visited, 0, endIndex * sizeof(CompressionU_visited));
	}

	for (i = 0, cur_arrival = mArrival; i != endIndex; i++, cur_arrival += nMaxArrivalsPerPosition)
	{
		CompressionU_arrival* pDestLiteralSlots = &cur_arrival[nMaxArrivalsPerPosition];
		int j, m;

		for (j = 0; j < nArrivalsPerPosition && cur_arrival[j].mFromSlot; j++)
		{
			const int numLiterals = cur_arrival[j].mNumLiterals + 1;
			const int nCodingChoiceCost = cur_arrival[j].mCost + 8 /* for the literal */ + (((numLiterals & (numLiterals - 1)) == 0) ? 2 : 0);
			const int nScore = cur_arrival[j].mScore + 1;
			const int nRepOffset = cur_arrival[j].mRepIndex;

			if (nCodingChoiceCost < pDestLiteralSlots[nArrivalsPerPosition - 1].mCost ||
				(nCodingChoiceCost == pDestLiteralSlots[nArrivalsPerPosition - 1].mCost &&
				nScore < pDestLiteralSlots[nArrivalsPerPosition - 1].mScore &&
				nRepOffset != pDestLiteralSlots[nArrivalsPerPosition - 1].mRepIndex))
			{
				int exists = 0, n;

				for (n = 0; pDestLiteralSlots[n].mCost < nCodingChoiceCost; n++)
				{
					if (pDestLiteralSlots[n].mRepIndex == nRepOffset)
					{
						exists = 1;
						break;
					}
				}

				if (!exists)
				{
					for (;  pDestLiteralSlots[n].mCost == nCodingChoiceCost && nScore >= pDestLiteralSlots[n].mScore; n++)
					{
						if (pDestLiteralSlots[n].mRepIndex == nRepOffset)
						{
							exists = 1;
							break;
						}
					}

					if (!exists)
					{
						int z;

						for (z = n; z < nArrivalsPerPosition - 1 && pDestLiteralSlots[z].mCost == nCodingChoiceCost; z++)
						{
							if (pDestLiteralSlots[z].mRepIndex == nRepOffset)
							{
								exists = 1;
								break;
							}
						}

						if (!exists)
						{
							for (; z < nArrivalsPerPosition - 1 && pDestLiteralSlots[z].mFromSlot; z++)
							{
								if (pDestLiteralSlots[z].mRepIndex == nRepOffset)
									break;
							}

							memmove(&pDestLiteralSlots[n + 1],
								&pDestLiteralSlots[n],
								sizeof(CompressionU_arrival) * (z - n));

							CompressionU_arrival* pDestArrival = &pDestLiteralSlots[n];
							pDestArrival->mCost = nCodingChoiceCost;
							pDestArrival->mFromIndex = i;
							pDestArrival->mFromSlot = j + 1;
							pDestArrival->mRepIndex = nRepOffset;
							pDestArrival->mRepPos = cur_arrival[j].mRepPos;
							pDestArrival->mMatchLen = 0;
							pDestArrival->mNumLiterals = numLiterals;
							pDestArrival->mScore = nScore;
						}
					}
				}
			}
		}

		if (i == 0) continue;

		const int nNumArrivalsForThisPos = j;
		int nOverallMinRepLen = 0, nOverallMaxRepLen = 0;

		int nRepMatchArrivalIdx[(2 * NMAX_ARRIVALS_PER_POSITION) + 1];
		int nNumRepMatchArrivals = 0;

		if (i < endIndex)
		{
			unsigned int nMaxRepLenForPos = endIndex - i;
			if (nMaxRepLenForPos > LCP_MAX)
				nMaxRepLenForPos = LCP_MAX;

			const unsigned char* pInWindowStart = mpData + i;
			const unsigned char* pInWindowMax = pInWindowStart + nMaxRepLenForPos;

			for (j = 0; j < nNumArrivalsForThisPos; j++)
			{
				if (cur_arrival[j].mNumLiterals)
				{
					const int nRepOffset = cur_arrival[j].mRepIndex;

					if (i >= nRepOffset)
					{
						if (pInWindowStart[0] == pInWindowStart[-nRepOffset])
						{
							if (nRepOffset)
							{
								const int nLen0 = rle_len[i - nRepOffset];
								const int nLen1 = rle_len[i];
								const int nMinLen = (nLen0 < nLen1) ? nLen0 : nLen1;
								const unsigned char* pInWindowAtPos = pInWindowStart + nMinLen;

								if (pInWindowAtPos > pInWindowMax)
									pInWindowAtPos = pInWindowMax;

								while ((pInWindowAtPos + 8) < pInWindowMax && !memcmp(pInWindowAtPos, pInWindowAtPos - nRepOffset, 8))
									pInWindowAtPos += 8;
								while ((pInWindowAtPos + 4) < pInWindowMax && !memcmp(pInWindowAtPos, pInWindowAtPos - nRepOffset, 4))
									pInWindowAtPos += 4;
								while (pInWindowAtPos < pInWindowMax && pInWindowAtPos[0] == pInWindowAtPos[-nRepOffset])
									pInWindowAtPos++;
								const int curRepLen = (const int)(pInWindowAtPos - pInWindowStart);

								if (nOverallMaxRepLen < curRepLen)
									nOverallMaxRepLen = curRepLen;
								nRepMatchArrivalIdx[nNumRepMatchArrivals++] = j;
								nRepMatchArrivalIdx[nNumRepMatchArrivals++] = curRepLen;
							}
						}
					}
				}
			}
		}

		nRepMatchArrivalIdx[nNumRepMatchArrivals] = -1;

		const CompressionU_match* thisMatch = pCompressor->mMatch + (i * NMATCHES_PER_INDEX);

		for (m = 0; m < NMATCHES_PER_INDEX && thisMatch[m].mLength; m++)
		{
			int nOrigMatchLen = thisMatch[m].mLength;
			const int nOrigMatchOffset = thisMatch[m].mOffset;
			unsigned int d = 0;

			if ((i + nOrigMatchLen) > endIndex)
				nOrigMatchLen = endIndex - i;

			const int matchLen = nOrigMatchLen - d;
			const int matchOffset = nOrigMatchOffset - d;
			int nNonRepMatchArrivalIdx, nStartingMatchLen, k;

			if (nInsertForwardReps)
			{
				CompressionU_insertForwardMatch(pCompressor, mpData, i, matchOffset, endIndex, 0);
			}

			nNonRepMatchArrivalIdx = -1;
			for (j = 0; j < nNumArrivalsForThisPos; j++)
			{
				const int nRepOffset = cur_arrival[j].mRepIndex;

				if (matchOffset != nRepOffset || cur_arrival[j].mNumLiterals == 0)
				{
					nNonRepMatchArrivalIdx = j;
					break;
				}
			}

			if (nNonRepMatchArrivalIdx >= 0)
			{
				// Literals have a cost impact
				const int nNoRepmatchOffsetCost = cur_arrival[nNonRepMatchArrivalIdx].mCost + OFFSET_COST(matchOffset);
				const int nNoRepmatchScore = cur_arrival[nNonRepMatchArrivalIdx].mScore + 3;

				// Insert candidate
				if (matchLen < LEAVE_ALONE_MATCH_SIZE)
				{
					nStartingMatchLen = 2;
				}
				else
				{
					nStartingMatchLen = matchLen;
				}

				for (k = nStartingMatchLen; k <= matchLen; k++)
				{
					CompressionU_arrival* pDestSlots = &cur_arrival[k * nMaxArrivalsPerPosition];
					const int nMatchLenCost = CompressionU_getMatchLenSizeNoRep(k) + TOKEN_SIZE;
					const int nCodingChoiceCost = nMatchLenCost + nNoRepmatchOffsetCost;

					if (nCodingChoiceCost < pDestSlots[nArrivalsPerPosition - 2].mCost ||
						(nCodingChoiceCost == pDestSlots[nArrivalsPerPosition - 2].mCost &&
						nNoRepmatchScore < pDestSlots[nArrivalsPerPosition - 2].mScore &&
						(nCodingChoiceCost != pDestSlots[nArrivalsPerPosition - 1].mCost || matchOffset != pDestSlots[nArrivalsPerPosition - 1].mRepIndex)))
					{
						int exists = 0, n;

						for (n = 0;
							pDestSlots[n].mCost < nCodingChoiceCost;
							n++)
						{
							if (pDestSlots[n].mRepIndex == matchOffset)
							{
								exists = 1;
								break;
							}
						}

						if (!exists)
						{
							for (;
								pDestSlots[n].mCost == nCodingChoiceCost && nNoRepmatchScore >= pDestSlots[n].mScore;
								n++)
							{
								if (pDestSlots[n].mRepIndex == matchOffset)
								{
									exists = 1;
									break;
								}
							}

							if (!exists)
							{
								int z;

								for (z = n; z < nArrivalsPerPosition - 1 && pDestSlots[z].mCost == nCodingChoiceCost; z++)
								{
									if (pDestSlots[z].mRepIndex == matchOffset)
									{
										exists = 1;
										break;
									}
								}

								if (!exists)
								{
									for (; z < nArrivalsPerPosition - 1 && pDestSlots[z].mFromSlot; z++)
									{
										if (pDestSlots[z].mRepIndex == matchOffset)
											break;
									}

									memmove(&pDestSlots[n + 1],
										&pDestSlots[n],
										sizeof(CompressionU_arrival) * (z - n));

									CompressionU_arrival* pDestArrival = &pDestSlots[n];
									pDestArrival->mCost = nCodingChoiceCost;
									pDestArrival->mFromIndex = i;
									pDestArrival->mFromSlot = nNonRepMatchArrivalIdx + 1;
									pDestArrival->mRepIndex = matchOffset;
									pDestArrival->mRepPos = i;
									pDestArrival->mMatchLen = k;
									pDestArrival->mNumLiterals = 0;
									pDestArrival->mScore = nNoRepmatchScore;
								}
							}
						}
					}
				}


				// Insert more candidates
				if (matchLen < LEAVE_ALONE_MATCH_SIZE || matchLen <= nOverallMinRepLen)
				{
					nStartingMatchLen = nOverallMinRepLen + 1;
				}
				else
				{
					nStartingMatchLen = matchLen;
				}

				for (k = nStartingMatchLen; k <= (nOverallMaxRepLen < matchLen ? nOverallMaxRepLen : matchLen); k++)
				{
					const int nMatchLenCost = CompressionU_getMatchLenSizeRep(k) + TOKEN_SIZE;
					CompressionU_arrival* pDestSlots = &cur_arrival[k * nMaxArrivalsPerPosition];
					int nCurRepMatchArrival;

					for (nCurRepMatchArrival = 0; (j = nRepMatchArrivalIdx[nCurRepMatchArrival]) >= 0; nCurRepMatchArrival += 2)
					{
						if (nRepMatchArrivalIdx[nCurRepMatchArrival + 1] >= k)
						{
							const int nRepCodingChoiceCost = cur_arrival[j].mCost + nMatchLenCost;
							const int nScore = cur_arrival[j].mScore + 2;
							const int nRepOffset = cur_arrival[j].mRepIndex;

							if (nRepCodingChoiceCost < pDestSlots[nArrivalsPerPosition - 1].mCost ||
								(nRepCodingChoiceCost == pDestSlots[nArrivalsPerPosition - 1].mCost &&
								nScore < pDestSlots[nArrivalsPerPosition - 1].mScore &&
								nRepOffset != pDestSlots[nArrivalsPerPosition - 1].mRepIndex))
							{
								int exists = 0, n;

								for (n = 0;
									pDestSlots[n].mCost < nRepCodingChoiceCost;
									n++)
								{
									if (pDestSlots[n].mRepIndex == nRepOffset)
									{
										exists = 1;
										break;
									}
								}

								if (!exists)
								{
									for (;
										pDestSlots[n].mCost == nRepCodingChoiceCost && nScore >= pDestSlots[n].mScore;
										n++)
									{
										if (pDestSlots[n].mRepIndex == nRepOffset)
										{
											exists = 1;
											break;
										}
									}

									if (!exists)
									{
										int z;

										for (z = n; z < nArrivalsPerPosition - 1 && pDestSlots[z].mCost == nRepCodingChoiceCost; z++)
										{
											if (pDestSlots[z].mRepIndex == nRepOffset)
											{
												exists = 1;
												break;
											}
										}

										if (!exists)
										{
											for (; z < nArrivalsPerPosition - 1 && pDestSlots[z].mFromSlot; z++)
											{
												if (pDestSlots[z].mRepIndex == nRepOffset)
													break;
											}

											memmove(&pDestSlots[n + 1],
												&pDestSlots[n],
												sizeof(CompressionU_arrival) * (z - n));

											CompressionU_arrival* pDestArrival = &pDestSlots[n];
											pDestArrival->mCost = nRepCodingChoiceCost;
											pDestArrival->mFromIndex = i;
											pDestArrival->mFromSlot = j + 1;
											pDestArrival->mRepIndex = nRepOffset;
											pDestArrival->mRepPos = i;
											pDestArrival->mMatchLen = k;
											pDestArrival->mNumLiterals = 0;
											pDestArrival->mScore = nScore;
										}
									}
								}
							}
							else
							{
								break;
							}
						}
					}

					if (k <= LEAVE_ALONE_MATCH_SIZE)
						nOverallMinRepLen = k;
					else if (nOverallMaxRepLen == k)
						nOverallMaxRepLen--;
				}
			}

			if (nOrigMatchLen >= 1280 && ((m + 1) >= NMATCHES_PER_INDEX || thisMatch[m + 1].mLength < 512))
				break;
		}
	}

	if (!nInsertForwardReps)
	{
		const CompressionU_arrival* end_arrival = &mArrival[i * nMaxArrivalsPerPosition];
		CompressionU_match* pBestMatch = pCompressor->mBestMatch;

		while (end_arrival->mFromSlot > 0 && end_arrival->mFromIndex < (const unsigned int)endIndex)
		{
			pBestMatch[end_arrival->mFromIndex].mLength = end_arrival->mMatchLen;
			pBestMatch[end_arrival->mFromIndex].mOffset = (end_arrival->mMatchLen) ? end_arrival->mRepIndex : 0;

			end_arrival = &mArrival[(end_arrival->mFromIndex * nMaxArrivalsPerPosition) + (end_arrival->mFromSlot - 1)];
		}
	}
}

static int CompressionU_reduceMergeCommands(CompressionU_compressor* pCompressor, const unsigned char* mpData, const int endIndex, const int* nCurRepMatchOffset)
{
	CompressionU_match* pBestMatch = pCompressor->mBestMatch;
	int i;
	int numLiterals = 1;
	int nRepMatchOffset = *nCurRepMatchOffset;
	int nDidReduce = 0;

	for (i = 1; i < endIndex; )
	{
		CompressionU_match* pMatch = pBestMatch + i;

		if (pMatch->mLength == 0 &&
			(i + 1) < endIndex &&
			pBestMatch[i + 1].mLength >= MIN_ENCODED_MATCH_SIZE &&
			pBestMatch[i + 1].mLength < MAX_VARLEN &&
			pBestMatch[i + 1].mOffset &&
			i >= pBestMatch[i + 1].mOffset &&
			(i + pBestMatch[i + 1].mLength + 1) <= endIndex &&
			(numLiterals != 0 || pBestMatch[i + 1].mOffset != nRepMatchOffset) &&
			!memcmp(mpData + i - (pBestMatch[i + 1].mOffset), mpData + i, pBestMatch[i + 1].mLength + 1))
		{
			int nCurLenSize, nReducedLenSize;

			if (pBestMatch[i + 1].mOffset == nRepMatchOffset)
			{
				nCurLenSize = CompressionU_getLiteralsLengthSize(numLiterals + 1) + 8 + CompressionU_getMatchLenSizeRep(pBestMatch[i + 1].mLength);
			}
			else
			{
				nCurLenSize = CompressionU_getLiteralsLengthSize(numLiterals + 1) + 8 +
					CompressionU_getPackedValueSize(((pBestMatch[i + 1].mOffset - 1) >> 7) + 1) + 7 +
					CompressionU_getMatchLenSizeNoRep(pBestMatch[i + 1].mLength);
			}

			if (numLiterals != 0 && pBestMatch[i + 1].mOffset == nRepMatchOffset && nRepMatchOffset)
			{
				nReducedLenSize = CompressionU_getLiteralsLengthSize(numLiterals) + CompressionU_getMatchLenSizeRep(pBestMatch[i + 1].mLength + 1);
			}
			else
			{
				nReducedLenSize = CompressionU_getLiteralsLengthSize(numLiterals) +
					CompressionU_getPackedValueSize(((pBestMatch[i + 1].mOffset - 1) >> 7) + 1) + 7 +
					CompressionU_getMatchLenSizeNoRep(pBestMatch[i + 1].mLength + 1);
			}

			// Merge benefit?
			if (nReducedLenSize <= nCurLenSize)
			{
				pBestMatch[i].mLength = pBestMatch[i + 1].mLength + 1;
				pBestMatch[i].mOffset = pBestMatch[i + 1].mOffset;
				pBestMatch[i + 1].mLength = 0;
				pBestMatch[i + 1].mOffset = 0;
				nDidReduce = 1;
				continue;
			}
		}

		if (pMatch->mLength >= MIN_ENCODED_MATCH_SIZE)
		{
			// Don't consider the last block, cannot match here
			if ((i + pMatch->mLength) < endIndex)
			{
				int nNextIndex = i + pMatch->mLength;
				int nNextLiterals = 0;

				while (nNextIndex < endIndex && pBestMatch[nNextIndex].mLength == 0)
				{
					nNextLiterals++;
					nNextIndex++;
				}

				if (nNextIndex < endIndex)
				{
					// Is this a match, with literals, then another match?
					if (pBestMatch[nNextIndex].mLength >= MIN_ENCODED_MATCH_SIZE)
					{
						if (numLiterals != 0 && nRepMatchOffset && pMatch->mOffset != nRepMatchOffset && (pBestMatch[nNextIndex].mOffset != pMatch->mOffset || OFFSET_COST(pMatch->mOffset) > OFFSET_COST(pBestMatch[nNextIndex].mOffset)))
						{
							// Check for any missing repmatches
							if (i >= nRepMatchOffset && (i - nRepMatchOffset + pMatch->mLength) <= endIndex)
							{
								int nMaxLen = 0;
								while ((nMaxLen + 8) < pMatch->mLength && !memcmp(mpData + i - nRepMatchOffset + nMaxLen, mpData + i - pMatch->mOffset + nMaxLen, 8))
									nMaxLen += 8;
								while ((nMaxLen + 4) < pMatch->mLength && !memcmp(mpData + i - nRepMatchOffset + nMaxLen, mpData + i - pMatch->mOffset + nMaxLen, 4))
									nMaxLen += 4;
								while (nMaxLen < pMatch->mLength && mpData[i - nRepMatchOffset + nMaxLen] == mpData[i - pMatch->mOffset + nMaxLen])
									nMaxLen++;

								if (nMaxLen >= 1)
								{
									int nCurCommandSize, nReducedCommandSize;

									// Original match length calc
									// High bits of match offset
									nCurCommandSize = CompressionU_getPackedValueSize(((pMatch->mOffset - 1) >> 7) + 1);

									// Low byte of match offset
									nCurCommandSize += 7;

									// Match length
									nCurCommandSize += CompressionU_getMatchLenSizeNoRep(pMatch->mLength);

									// Literals after command
									nCurCommandSize += CompressionU_getLiteralsLengthSize(nNextLiterals);

									// New match length calc
									// Match length
									nReducedCommandSize = CompressionU_getMatchLenSizeRep(nMaxLen);

									// Literals after command
									nReducedCommandSize += (pMatch->mLength - nMaxLen) << 3;
									nReducedCommandSize += CompressionU_getLiteralsLengthSize(nNextLiterals + (pMatch->mLength - nMaxLen));

									// Benefit?
									if (nReducedCommandSize < nCurCommandSize)
									{
										int j;

										pMatch->mOffset = nRepMatchOffset;
										for (j = nMaxLen; j < pMatch->mLength; j++)
										{
											pBestMatch[i + j].mLength = 0;
										}
										pMatch->mLength = nMaxLen;
										nDidReduce = 1;
									}
								}
							}
						}

						if (pBestMatch[nNextIndex].mOffset && pMatch->mOffset != pBestMatch[nNextIndex].mOffset && nRepMatchOffset != pBestMatch[nNextIndex].mOffset && nNextLiterals)
						{
							// Otherwise, try to gain a match forward as well
							if (i >= pBestMatch[nNextIndex].mOffset && (i - pBestMatch[nNextIndex].mOffset + pMatch->mLength) <= endIndex && pMatch->mOffset != nRepMatchOffset)
							{
								int nMaxLen = 0;
								while ((nMaxLen + 8) < pMatch->mLength && !memcmp(mpData + i - pBestMatch[nNextIndex].mOffset + nMaxLen, mpData + i - pMatch->mOffset + nMaxLen, 8))
									nMaxLen += 8;
								while ((nMaxLen + 4) < pMatch->mLength && !memcmp(mpData + i - pBestMatch[nNextIndex].mOffset + nMaxLen, mpData + i - pMatch->mOffset + nMaxLen, 4))
									nMaxLen += 4;
								while (nMaxLen < pMatch->mLength && mpData[i - pBestMatch[nNextIndex].mOffset + nMaxLen] == mpData[i - pMatch->mOffset + nMaxLen])
									nMaxLen++;
								if (nMaxLen >= pMatch->mLength)
								{
									// Replace it
									pMatch->mOffset = pBestMatch[nNextIndex].mOffset;
									nDidReduce = 1;
								}
								else if (nMaxLen >= 2)
								{
									int nPartialSizeBefore, nPartialSizeAfter;

									nPartialSizeBefore = CompressionU_getMatchLenSizeNoRep(pMatch->mLength);
									nPartialSizeBefore += OFFSET_COST(pMatch->mOffset);
									nPartialSizeBefore += CompressionU_getLiteralsLengthSize(nNextLiterals);

									nPartialSizeAfter = CompressionU_getMatchLenSizeRep(nMaxLen);
									nPartialSizeAfter += CompressionU_getLiteralsLengthSize(nNextLiterals + (pMatch->mLength - nMaxLen)) + ((pMatch->mLength - nMaxLen) << 3);

									if (nPartialSizeAfter < nPartialSizeBefore)
									{
										int j;

										// Gain a repmatch that is shorter than the original match
										pMatch->mOffset = pBestMatch[nNextIndex].mOffset;
										for (j = nMaxLen; j < pMatch->mLength; j++)
										{
											pBestMatch[i + j].mLength = 0;
										}
										pMatch->mLength = nMaxLen;
										nDidReduce = 1;
									}
								}
							}
						}
					}

					// Consider smaller matches to check if they win over literals
					// Longer matches will almost always win
					if (pMatch->mLength < 9)
					{
						// Calculate this command current cost */

						int nCurCommandSize = CompressionU_getLiteralsLengthSize(numLiterals);
						// Don't include current command's literal data bits

						if (pMatch->mOffset == nRepMatchOffset && numLiterals != 0 && nRepMatchOffset)
						{
							// Add cost for rep match but don't include 'rep match follows' bit
							nCurCommandSize += CompressionU_getMatchLenSizeRep(pMatch->mLength);
						}
						else
						{
							// Match with offset but don't include 'match with offset follows' bit

							// High bits of match offset
							nCurCommandSize += CompressionU_getPackedValueSize(((pMatch->mOffset - 1) >> 7) + 1);

							// Low byte of match offset
							nCurCommandSize += 7;

							// Match length
							nCurCommandSize += CompressionU_getMatchLenSizeNoRep(pMatch->mLength);
						}

						// Calculate the next command's current cost
						int nNextCommandSize = CompressionU_getLiteralsLengthSize(nNextLiterals);
						// Don't include next command's literal data bits

						// Rep match or match with offset follows
						nNextCommandSize += 1;

						if (pMatch->mOffset && pBestMatch[nNextIndex].mOffset == pMatch->mOffset && nNextLiterals != 0)
						{
							nNextCommandSize += CompressionU_getMatchLenSizeRep(pBestMatch[nNextIndex].mLength);
						}
						else
						{
							// High bits of match offset
							nNextCommandSize += CompressionU_getPackedValueSize(((pBestMatch[nNextIndex].mOffset - 1) >> 7) + 1);

							// Low byte of match offset
							nNextCommandSize += 7;

							// Match length
							nNextCommandSize += CompressionU_getMatchLenSizeNoRep(pBestMatch[nNextIndex].mLength);
						}

						const int nOriginalCombinedCommandSize = nCurCommandSize + nNextCommandSize;

						// Calculate the mCost of replacing this match command by literals + the next command with the mCost of encoding these literals */
						int nReducedCommandSize = (pMatch->mLength << 3);
						nReducedCommandSize += CompressionU_getLiteralsLengthSize(numLiterals + pMatch->mLength + nNextLiterals);
						// Don't include current + next command's literal data bits

						if (pBestMatch[nNextIndex].mOffset == nRepMatchOffset && (numLiterals + pMatch->mLength + nNextLiterals) != 0 && nRepMatchOffset)
						{
							// Rep match but don't include 'rep match follows' bit

							nReducedCommandSize += CompressionU_getMatchLenSizeRep(pBestMatch[nNextIndex].mLength);
						}
						else
						{
							// Match with offset but don't include 'match with offset follows' bit

							// High bits of match offset
							nReducedCommandSize += CompressionU_getPackedValueSize(((pBestMatch[nNextIndex].mOffset - 1) >> 7) + 1);

							// Low byte of match offset
							nReducedCommandSize += 7;

							// Match length
							nReducedCommandSize += CompressionU_getMatchLenSizeNoRep(pBestMatch[nNextIndex].mLength);
						}

						// Any gain?
						if (nOriginalCombinedCommandSize >= nReducedCommandSize)
						{
							const int matchLen = pMatch->mLength;
							int j;

							for (j = 0; j < matchLen; j++)
							{
								pBestMatch[i + j].mLength = 0;
							}

							nDidReduce = 1;
							continue;
						}
					}
				}
			}

			if ((i + pMatch->mLength) < endIndex && pMatch->mOffset && pMatch->mLength >= MIN_ENCODED_MATCH_SIZE &&
				pBestMatch[i + pMatch->mLength].mOffset &&
				pBestMatch[i + pMatch->mLength].mLength >= MIN_ENCODED_MATCH_SIZE &&
				(pMatch->mLength + pBestMatch[i + pMatch->mLength].mLength) <= MAX_VARLEN &&
				(i + pMatch->mLength) >= pMatch->mOffset &&
				(i + pMatch->mLength) >= pBestMatch[i + pMatch->mLength].mOffset &&
				(i + pMatch->mLength + pBestMatch[i + pMatch->mLength].mLength) <= endIndex &&
				!memcmp(mpData + i - pMatch->mOffset + pMatch->mLength,
				mpData + i + pMatch->mLength - pBestMatch[i + pMatch->mLength].mOffset,
				pBestMatch[i + pMatch->mLength].mLength))
			{

				int nNextIndex = i + pMatch->mLength + pBestMatch[i + pMatch->mLength].mLength;
				int nNextLiterals = 0;

				while (nNextIndex < endIndex && pBestMatch[nNextIndex].mLength == 0)
				{
					nNextIndex++;
					nNextLiterals++;
				}

				int nCurPartialSize;
				if (pMatch->mOffset == nRepMatchOffset && numLiterals != 0)
				{
					// Rep match but don't include 'rep-match follows' bit.
					nCurPartialSize = CompressionU_getMatchLenSizeRep(pMatch->mLength);
				}
				else
				{
					// Match with offset but don't include 'match with offset follows' bit.

					// High bits of match offset
					nCurPartialSize = CompressionU_getPackedValueSize(((pMatch->mOffset - 1) >> 7) + 1);

					// Low byte of match offset
					nCurPartialSize += 7;

					// Match length
					nCurPartialSize += CompressionU_getMatchLenSizeNoRep(pMatch->mLength);
				}

				// Match with offset
				nCurPartialSize += 1;

				// High bits of match offset
				nCurPartialSize += CompressionU_getPackedValueSize(((pBestMatch[i + pMatch->mLength].mOffset - 1) >> 7) + 1);

				// Low byte of match offset
				nCurPartialSize += 7;

				// Match length
				nCurPartialSize += CompressionU_getMatchLenSizeNoRep(pBestMatch[i + pMatch->mLength].mLength);

				if (nNextIndex < endIndex)
				{
					if (pBestMatch[i + pMatch->mLength].mOffset && pBestMatch[nNextIndex].mOffset == pBestMatch[i + pMatch->mLength].mOffset && nNextLiterals != 0)
					{
						// Rep match but don't include 'rep-match follows' bit.
						nCurPartialSize += CompressionU_getMatchLenSizeRep(pBestMatch[nNextIndex].mLength);
					}
					else
					{
						// Match with offset but don't include 'match with offset follows' bit.

						// High bits of match offset
						nCurPartialSize += CompressionU_getPackedValueSize(((pBestMatch[nNextIndex].mOffset - 1) >> 7) + 1);

						// Low byte of match offset
						nCurPartialSize += 7;

						// Match length
						nCurPartialSize += CompressionU_getMatchLenSizeNoRep(pBestMatch[nNextIndex].mLength);
					}
				}

				int nReducedPartialSize;
				if (pMatch->mOffset == nRepMatchOffset && numLiterals != 0 && nRepMatchOffset)
				{
					// Rep match but don't include 'rep-match follows' bit.

					// Match length
					nReducedPartialSize = CompressionU_getMatchLenSizeRep(pMatch->mLength + pBestMatch[i + pMatch->mLength].mLength);
				}
				else
				{
					// Match with offset but don't include 'match with offset follows' bit.

					// High bits of match offset
					nReducedPartialSize = CompressionU_getPackedValueSize(((pMatch->mOffset - 1) >> 7) + 1);

					// Low byte of match offset
					nReducedPartialSize += 7;

					// Match length
					nReducedPartialSize += CompressionU_getMatchLenSizeNoRep(pMatch->mLength + pBestMatch[i + pMatch->mLength].mLength);
				}

				int nCannotReduce = 0;
				if (nNextIndex < endIndex)
				{
					if (pMatch->mOffset && pBestMatch[nNextIndex].mOffset == pMatch->mOffset && nNextLiterals != 0)
					{
						// Rep match but don't include 'rep-match follows' bit.
						nReducedPartialSize += CompressionU_getMatchLenSizeRep(pBestMatch[nNextIndex].mLength);
					}
					else
					{
						if (pBestMatch[nNextIndex].mLength >= MIN_ENCODED_MATCH_SIZE)
						{
							// Match with offset but don't include 'match with offset follows' bit.

							// High bits of match offset
							nReducedPartialSize += CompressionU_getPackedValueSize(((pBestMatch[nNextIndex].mOffset - 1) >> 7) + 1);

							// Low byte of match offset
							nReducedPartialSize += 7;

							// Match length
							nReducedPartialSize += CompressionU_getMatchLenSizeNoRep(pBestMatch[nNextIndex].mLength);
						}
						else
						{
							nCannotReduce = 1;
						}
					}
				}

				// Can join?
				if (nCurPartialSize >= nReducedPartialSize && !nCannotReduce)
				{
					const int matchLen = pMatch->mLength;

					pMatch->mLength += pBestMatch[i + matchLen].mLength;
					pBestMatch[i + matchLen].mLength = 0;
					pBestMatch[i + matchLen].mOffset = 0;
					nDidReduce = 1;
					continue;
				}
			}

			if (numLiterals != 0 && pMatch->mOffset != nRepMatchOffset && pMatch->mLength == MIN_ENCODED_MATCH_SIZE && nRepMatchOffset)
			{
				if ((i + MIN_ENCODED_MATCH_SIZE ) < endIndex)
				{
					int nNextIndex = i + MIN_ENCODED_MATCH_SIZE;
					int nNextLiterals = 0;

					// Check if we can turn a match plus a one byte rep match into all literals, and reduce the output or keep it the same size
					while (nNextIndex < endIndex && pBestMatch[nNextIndex].mLength == 0)
					{
						nNextLiterals++;
						nNextIndex++;
					}

					if (nNextIndex < endIndex && nNextLiterals != 0 &&
						pBestMatch[nNextIndex].mLength == 1 &&
						pBestMatch[nNextIndex].mOffset == pMatch->mOffset)
					{
						int nNextNextIndex = nNextIndex + 1;
						int nNextNextLiterals = 0;

						while (nNextNextIndex < endIndex && pBestMatch[nNextNextIndex].mLength == 0)
						{
							nNextNextLiterals++;
							nNextNextIndex++;
						}

						if (nNextNextIndex < endIndex && nNextNextLiterals != 0 &&
							pBestMatch[nNextNextIndex].mLength >= MIN_ENCODED_MATCH_SIZE &&
							pBestMatch[nNextNextIndex].mOffset != pBestMatch[nNextIndex].mOffset)
						{
							int nCurCommandSize, nCurRepMatchSize, nReducedCommandSize;

							// First command: match with offset
							nCurCommandSize = CompressionU_getLiteralsLengthSize(numLiterals);
							// Don't include match with offset's literal data bits

							// Match with offset
							nCurCommandSize += 1; // match with offset follows

							// High bits of match offset
							nCurCommandSize += CompressionU_getPackedValueSize(((pMatch->mOffset - 1) >> 7) + 1);

							// Low byte of match offset
							nCurCommandSize += 7;

							// Match length
							nCurCommandSize += 1 /* CompressionU_getMatchLenSizeNoRep(pMatch->mLength) */;

							// Second command: rep-match
							nCurRepMatchSize = CompressionU_getLiteralsLengthSize(nNextLiterals);
							nCurRepMatchSize += (nNextLiterals << 3);

							nCurRepMatchSize += 1; // rep-match follows
							nCurRepMatchSize += 1 /* CompressionU_getMatchLenSizeRep(pBestMatch[nNextIndex].mLength) */;

							// Combined commands as literals
							nReducedCommandSize = CompressionU_getLiteralsLengthSize(numLiterals + MIN_ENCODED_MATCH_SIZE /* pMatch->mLength */ + nNextLiterals + 1 /* pBestMatch[nNextIndex].mLength */);
							// Don't include combined command's literal data bits
							nReducedCommandSize += (MIN_ENCODED_MATCH_SIZE /* pMatch->mLength */ << 3);
							nReducedCommandSize += (nNextLiterals << 3);
							nReducedCommandSize += (1 /* pBestMatch[nNextIndex].mLength */ << 3);

							if ((nCurCommandSize + nCurRepMatchSize) >= nReducedCommandSize)
							{
								int j;

								for (j = 0; j < MIN_ENCODED_MATCH_SIZE /* pMatch->mLength */; j++)
								{
									pBestMatch[i + j].mLength = 0;
								}

								pBestMatch[nNextIndex].mLength = 0;
								nDidReduce = 1;
							}
						}
					}
				}
			}

			nRepMatchOffset = pMatch->mOffset;

			i += pMatch->mLength;
			numLiterals = 0;
		}
		else if (pMatch->mLength == 1)
		{
			if (numLiterals != 0)
			{
				int nNextIndex = i + 1 /* pMatch->mLength */;
				int nNextLiterals = 0;

				while (nNextIndex < endIndex && pBestMatch[nNextIndex].mLength == 0)
				{
					nNextLiterals++;
					nNextIndex++;
				}

				if (nRepMatchOffset != pMatch->mOffset && (nNextIndex < endIndex || !nDidReduce))
				{
					pMatch->mLength = 0;
					pMatch->mOffset = 0;
					nDidReduce = 1;
					continue;
				}

				if (nNextLiterals != 0)
				{
					int nCurPartialSize = CompressionU_getLiteralsLengthSize(numLiterals);
					nCurPartialSize += TOKEN_SIZE + 1 /* CompressionU_getMatchLenSizeRep(pMatch->mLength) */;
					nCurPartialSize += CompressionU_getLiteralsLengthSize(nNextLiterals);

					const int nReducedPartialSize = CompressionU_getLiteralsLengthSize(numLiterals + 1 + nNextLiterals) + 8;

					if (nCurPartialSize >= nReducedPartialSize)
					{
						pMatch->mLength = 0;
						pMatch->mOffset = 0;
						nDidReduce = 1;
						continue;
					}
				}
			}

			numLiterals = 0;
			i++;
		}
		else
		{
			numLiterals++;
			i++;
		}
	}

	return nDidReduce;
}

static int CompressionU_writeBlock(CompressionU_compressor* pCompressor, const unsigned char* mpData, const int endIndex, unsigned char* pOutData, const int nMaxOutDataSize, int* nCurBitsOffset, int* nCurBitShift, int* nCurRepMatchOffset)
{
	CompressionU_match* pBestMatch = pCompressor->mBestMatch;
	int nOutOffset = 0;
	const int nMaxOffset = pCompressor->mMaxOffset;
	int nInFirstLiteralOffset = 0;
	int nIsFirstCommand = 1;
	int nRepMatchOffset = *nCurRepMatchOffset;
	int numLiterals = 0;
	int i;

	bool removedMatch = false;

	// Final optimisation pass
	do
	{
		nInFirstLiteralOffset = 0;
		nIsFirstCommand = 1;
		nRepMatchOffset = *nCurRepMatchOffset;
		numLiterals = 0;

		removedMatch = false;
		for (i = 0; i < endIndex; )
		{
			CompressionU_match* pMatch = pBestMatch + i;

			if (pMatch->mLength >= 2 || (pMatch->mLength == 1 && pMatch->mOffset == nRepMatchOffset && numLiterals != 0))
			{
				const int matchLen = pMatch->mLength;
				const int matchOffset = pMatch->mOffset;

				if (matchOffset < MIN_OFFSET || matchOffset >= nMaxOffset)
				{
					return -1;
				}

				if (nIsFirstCommand && numLiterals == 0)
				{
					// The command should always be a literal so report an error
					return -1;
				}
				if (numLiterals != 0)
				{
					// Ensure the command code for all other literals is written
					nIsFirstCommand = 0;
				}

				// Spot one literal, match with offset 1, one same literal, match with offset 1 and then merge
				if (numLiterals == 1 && matchOffset == 1 && matchLen >= MIN_ENCODED_MATCH_SIZE)
				{
					unsigned char currentLiteral = mpData[i];
					unsigned char matchLiteral = mpData[i + 1];
					CompressionU_match* pNextMatch = pMatch + pMatch->mLength + 1;
					if (pNextMatch->mOffset == 1 && pNextMatch->mLength >= MIN_ENCODED_MATCH_SIZE)
					{
						int nextPos = i + numLiterals + pMatch->mLength + 1;
						if (nextPos < endIndex)
						{
							unsigned char nextMatchLiteral = mpData[nextPos];
							// Is the match the same in all respects?
							if (currentLiteral == matchLiteral && matchLiteral == nextMatchLiteral)
							{
								// Note: Debug
								printf("Grow match at pos $%x : numLiterals $%x matchLen $%x matchOffset $%x\n" , i , numLiterals , matchLen , matchOffset);

								// Grow the match
								pMatch->mLength += pNextMatch->mLength + 1;
								pNextMatch->mLength = 0;
								// Try again...
								numLiterals = 0;
								removedMatch = true;
								break;
							}
						}
					}
				}

				numLiterals = 0;

				nRepMatchOffset = matchOffset;

				i += matchLen;
			}
			else
			{
				if (numLiterals == 0)
					nInFirstLiteralOffset = i;
				numLiterals++;
				i++;
			}
		}
	} while (removedMatch);

	// Final write
	nInFirstLiteralOffset = 0;
	nIsFirstCommand = 1;
	nRepMatchOffset = *nCurRepMatchOffset;
	numLiterals = 0;

	for (i = 0; i < endIndex; )
	{
		const CompressionU_match* pMatch = pBestMatch + i;

		if (pMatch->mLength >= 2 || (pMatch->mLength == 1 && pMatch->mOffset == nRepMatchOffset && numLiterals != 0))
		{
			const int matchLen = pMatch->mLength;
			const int matchOffset = pMatch->mOffset;

			if (matchOffset < MIN_OFFSET || matchOffset >= nMaxOffset)
				return -1;

			if (nIsFirstCommand && numLiterals == 0)
			{
				// The command should always be a literal so report an error
				return -1;
			}

			// Note: Debug
//			printf("Pos $%x : numLiterals $%x matchLen $%x matchOffset $%x\n" , i , numLiterals , matchLen , matchOffset);

			if (numLiterals != 0)
			{
				// Wrote literals
				if (!nIsFirstCommand)
				{
					// If it's not the first command then write the command code
					nOutOffset = CompressionU_writeDataBit(pOutData, nOutOffset, nMaxOutDataSize, 0 /* literals follow */, nCurBitsOffset, nCurBitShift);
					if (nOutOffset < 0) return -1;
				}

				// Ensure the command code for all other literals is written
				nIsFirstCommand = 0;

				nOutOffset = CompressionU_writeNormalPackedValue(pOutData, nOutOffset, nMaxOutDataSize, numLiterals, nCurBitsOffset, nCurBitShift);
				if (nOutOffset < 0) return -1;

				if ((nOutOffset + numLiterals) > nMaxOutDataSize)
					return -1;

				memcpy(pOutData + nOutOffset, mpData + nInFirstLiteralOffset, numLiterals);
				nOutOffset += numLiterals;
			}

			if (matchOffset == nRepMatchOffset && numLiterals != 0)
			{
				// Rep match
				nOutOffset = CompressionU_writeDataBit(pOutData, nOutOffset, nMaxOutDataSize, 0 /* rep match */, nCurBitsOffset, nCurBitShift);
				if (nOutOffset < 0) return -1;

				// Write match length
				nOutOffset = CompressionU_writeNormalPackedValue(pOutData, nOutOffset, nMaxOutDataSize, matchLen, nCurBitsOffset, nCurBitShift);
				if (nOutOffset < 0) return -1;
			}
			else
			{
				// Match with offset
				nOutOffset = CompressionU_writeDataBit(pOutData, nOutOffset, nMaxOutDataSize, 1 /* match with offset */, nCurBitsOffset, nCurBitShift);
				if (nOutOffset < 0) return -1;

				// Write high bits of match offset
				nOutOffset = CompressionU_writeNormalPackedValue(pOutData, nOutOffset, nMaxOutDataSize, ((matchOffset - 1) >> 7) + 1, nCurBitsOffset, nCurBitShift);
				if (nOutOffset < 0) return -1;

				// Write low byte of match offset
				if (nOutOffset >= nMaxOutDataSize)
					return -1;

				pOutData[nOutOffset++] = (255 - ((matchOffset - 1) & 0x7f)) << 1;

				// Write match length
				if (matchLen > 2)
				{
					nOutOffset = CompressionU_writeSplitPackedValue(pOutData, nOutOffset, nMaxOutDataSize, matchLen - 1, nCurBitsOffset, nCurBitShift);
					if (nOutOffset < 0) return -1;
				}
				else
				{
					// Or just its optimal bit
					pOutData[nOutOffset - 1] |= 1;
				}
			}

			numLiterals = 0;

			nRepMatchOffset = matchOffset;

			i += matchLen;

			const int nCurSafeDist = i - nOutOffset;
		}
		else
		{
			if (numLiterals == 0)
				nInFirstLiteralOffset = i;
			numLiterals++;
			i++;
		}
	}

	if (numLiterals != 0)
	{
		// Any remaining literals
		if (!nIsFirstCommand)
		{
			nOutOffset = CompressionU_writeDataBit(pOutData, nOutOffset, nMaxOutDataSize, 0 /* literals follow */, nCurBitsOffset, nCurBitShift);
			if (nOutOffset < 0) return -1;
		}

		nOutOffset = CompressionU_writeNormalPackedValue(pOutData, nOutOffset, nMaxOutDataSize, numLiterals, nCurBitsOffset, nCurBitShift);
		if (nOutOffset < 0) return -1;

		if ((nOutOffset + numLiterals) > nMaxOutDataSize)
			return -1;
		memcpy(pOutData + nOutOffset, mpData + nInFirstLiteralOffset, numLiterals);
		nOutOffset += numLiterals;
	}

	nOutOffset = CompressionU_writeDataBit(pOutData, nOutOffset, nMaxOutDataSize, 1 /* match with offset */, nCurBitsOffset, nCurBitShift);
	if (nOutOffset < 0) return -1;

	nOutOffset = CompressionU_writeNormalPackedValue(pOutData, nOutOffset, nMaxOutDataSize, COMPU_EOD, nCurBitsOffset, nCurBitShift);
	if (nOutOffset < 0) return -1;

	*nCurRepMatchOffset = nRepMatchOffset;
	return nOutOffset;
}

static int CompressionU_optimizeAndWriteBlock(CompressionU_compressor* pCompressor, const unsigned char* mpData, const int nInDataSize, unsigned char* pOutData, const int nMaxOutDataSize, int* nCurBitsOffset, int* nCurBitShift, int* nCurRepMatchOffset)
{
	const int endIndex = nInDataSize;
	int* rle_len = (int*)pCompressor->mIntervals;
	int* mFirstIndexForByte = pCompressor->mFirstIndexForByte;
	int* mNextIndexForPos = pCompressor->mNextIndexForPos;
	int* mIndexCache = pCompressor->mIndexCache;
	int i, nPosition;

	memset(pCompressor->mBestMatch, 0, pCompressor->mBlockSize * sizeof(CompressionU_match));

	// Count identical bytes

	i = 0;
	while (i < endIndex)
	{
		int nRangeStartIdx = i;
		const unsigned char c = mpData[nRangeStartIdx];

		do
		{
			i++;
		} while (i < endIndex && mpData[i] == c);

		while (nRangeStartIdx < i)
		{
			rle_len[nRangeStartIdx] = i - nRangeStartIdx;
			nRangeStartIdx++;
		}
	}

	// Supplement small matches

	memset(mFirstIndexForByte, 0xff, sizeof(int) * 65536);
	memset(mNextIndexForPos, 0xff, sizeof(int) * nInDataSize);

	for (nPosition = 0; nPosition < (endIndex - 1); nPosition++)
	{
		mNextIndexForPos[nPosition] = mFirstIndexForByte[((unsigned int)mpData[nPosition]) | (((unsigned int)mpData[nPosition + 1]) << 8)];
		mFirstIndexForByte[((unsigned int)mpData[nPosition]) | (((unsigned int)mpData[nPosition + 1]) << 8)] = nPosition;
	}

	memset(mIndexCache, 0xff, sizeof(int) * 2048);

	for (nPosition = 1; nPosition < (endIndex - 1); nPosition++)
	{
		CompressionU_match* thisMatch = pCompressor->mMatch + (nPosition * NMATCHES_PER_INDEX);
		const int nMaxMatchLen = ((nPosition + 130) < endIndex) ? 130 : (endIndex - nPosition);
		const unsigned char* pInWindowMax = mpData + nPosition + nMaxMatchLen;
		const unsigned char* pInWindowStart = mpData + nPosition;
		int m = 0;
		int nMatchPos;

		while (m < NMATCHES_PER_INDEX && thisMatch[m].mLength)
		{
			mIndexCache[thisMatch[m].mOffset & 2047] = nPosition;
			m++;
		}

		for (nMatchPos = mNextIndexForPos[nPosition]; m < 16 && nMatchPos >= 0; nMatchPos = mNextIndexForPos[nMatchPos])
		{
			const int matchOffset = nPosition - nMatchPos;

			if (matchOffset <= pCompressor->mMaxOffset)
			{
				int nAlreadyExists = 0;

				if (mIndexCache[matchOffset & 2047] == nPosition)
				{
					int nExistingMatchIdx;

					for (nExistingMatchIdx = 0; nExistingMatchIdx < m; nExistingMatchIdx++)
					{
						if (thisMatch[nExistingMatchIdx].mOffset == matchOffset)
						{
							nAlreadyExists = 1;
							break;
						}
					}
				}

				if (!nAlreadyExists)
				{
					const int nLen0 = rle_len[nMatchPos];
					const int nLen1 = rle_len[nPosition];
					const int nMinLen = (nLen0 < nLen1) ? nLen0 : nLen1;
					const unsigned char* pInWindowAtPos = pInWindowStart + nMinLen;

					if (pInWindowAtPos > pInWindowMax)
						pInWindowAtPos = pInWindowMax;

					while ((pInWindowAtPos + 8) < pInWindowMax && !memcmp(pInWindowAtPos, pInWindowAtPos - matchOffset, 8))
						pInWindowAtPos += 8;
					while ((pInWindowAtPos + 4) < pInWindowMax && !memcmp(pInWindowAtPos, pInWindowAtPos - matchOffset, 4))
						pInWindowAtPos += 4;
					while (pInWindowAtPos < pInWindowMax && pInWindowAtPos[0] == pInWindowAtPos[-matchOffset])
						pInWindowAtPos++;

					thisMatch[m].mLength = (const unsigned short)(pInWindowAtPos - pInWindowStart);
					thisMatch[m].mOffset = matchOffset;
					m++;
				}
			}
			else
			{
				break;
			}
		}
	}

	// Compress and insert additional matches
	CompressionU_optimizeForward(pCompressor, mpData, endIndex, 1 , nCurRepMatchOffset, NINITIAL_ARRIVALS_PER_POSITION);

	// Supplement matches further
	for (nPosition = 1; nPosition < (endIndex - 1); nPosition++)
	{
		CompressionU_match* thisMatch = pCompressor->mMatch + (nPosition * NMATCHES_PER_INDEX);

		if (thisMatch[0].mLength < 8)
		{
			const int nMaxMatchLen = ((nPosition + 130) < endIndex) ? 130 : (endIndex - nPosition);
			const unsigned char* pInWindowMax = mpData + nPosition + nMaxMatchLen;
			const unsigned char* pInWindowStart = mpData + nPosition;
			int m = 0, nInserted = 0;
			int nMatchPos;
			int nMaxForwardPos = nPosition + 2 + 1 + 3;

			if (nMaxForwardPos > (endIndex - 2))
				nMaxForwardPos = endIndex - 2;

			while (m < NMATCHES_PER_INDEX && thisMatch[m].mLength)
			{
				mIndexCache[thisMatch[m].mOffset & 2047] = nPosition;
				m++;
			}

			for (nMatchPos = mNextIndexForPos[nPosition]; m < NMATCHES_PER_INDEX && nMatchPos >= 0; nMatchPos = mNextIndexForPos[nMatchPos])
			{
				const int matchOffset = nPosition - nMatchPos;

				if (matchOffset <= pCompressor->mMaxOffset)
				{
					int nAlreadyExists = 0;

					if (mIndexCache[matchOffset & 2047] == nPosition)
					{
						int nExistingMatchIdx;

						for (nExistingMatchIdx = 0; nExistingMatchIdx < m; nExistingMatchIdx++)
						{
							if (thisMatch[nExistingMatchIdx].mOffset == matchOffset)
							{
								nAlreadyExists = 1;
								break;
							}
						}
					}

					if (!nAlreadyExists)
					{
						int nForwardPos = nPosition + 2 + 1;

						if (nForwardPos >= matchOffset)
						{
							while (nForwardPos < nMaxForwardPos)
							{
								if (mpData[nForwardPos] == mpData[nForwardPos - matchOffset])
								{
									break;
								}
								nForwardPos++;
							}

							if (nForwardPos < nMaxForwardPos)
							{
								const int nLen0 = rle_len[nMatchPos];
								const int nLen1 = rle_len[nPosition];
								const int nMinLen = (nLen0 < nLen1) ? nLen0 : nLen1;
								const unsigned char* pInWindowAtPos = pInWindowStart + nMinLen;

								if (pInWindowAtPos > pInWindowMax)
									pInWindowAtPos = pInWindowMax;

								while ((pInWindowAtPos + 8) < pInWindowMax && !memcmp(pInWindowAtPos, pInWindowAtPos - matchOffset, 8))
									pInWindowAtPos += 8;
								while ((pInWindowAtPos + 4) < pInWindowMax && !memcmp(pInWindowAtPos, pInWindowAtPos - matchOffset, 4))
									pInWindowAtPos += 4;
								while (pInWindowAtPos < pInWindowMax && pInWindowAtPos[0] == pInWindowAtPos[-matchOffset])
									pInWindowAtPos++;

								thisMatch[m].mLength = (const unsigned short)(pInWindowAtPos - pInWindowStart);
								thisMatch[m].mOffset = matchOffset;
								m++;

								CompressionU_insertForwardMatch(pCompressor, mpData, nPosition, matchOffset, endIndex, 8);

								nInserted++;
								if (nInserted >= 10)
									break;
							}
						}
					}
				}
				else
				{
					break;
				}
			}
		}
	}

	// Pick final matches
	CompressionU_optimizeForward(pCompressor, mpData, endIndex, 0 , nCurRepMatchOffset, pCompressor->mMaxArrivalsByIndex);

	// Apply reduction and merge pass
	int nDidReduce;
	int nPasses = 0;
	do
	{
		nDidReduce = CompressionU_reduceMergeCommands(pCompressor, mpData, endIndex, nCurRepMatchOffset);
		nPasses++;
	} while (nDidReduce && nPasses < 20);

	// Write compressed block
	return CompressionU_writeBlock(pCompressor, mpData, endIndex, pOutData, nMaxOutDataSize, nCurBitsOffset, nCurBitShift, nCurRepMatchOffset);
}

static void CompressionU_compressor_destroy(CompressionU_compressor* pCompressor);

static int CompressionU_compressor_init(CompressionU_compressor* pCompressor, const int nBlockSize, const int nMaxWindowSize, const int nMaxArrivals)
{

	pCompressor->mIntervals = NULL;
	pCompressor->mPosData = NULL;
	pCompressor->mMatch = NULL;
	pCompressor->mBestMatch = NULL;
	pCompressor->mArrival = NULL;
	pCompressor->mFirstIndexForByte = NULL;
	pCompressor->mNextIndexForPos = NULL;
	pCompressor->mIndexCache = NULL;
	pCompressor->mBlockSize = nBlockSize;
	pCompressor->mMaxOffset = MAX_OFFSET;
	pCompressor->mMaxArrivalsByIndex = nMaxArrivals;

	pCompressor->mIntervals = (unsigned long long*)malloc(nMaxWindowSize * sizeof(unsigned long long));

	if (pCompressor->mIntervals)
	{
		pCompressor->mPosData = (unsigned long long*)malloc(nMaxWindowSize * sizeof(unsigned long long));

		if (pCompressor->mPosData)
		{
			pCompressor->mArrival = (CompressionU_arrival*)malloc((nBlockSize + 1) * nMaxArrivals * sizeof(CompressionU_arrival));

			if (pCompressor->mArrival)
			{
				pCompressor->mBestMatch = (CompressionU_match*)malloc(nBlockSize * sizeof(CompressionU_match));

				if (pCompressor->mBestMatch)
				{
					pCompressor->mMatch = (CompressionU_match*)malloc(nBlockSize * NMATCHES_PER_INDEX * sizeof(CompressionU_match));
					if (pCompressor->mMatch)
					{
						pCompressor->mFirstIndexForByte = (int*)malloc(65536 * sizeof(int));
						if (pCompressor->mFirstIndexForByte)
						{
							pCompressor->mNextIndexForPos = (int*)malloc(nBlockSize * sizeof(int));
							if (pCompressor->mNextIndexForPos)
							{
								pCompressor->mIndexCache = (int*)malloc(2048 * sizeof(int));
								if (pCompressor->mIndexCache)
								{
									return 0;
								}
							}
						}
					}
				}
			}
		}
	}


	CompressionU_compressor_destroy(pCompressor);
	return 100;
}

static void CompressionU_compressor_destroy(CompressionU_compressor* pCompressor)
{

	if (pCompressor->mIndexCache)
	{
		free(pCompressor->mIndexCache);
		pCompressor->mIndexCache = NULL;
	}

	if (pCompressor->mNextIndexForPos)
	{
		free(pCompressor->mNextIndexForPos);
		pCompressor->mNextIndexForPos = NULL;
	}

	if (pCompressor->mFirstIndexForByte)
	{
		free(pCompressor->mFirstIndexForByte);
		pCompressor->mFirstIndexForByte = NULL;
	}

	if (pCompressor->mMatch)
	{
		free(pCompressor->mMatch);
		pCompressor->mMatch = NULL;
	}

	if (pCompressor->mArrival)
	{
		free(pCompressor->mArrival);
		pCompressor->mArrival = NULL;
	}

	if (pCompressor->mBestMatch)
	{
		free(pCompressor->mBestMatch);
		pCompressor->mBestMatch = NULL;
	}

	if (pCompressor->mPosData)
	{
		free(pCompressor->mPosData);
		pCompressor->mPosData = NULL;
	}

	if (pCompressor->mIntervals)
	{
		free(pCompressor->mIntervals);
		pCompressor->mIntervals = NULL;
	}
}

static int CompressionU_compressorShrinkBlockExhaustive(CompressionU_compressor* pCompressor, const unsigned char* mpData, const int nInDataSize, unsigned char* pOutData, const int nMaxOutDataSize)
{
	int nCurBitsOffset = 0, nCurBitShift = -1;
	int nCurRepMatchOffset = 1;
	int nCompressedSize;

	pCompressor->mpData = mpData;

	CompressionU_findAllMatches(pCompressor, NMATCHES_PER_INDEX, nInDataSize);

	nCompressedSize = CompressionU_optimizeAndWriteBlock(pCompressor, mpData, nInDataSize, pOutData, nMaxOutDataSize, &nCurBitsOffset, &nCurBitShift, &nCurRepMatchOffset);

	return nCompressedSize;
}

size_t CompressionU_compress(const unsigned char* pInputData, unsigned char* pOutBuffer, const size_t nInputSize, const size_t nMaxOutBufferSize)
{
	CompressionU_compressor compressor;
	const int nBlockSize = (nInputSize < BLOCK_SIZE) ? ((nInputSize < 1024) ? 1024 : (int)nInputSize) : BLOCK_SIZE;

	if (CompressionU_compressor_init(&compressor, nBlockSize, nBlockSize * 2, NMAX_ARRIVALS_PER_POSITION) != 0)
	{
		return -1;
	}


	int compSize = CompressionU_compressorShrinkBlockExhaustive(&compressor, pInputData, nInputSize, pOutBuffer, nMaxOutBufferSize);

	CompressionU_compressor_destroy(&compressor);

	if (compSize >= 1 && compSize < (int)nInputSize)
	{
		return compSize;
	}

	return -1;
}







CompressionU::CompressionU()
{

}

CompressionU::~CompressionU()
{
}

int CompressionU::Compress( const u8*src, u32 src_len,u8 *dst, u32 *dst_len,int level)
{
	size_t mLength = CompressionU_compress(src, dst, src_len, 0x10000);
	*dst_len = (u32) mLength;
	return 0;
}
