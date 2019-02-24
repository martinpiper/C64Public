/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#include "Compress.h"
#include "CompressE.h"

namespace RNReplicaNet
{

namespace RNXPCompression
{

// Define this to print out the total bits written during compression
//#define TOTAL_BITS_DEBUG

#define FORCE_BYTE(x)						((unsigned char) (x))
#define FORCE_UNSIGNED(c)					c ## U

CompressionE::CompressionE()
{
}

CompressionE::~CompressionE()
{
}

void CompressionE::BitBufferWriteBits(void)
{
	u8 *p = mBitBufferCurrentOut;
	u32 b = mBitBufferValue;

	p[0] = FORCE_BYTE(b);
}


void CompressionE::BitBufferPutBit(unsigned char bit)
{
#ifdef TOTAL_BITS_DEBUG
	mTotalBits++;
#endif
	// Paranoia checking to ensure only the bottom bit is passed in
	if (bit)
	{
		bit = 1;
	}
	if (mBitBufferCount < 8)
	{
		// Fetch new output pointer
		if (mBitBufferCount == 0)
		{
			mBitBufferCurrentOut = mBitBufferNextOut;
			mBitBufferNextOut++;
		}

		mBitBufferValue = (mBitBufferValue << 1) + bit;
		mBitBufferCount++;
	}
	else
	{
		BitBufferWriteBits();
		mBitBufferCurrentOut = mBitBufferNextOut;
		mBitBufferNextOut++;
		mBitBufferValue = bit;
		mBitBufferCount = 1;
	}
}


void CompressionE::BitBufferPutByte(unsigned char b)
{
#ifdef TOTAL_BITS_DEBUG
	mTotalBits += 8;
#endif

	*mBitBufferNextOut++ = FORCE_BYTE(b);
}


void CompressionE::BitBufferFlushBits(unsigned char filler_bit)
{
#ifdef TOTAL_BITS_DEBUG
	printf("\n//mTotalBits before flush = %d\n",mTotalBits);
#endif

	if (mBitBufferCount > 0)
	{
		while (mBitBufferCount != 8)
		{
			BitBufferPutBit(filler_bit);
		}
		BitBufferWriteBits();
		mBitBufferCount = 0;
	}
	mBitBufferCurrentOut = NULL;
}


void CompressionE::PackValue(u32 i)
{
	if (i >= 2)
	{
		u32 t = 4;
		i += 2;
		do
		{
			t = t << 1;
		} while (i >= t);
		t = t >> 1;
		do
		{
			t = t >> 1;
			BitBufferPutBit((i & t) ? 1 : 0);
			BitBufferPutBit(0);
		} while (t > 2);
	}
	BitBufferPutBit((unsigned)i & 1);
	BitBufferPutBit(1);
}

void CompressionE::EncodeMatch(u32 len, const u32 offset)
{
#ifdef TOTAL_BITS_DEBUG
	printf("//EM%d,%d   ",len,offset);
#endif

	BitBufferPutBit(0);

	if (offset == mLastOffset)
	{
		BitBufferPutBit(0);
		BitBufferPutBit(1);
	}
	else
	{
		PackValue(1 + ((offset - 1) >> 8));
		BitBufferPutByte((unsigned)offset - 1);
	}

	// Here len can be 2,3,4... if offset <= LONG_OFFSET_THRESHOLD
	u32 workLen = len - 1;
	if (offset > LONG_OFFSET_THRESHOLD)
	{
		workLen--;
	}

	if (workLen >= 4)
	{
		// Effectively output 0
		BitBufferPutBit(0);
		BitBufferPutBit(0);
		// Followed by a packed bit representation
		PackValue(workLen - 4);
	}
	else
	{
		BitBufferPutBit(workLen > 1);
		BitBufferPutBit((unsigned)workLen & 1);
	}

	mLastOffset = offset;
}


int CompressionE::CodedMatchLength(u32 len, u32 offset)
{
	int b;
	if (len < 2 || (len == 2 && (offset > LONG_OFFSET_THRESHOLD)))
		return -1;

	u32 workLen = len - 2;
	if (offset > LONG_OFFSET_THRESHOLD)
	{
		workLen--;
	}

	if (offset == mLastOffset)
	{
		b = 1 + 2;
	}
	else
	{
		b = 1 + 10;
		offset = (offset - 1) >> 8;
		while (offset > 0)
		{
			b += 2;
			offset >>= 1;
		}
	}

	b += 2;
	if (workLen < 3)
	{
		return b;
	}

	// MPi: TODO: Research why tweaking this number slightly to 2 produces a 4 bytes saving on the small (53KB) test C64 file.
	workLen -= 3;

	do
	{
		b += 2;
		workLen >>= 1;
	} while (workLen > 0);

	return b;
}

//static FILE *fp = 0;

void CompressionE::EncodeLiteralRun(const u8 *litRun, u32 currentLitNum)
{
#ifdef TOTAL_BITS_DEBUG
	printf("//EL%d   ",currentLitNum);
#endif

	if (currentLitNum == 0)
	{
		return;
	}

	do
	{
		BitBufferPutBit(1);
#ifdef TOTAL_BITS_DEBUG
//		printf("$%x ",*litRun);
#endif

		BitBufferPutByte(*litRun++);
		currentLitNum--;

	} while (currentLitNum > 0);
}


int CompressionE::Compress( const u8 *in, u32 inLen,u8 *out, u32 *outLen,int compLevel)
{
	if ((compLevel < 1) || (compLevel > 10))
	{
		return GBA_E_INVALID_ARGUMENT;
	}

	if (gXPCompressionTweak2 > gXPCompressionTweak3)
	{
		return -1;
	}
	if (gXPCompressionTweak1 > gXPCompressionTweak2)
	{
		return -1;
	}

	mHashTable = 0;

	mTotalBits = 0;
	mLastOffset = 1;
	mInLen = inLen;
	mCurBuf = in;
	mBufEnd = in + inLen;

	mBitBufferValue = 0;
	mBitBufferCount = 0;
	mBitBufferCurrentOut = 0;
	mBitBufferNextOut = out;

	// Now the parts which decide compression speed and level
	mUpperLimitForwardMatchLen = compLevel * 1024;
	mMaximumRangeCheck = (compLevel * 32) / 11;
	mMaximumDictionaryBlocks = (compLevel * 131072) / 11;
	mDoExtendedDictionaryAdd = false;
	mDoForwardCheck = false;
	mUseDictionary = false;
	// MPi: TODO: For now we will always have the dictionary on even on the lowest compression.
	// It will be using only a small dictionary though.
//	if (compLevel > 1)		// Hence this "if" is commented out.
	{
		mUseDictionary = true;
	}
	if (compLevel > 6)
	{
		// Makes compression ~3 times slower than level 6
		mDoForwardCheck = true;
	}
	if (compLevel > 8)
	{
		// Makes compression ~2 times slower than level 8
		mDoExtendedDictionaryAdd = true;
	}
	// Special case for turning everything on to a sensible maximum
	if (compLevel == 10)
	{
		mUpperLimitForwardMatchLen = 0x7fffffff;
		mMaximumRangeCheck = 96;
		mMaximumDictionaryBlocks = 131072;
	}

	// If the input length is larger than this then switch to using the hash table.
	// The idea being that even though it uses more memory and constructs (clears) the table it is actually quicker for larger input blocks.
	if ((inLen >= (512*1024)) && (mMaximumDictionaryBlocks > 50000))
	{
// mHashTable is slower, even for large blocks. It has been removed. Bah.
//		mHashTable = new HashNode[65536];
	}

	const u8 *litRun = mCurBuf;			// Pointer to the start of the literal run
	u32 currentLitNum = 0;			// Number of literals to encode

	u32 curPos = 0;

	mLastMatchPos = 0x7fffffff;

	while (curPos < inLen)
	{
		if ((u32)(mBitBufferNextOut - out) > inLen)
		{
			return -1;
		}
		// Now try a dictionary match
		int dictionaryBits = -1;
		u32 matchLen,matchOffset;
		Value *pValue = 0;
		if (DictionaryFindMatch(curPos,matchLen,matchOffset,&pValue))
		{
			dictionaryBits = CodedMatchLength(matchLen,matchOffset);
		}

		// Is length of the proposed match smaller than the number of bits needed for a literal version of the match length?
		if ((dictionaryBits != -1) && (dictionaryBits < (int)(matchLen * 9)))
		{
			// Yes, so the match results in a smaller size so we use it.

			// Now do a forward match test
			u32 curPos2 = curPos+1;
			int dictionaryBits2 = -1;
			u32 matchLen2,matchOffset2;
			bool firstMatchIsBetter = true;
			Value *pValue2 = 0;
			// This forward match test takes some extra time
			if ((curPos2 < inLen) && mDoForwardCheck)
			{
				if (DictionaryFindMatch(curPos2,matchLen2,matchOffset2,&pValue2))
				{
					dictionaryBits2 = CodedMatchLength(matchLen2,matchOffset2);

					if ((dictionaryBits2 != -1) && (dictionaryBits2 < (int)(matchLen2 * 9)))
					{
						// Figure out if it is shorter (in terms of bits saved to the file) to skip the first match and encode using this second try instead
						if ( (dictionaryBits + (int) gXPCompressionTweak1 + (int(matchLen2) * gXPCompressionTweak2) - (int(matchLen) * gXPCompressionTweak3) ) > (dictionaryBits2 + gXPCompressionTweak4))
						{
							firstMatchIsBetter = false;
						}
					}
				}
			}

			if (firstMatchIsBetter)
			{
				// Store any literals we have not already output
				EncodeLiteralRun(litRun,currentLitNum);
				// Store the match
				EncodeMatch(matchLen,matchOffset);
				curPos += matchLen;
				litRun = in + curPos;
				currentLitNum = 0;
//				printf("left = %d\n",inLen - curPos);
				continue;
			}
		}

		currentLitNum++;
		curPos++;
	}

	// Store any remaining literals
	EncodeLiteralRun(litRun,currentLitNum);

	// EOF
	BitBufferPutBit(0);
	PackValue(FORCE_UNSIGNED(0x100000));
//	PackValue(FORCE_UNSIGNED(0x1000000));
//	BitBufferPutByte(0xff);
	BitBufferFlushBits(0);

	*outLen = (u32) (mBitBufferNextOut - out);

	// Makes sure the dictionary is freed
	DictionaryFree();

	delete []mHashTable;
	return GBA_E_OK;
}

void CompressionE::DictionaryFree(void)
{
//	printf("mValueByBytes = %d\n",mValueByBytes.size());
	std::list<Value*>::iterator st,en;
	st = mByAge.begin();
	en = mByAge.end();
	while (st != en)
	{
		Value *value = *st++;
		// No need to unlink this value because we are clearing the containers anyway
		delete value;
	}
	mValueByBytes.clear();
	mByAge.clear();
	mByPos.clear();
}

void CompressionE::DictionaryAdd(const u32 fromStartPos)
{
	if ((fromStartPos+1) >= mInLen)
	{
		return;
	}

	Bytes bytes(mCurBuf[fromStartPos],mCurBuf[fromStartPos+1]);
	u16 theHash = bytes.GetHash();
	if (mHashTable)
	{
		if (mHashTable[theHash].mPosAdded.find(fromStartPos) != mHashTable[theHash].mPosAdded.end())
		{
			return;
		}
	}
	else
	{
		// Quicker to do a find rather than allocating the Value block then failing on the first add
		if (mByPos.find(fromStartPos) != mByPos.end())
		{
			return;
		}
	}

#if 1
	Value *value = new Value;
	value->mPos = fromStartPos;
	value->mHashedValue = theHash;
	// Fill in the iterators
	if (mHashTable)
	{
		mHashTable[theHash].mValues.push_back(value);
		value->mHashValues = --mHashTable[theHash].mValues.end();

		std::pair<std::set<u32>::iterator,bool> ret = mHashTable[theHash].mPosAdded.insert(fromStartPos);
		assert(ret.second && "XPCompression couldn't add the pair");
		value->mHashPosAdded = ret.first;
	}
	else
	{
		value->mByBytes = mValueByBytes.insert(std::pair<Bytes,Value *>(bytes,value));
		std::pair<std::set<u32>::iterator,bool> ret = mByPos.insert(fromStartPos);
		assert(ret.second && "XPCompression couldn't add the pair");
		value->mByPos = ret.first;
	}
	mByAge.push_back(value);
	value->mByAge = --mByAge.end();

	// Expire old entries if we get too many
	// Saves memory at the expense of potential compression block matches
	// In most optimised STL implementation x.size() uses a counter variable so it isn't slow.
	if (mByAge.size() > mMaximumDictionaryBlocks)
	{
		// Get the oldest used block from the head of the list
		Value *value = mByAge.front();
		DictionaryFreeValue(value);
	}
#endif
}

void CompressionE::DictionaryFreeValue(Value *value)
{
	if (mHashTable)
	{
		mHashTable[value->mHashedValue].mValues.erase(value->mHashValues);
		mHashTable[value->mHashedValue].mPosAdded.erase(value->mHashPosAdded);
	}
	else
	{
		mValueByBytes.erase(value->mByBytes);
		mByPos.erase(value->mByPos);
	}
	mByAge.erase(value->mByAge);
	delete value;
}

bool CompressionE::DictionaryFindMatch(const u32 fromStartPos,u32 &len,u32 &matchOffset,Value **pValue)
{
	*pValue = 0;
	u32 bestMatchPos = fromStartPos,bestMatchLen = 0;
	u32 maxNumBytesAhead = mInLen - fromStartPos;
	int bestBitLength = 0;

	// Have we searched for this already? If yes, early out.
	if (mLastMatchPos == fromStartPos)
	{
		len = mLastMatchLen;
		matchOffset = mLastMatchOffset;
		*pValue = mLastMatchValue;
		return mLastMatchReturn;
	}

	mLastMatchPos = fromStartPos;
	mLastMatchLen = 0;
	mLastMatchOffset = 0;
	mLastMatchValue = 0;
	mLastMatchReturn = false;

	DictionaryAdd(fromStartPos);

	if (fromStartPos == 0)
	{
		return false;
	}

	// Try a simple backward search, slow but sure
	int pos;
	for (pos = fromStartPos - 1; (pos >= 0) && ((fromStartPos - pos) < mMaximumRangeCheck); pos--)
	{
		const u8 *start = mCurBuf + fromStartPos;
		const u8 *test = mCurBuf + pos;
		u32 i;
		u32 proposedMatchLen = 0;
		for (i=0; (i<mUpperLimitForwardMatchLen) && (i < maxNumBytesAhead);i++)
		{
			if (start[i] != test[i])
			{
				// This DictionaryAdd uses a bit more time and memory but improves the compression
				if (mDoExtendedDictionaryAdd)
				{
					DictionaryAdd(pos + i);
				}
				break;
			}
			proposedMatchLen = i+1;
		}
		if (proposedMatchLen == 0)
		{
			continue;
		}

		int proposedBits = CodedMatchLength(proposedMatchLen,fromStartPos - pos);
		// If we have a daft number of bits then don't use the match at all
		if (proposedBits == -1)
		{
			continue;
		}
		int proposedBitLength = (proposedMatchLen * 9) - proposedBits;

		if (proposedBitLength > bestBitLength)
		{
			bestMatchLen = proposedMatchLen;
			bestMatchPos = pos;
			bestBitLength = proposedBitLength;
		}
		// Logic end begins here
	}

	if (mUseDictionary && (maxNumBytesAhead >= 2) && (bestMatchLen < maxNumBytesAhead))
	{
		// Search the current dictionary for byte sequences that might match what we want
		Bytes bytes(mCurBuf[fromStartPos],mCurBuf[fromStartPos+1]);
		u16 theHash = bytes.GetHash();

		// MPi: TODO: With or without hash table there is quite a lot of common code. Make it common.
		if (mHashTable)
		{
			std::list<Value *>::iterator st = mHashTable[theHash].mValues.begin();
			std::list<Value *>::iterator en = mHashTable[theHash].mValues.end();
			//		printf(".\n");
			while (st != en)
			{
				Value *value = *st++;
				assert(value->mHashedValue == theHash);
				if (CommonValueChecks(value, fromStartPos, bestMatchPos, bestMatchLen, bestBitLength, maxNumBytesAhead))
				{
					*pValue = value;
				}
			}
		}
		else
		{
			std::multimap<Bytes,Value *,ByBytes>::iterator st = mValueByBytes.lower_bound(bytes);
			std::multimap<Bytes,Value *,ByBytes>::iterator en = mValueByBytes.upper_bound(bytes);
			//		printf(".\n");
			while (st != en)
			{
				Value *value = (*st++).second;
				assert(value->mHashedValue == theHash);
				if (CommonValueChecks(value, fromStartPos, bestMatchPos, bestMatchLen, bestBitLength, maxNumBytesAhead))
				{
					*pValue = value;
				}
			}
		}
	}

	if (bestMatchLen == 0)
	{
		return false;
	}

	// Do the forward match test with whatever the match is and calculate a new length based on how many times the sequence repeats
	const u8 *start = mCurBuf + fromStartPos + bestMatchLen;
	const u8 *test = mCurBuf + bestMatchPos + bestMatchLen;
	while ((start < mBufEnd) /*&& (bestMatchLen < mUpperLimitForwardMatchLen)*/)
	{
		if (*start != *test)
		{
			break;
		}
		start++;
		test++;
		bestMatchLen++;
	}

	// Now finally return the best match
	matchOffset = fromStartPos - bestMatchPos;
	len = bestMatchLen;

	mLastMatchLen = len;
	mLastMatchOffset = matchOffset;
	mLastMatchValue = *pValue;
	mLastMatchReturn = true;

	return true;
}



bool CompressionE::CommonValueChecks(Value *value,const u32 fromStartPos, u32 &bestMatchPos, u32 &bestMatchLen, int &bestBitLength, const u32 maxNumBytesAhead)
{
//	printf(".");
	// Check the byte sequences that are found
	u32 pos = value->mPos;
	// Stops us from comparing with the already completed backwards check
	if ((fromStartPos - pos) < mMaximumRangeCheck)
	{
		return false;
	}

	// Logic begins here
	const u8 *start = mCurBuf + fromStartPos;
	const u8 *test = mCurBuf + pos;
	u32 i;
	u32 proposedMatchLen = 0;
	for (i=0; (i<mUpperLimitForwardMatchLen) && (i < maxNumBytesAhead);i++)
	{
		if (start[i] != test[i])
		{
			break;
		}
		proposedMatchLen = i+1;
	}
	if (proposedMatchLen == 0)
	{
		return false;
	}

	int proposedBits = CodedMatchLength(proposedMatchLen,fromStartPos - pos);
	// If we have a daft number of bits then don't use the match at all
	if (proposedBits == -1)
	{
		return false;
	}
	int proposedBitLength = (proposedMatchLen * 9) - proposedBits;

	if (proposedBitLength > bestBitLength)
	{
		bestMatchLen = proposedMatchLen;
		bestMatchPos = pos;
		bestBitLength = proposedBitLength;

		// Because this entry is used it is put back at the end of the list again
		mByAge.erase(value->mByAge);
		mByAge.push_back(value);
		value->mByAge = --mByAge.end();
		return true;

	}

	return false;
}


} // namespace RNXPCompression

} // namespace RNReplicaNet
