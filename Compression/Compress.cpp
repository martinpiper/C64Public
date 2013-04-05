// Harmless warning about browse information
#pragma warning (disable : 4786)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <list>

#include "Compress.h"

// Tweak values
const int gXPCompressionTweak1 = 5;
const int gXPCompressionTweak2 = 5;
const int gXPCompressionTweak3 = 5;
const int gXPCompressionTweak4 = 9;

#define HISTORY_MATCH

#define FORCE_BYTE(x)						((unsigned char) (x))
#define FORCE_UNSIGNED(c)					c ## U

Compression::Compression() : mEnableChoicesOutput(false)
{
	mIgnoreChoicePos.clear();
}

Compression::~Compression()
{
}

void Compression::BitBufferWriteBits(void)
{
	u8 *p = mBitBufferCurrentOut;
	u32 b = mBitBufferValue;

	p[0] = FORCE_BYTE(b);
}


void Compression::BitBufferPutBit(u32 bit)
{
	mTotalBitsOut++;
	// Paranoia check!
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


void Compression::BitBufferPutByte(unsigned char b)
{
	mTotalBitsOut+=8;
	*mBitBufferNextOut++ = FORCE_BYTE(b);
}


void Compression::BitBufferFlushBits(unsigned char filler_bit)
{
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


void Compression::PackValue(u32 i)
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
			BitBufferPutBit(i & t);
			BitBufferPutBit(0);
		} while (t > 2);
	}
	BitBufferPutBit((unsigned)i & 1);
	BitBufferPutBit(1);
}

static u32 PackValueLen(u32 i)
{
	u32 b = 0;
	do
	{
		i = i >> 1;
		b += 2;
	} while (i);
	return b;
}

static u32 PackValueLen2(u32 i)
{
	u32 b = 0;
	while (i > 0)
	{
		b += 2;
		i = i >> 1;
	}
	return b;
}

void Compression::PackValueLarge(u32 iVal)
{
	u32 i;
	int bitPos;

	i = iVal >> 6;
	bitPos = 0;
	while (i > 0)
	{
		i = i >> 1;
		bitPos += 1;
	}
	i = iVal >> 6;
	// MPi: TODO: This could be updated to always assume the top bit is 1? Check...
	while (bitPos > 0)
	{
		bitPos--;
		BitBufferPutBit(0);
		BitBufferPutBit((unsigned)i & (1<<bitPos));
	}
	BitBufferPutBit(1);


	i = iVal;
	bitPos = 5;
	int j;
	for (j = 0 ; j < 6; j++)
	{
		BitBufferPutBit((unsigned)i & (1<<bitPos));
		bitPos--;
	}
}

static u32 PackValueLargeLen(u32 i)
{
	u32 b = 0;
	b += 6;
	i = i >> 6;

	while (i > 0)
	{
		i = i >> 1;
		b += 2;
	}
	b++;
	return b;
}

enum NodeClassTypes
{
	kMatch = 0,
	kLiteral
};

class Node
{
public:
	Node() {}
	virtual ~Node() {}
	virtual void Write(Compression &dest) = 0;
	virtual NodeClassTypes GetType(void) = 0;
};

class Literal : public Node
{
public:
	Literal(const u8 literal) : mLiteral(literal) , mEscapeIt(true) , mNewEscape(0) {}
	virtual ~Literal() {}
	void Write(Compression &dest)
	{
		int i;
		if (mEscapeIt)
		{
			for (i=dest.mNumEscapeBits-1;i>=0;i--)
			{
				dest.BitBufferPutBit(dest.mEscape & (1<<i));
			}
			dest.BitBufferPutBit(1);

			if (dest.mChangingEscape)
			{
				dest.mEscape = mNewEscape;
				for (i=dest.mNumEscapeBits-1;i>=0;i--)
				{
					dest.BitBufferPutBit(dest.mEscape & (1<<i));
				}
			}

			if (dest.mNumEscapeBits == 0)
			{
				dest.BitBufferPutByte(mLiteral);
			}
			else
			{
				if (dest.mNumEscapeBits < 8)
				{
					for (i = 7 - dest.mNumEscapeBits ; i >= 0 ; i--)
					{
						dest.BitBufferPutBit(mLiteral & (1<<i));
					}
				}
			}
		}
		else
		{
			for (i = 7 ; i >= 0 ; i--)
			{
				dest.BitBufferPutBit(mLiteral & (1<<i));
			}
		}
	}

	NodeClassTypes GetType(void)
	{
		return kLiteral;
	}

	u8 mLiteral;
	bool mEscapeIt;
	u8 mNewEscape;
};

class Match : public Node
{
public:
	Match(u32 len , const u32 offset , const u32 lastOffset , const int historyMatch) : mLen(len) , mOffset(offset) , mLastOffset(lastOffset) , mHistoryMatch(historyMatch) {}
	virtual ~Match() {}
	void Write(Compression &dest)
	{
		if (mLen == 770 && mOffset == 1)
		{
			mLen = mLen;
		}

		int i;
		for (i=dest.mNumEscapeBits-1;i>=0;i--)
		{
			dest.BitBufferPutBit(dest.mEscape & (1<<i));
		}
		dest.BitBufferPutBit(0);

		if (mHistoryMatch != -1)
		{
			// Effectively a 0 value for the length which is a special case with history match turned on
			dest.BitBufferPutBit(0);
			dest.BitBufferPutBit(1);
			dest.BitBufferPutBit(mHistoryMatch & 8);
			dest.BitBufferPutBit(mHistoryMatch & 4);
			dest.BitBufferPutBit(mHistoryMatch & 2);
			dest.BitBufferPutBit(mHistoryMatch & 1);
			return;
		}

		// MPi: TODO: Consider making this a >= check to make the 6502 check shorter because it
		// would then only need to check the high byte.
		if (mOffset > LONG_OFFSET_THRESHOLD)
		{
			mLen--;
		}
		if (mLen >= 4)
		{
			// Escape code from below
			dest.BitBufferPutBit(0);
			dest.BitBufferPutBit(0);
			dest.PackValue(mLen - 4);
		}
		else
		{
			// This can never output 00
			dest.BitBufferPutBit(mLen > 1);
			dest.BitBufferPutBit((unsigned)mLen & 1);
		}


		if (mOffset == mLastOffset)
		{
			// Cannot happen with pack value large
			dest.BitBufferPutBit(0);
			dest.BitBufferPutBit(0);
		}
		else
		{
			dest.PackValueLarge(mOffset-1);
		}
	}

	NodeClassTypes GetType(void)
	{
		return kMatch;
	}

	u32 mLen;
	u32 mOffset;
	u32 mLastOffset;
	int mHistoryMatch;
};

void Compression::EncodeMatch(u32 len , const u32 offset)
{
	int historyMatch = -1;
	int i;
	bool found = false;
	for (i = 0 ; i < kHistoryBufferSize ; i++)
	{
		if ( (mHistoryBufferLength[i] == len) && (mHistoryBufferOffset[i] == offset) )
		{
#ifdef HISTORY_MATCH
			historyMatch = i;
			found = true;
#endif
			break;
		}
	}

	bool history;
	CodedMatchLength(len,offset,history);
	if ((historyMatch != -1) && !history)
	{
		// This causes a duplicate entry in the history table when the old method of packing the
		// bits is shorter than this new history match method
		// This reduces compression performance very slightly, the solution is to scan the history
		// buffer for duplicates before inserting entries while compressing and decompressing. Which would be slow...
		// However the gain from having this history buffer, even if slightly inefficient, is large!
		historyMatch = -1;
//		printf("Clash %d,%d\n",len,offset);
	}
	if (historyMatch == -1)
	{
		// The following line if enabled effectively does the duplicate check mentioned above. Too slow for decompression though.
//		if (!found)
		{
			mHistoryBufferLength[mHistoryBufferIndex] = len;
			mHistoryBufferOffset[mHistoryBufferIndex] = offset;
			mHistoryBufferIndex++;
			mHistoryBufferIndex = mHistoryBufferIndex & (kHistoryBufferSize-1);
		}
	}

//	printf("EM%d,%d\n",len,offset);
	Node *node = new Match(len,offset,mLastOffset,historyMatch);
	mNodes.push_back(node);

	mLastOffset = offset;
}

void Compression::EncodeLiteralRun(const u8 *litRun, u32 currentLitNum)
{
	if (currentLitNum == 0)
	{
		return;
	}
	if (currentLitNum > mLastLit)
	{
		mLastLit = currentLitNum;
	}

	do
	{
//		printf("EL%d\n",*litRun);
		Node *node = new Literal(*litRun++);
		mNodes.push_back(node);
		currentLitNum--;
	} while (currentLitNum > 0);
}


int Compression::CodedMatchLength(u32 len, u32 offset,bool &historyMatch)
{
	int b;
	if (len < 2 || (len == 2 && (offset > LONG_OFFSET_THRESHOLD)))
		return -1;

	historyMatch = false;

	b = 0;//mNumEscapeBits;

	int i;
	for (i = 0 ; i < kHistoryBufferSize ; i++)
	{
		if ( (mHistoryBufferLength[i] == len) && (mHistoryBufferOffset[i] == offset) )
		{
			historyMatch = true;
			break;
		}
	}


	len = len - 1;
	// MPi: TODO: Investigate dynamic tweaking of LONG_OFFSET_THRESHOLD because in tests it produces
	// some (a couple of bytes) saving by tweaking it higher or lower depending on the data compressed.
	if (offset > LONG_OFFSET_THRESHOLD)
	{
		// MPi: TODO: Investigate dynamic tweaking of this "--" value per file because testing
		// changing it to -=5 and the len == 2 test above to len == 5 produces about 20 bytes
		// saving on the unit test.
		len--;
	}

	if (offset == mLastOffset)
	{
		b += 1 + 2;
	}
	else
	{
		b += 1 + PackValueLargeLen(offset-1);
	}

	int historySize = (/*mNumEscapeBits +*/ 1 + 2 + 4);

	b += 2;
	// MPi: TODO: This extra b++ below is not reflected in the code stage but it does cause the
	// compression to improve in the Scroller.prg test case, find out why.
	// Might be some interaction with the history match not throwing away so many entries due
	// to the duplicate check?
	b++;
	if (len < 3)	// >= 4
	{
		if ( historyMatch && (b >= historySize))
		{
			return historySize;
		}
		if (historyMatch)
		{
			historyMatch = false;
		}

		return b;
	}
	len -= 3;

	b += PackValueLen(len);

	if ( historyMatch && (b >= historySize))
	{
		return historySize;
	}
	if (historyMatch)
	{
		historyMatch = false;
	}

	return b;
}

int Compression::Compress(const u8 *in, u32 inLen,u8 *out, u32 *outLen,int compLevel,const int escapeBits)
{
	if ((compLevel < 1) || (compLevel > 10))
		return GBA_E_INVALID_ARGUMENT;

	mLastOffset = 1;
	mLastLit = 0;
	mInLen = inLen;
	mCurBuf = in;
	mBufEnd = in + inLen;

	mBitBufferValue = 0;
	mBitBufferCount = 0;
	mBitBufferCurrentOut = 0;
	mBitBufferNextOut = out;

	mNumEscapeBits = escapeBits;

	mChoicesPos.clear();
	mTotalBitsOut = 0;

	// Now the parts which decide compression speed and level
	mUpperLimitForwardMatchLen = compLevel * 1024;
	mMaximumRangeCheck = (compLevel * 32) / 11;
	mMaximumDictionaryBlocks = (compLevel * 131072) / 11;
	mDoExtendedDictionaryAdd = false;
	mDoForwardCheck = false;
	mUseDictionary = false;

	mHistoryBufferIndex = 0;
	memset(mHistoryBufferLength,0,sizeof(mHistoryBufferLength));
	memset(mHistoryBufferOffset,0,sizeof(mHistoryBufferOffset));

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

	const u8 *litRun = mCurBuf;			// Pointer to the start of the literal run
	u32 currentLitNum = 0;			// Number of literals to encode

	u32 curPos = 0;

	mLastMatchPos = FORCE_UNSIGNED(-1);

	while (curPos < inLen)
	{
		int quickBits = -1;
		u32 proposedQuickMatchLen = 0;

		// Now try a dictionary match
		int dictionaryBits = -1;
		u32 matchLen,matchOffset;
		if (DictionaryFindMatch(curPos,matchLen,matchOffset))
		{
			bool history;
			dictionaryBits = CodedMatchLength(matchLen,matchOffset,history);
		}

		// Is length of the proposed match smaller than the number of bits needed for a literal version of the match length?
		if ((dictionaryBits != -1) && dictionaryBits < (matchLen * 9))
		{
			// Yes, so the match results in a smaller size so we use it.

			// Now do a forward match test
			u32 currentLitNum2 = currentLitNum + 1;
			u32 curPos2 = curPos+1;
			int dictionaryBits2 = -1;
			u32 matchLen2,matchOffset2;
			bool firstMatchIsBetter = true;
			// This forward match test takes some extra time
			if ((curPos2 < inLen) && mDoForwardCheck)
			{
				if (DictionaryFindMatch(curPos2,matchLen2,matchOffset2))
				{
					bool history;
					dictionaryBits2 = CodedMatchLength(matchLen2,matchOffset2,history);

					if ((dictionaryBits2 != -1) && dictionaryBits2 < (matchLen2 * 9))
					{
//						u32 forward = curPos2 - curPos;

						// Figure out if it is shorter (in terms of bits saved to the file) to skip the first match and encode using this second try instead
//						if ( (dictionaryBits + (int) ( ((forward * 9) + (matchLen2 - matchLen) * 9) / 2) ) > (dictionaryBits2 + (int)forward * 9))
//						if ( (dictionaryBits + (int) ( ((forward + matchLen2 - matchLen) * 9) / 2) ) > (dictionaryBits2 + (int)forward * 9))
//						if ( (dictionaryBits + (int) (int(forward) + int(matchLen2) - int(matchLen)) * 5) > (dictionaryBits2 + int(forward) * 9))
//						if ( (dictionaryBits + (int) (1 + int(matchLen2) - int(matchLen)) * 5) > (dictionaryBits2 + 9))
//						if ( (dictionaryBits + (int) 5 + (int(matchLen2) * 5) - (int(matchLen) * 5) ) > (dictionaryBits2 + 9))
						if ( (dictionaryBits + (int) gXPCompressionTweak1 + (int(matchLen2) * gXPCompressionTweak2) - (int(matchLen) * gXPCompressionTweak3) ) > (dictionaryBits2 + gXPCompressionTweak4))
						{
							firstMatchIsBetter = false;
						}
					}
				}
			}

			if (firstMatchIsBetter)
			{
				if (mIgnoreChoicePos.find(curPos) == mIgnoreChoicePos.end())
				{
					// Pick some interesting matches to investgate later on
//					if ((currentLitNum <= 1) && (matchLen <= 3) && (matchOffset < 192))
					{
						mChoicesPos.push_back(curPos);
					}
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
				else
				{
					if (mEnableChoicesOutput)
					{
						printf("Ignoring at %d - currentLitNum %d matchLen %d matchOffset %d\n",curPos,currentLitNum,matchLen,matchOffset);
					}
				}
			}
		}

		currentLitNum++;
		curPos++;
	}

	// Store any remaining literals
	EncodeLiteralRun(litRun,currentLitNum);

	// Optimisation of the nodes here
	u32 escapeBitMask = (1<<mNumEscapeBits)-1;
	u32 escapeBitShift = 8 - mNumEscapeBits;
	u32 totalEscapes[256];
	mEscape = 0;
	std::list<Node *>::iterator st;
	int i;

	// Find the best initial escape to use
	if (mNumEscapeBits)
	{
		memset(totalEscapes,0,sizeof(u32) * (1<<mNumEscapeBits));
		st = mNodes.begin();
		while (st != mNodes.end())
		{
			Node *node = *st++;
			// Optimise the literal escape byte for the remaining literals
			if (node->GetType() == kLiteral)
			{
				Literal *lnode = (Literal*)node;
				u8 index = (lnode->mLiteral >> escapeBitShift) & escapeBitMask;
				totalEscapes[index]++;
			}
		}

		for (i = 1 ; i < (1<<mNumEscapeBits) ; i++)
		{
			if (totalEscapes[i] < totalEscapes[mEscape])
			{
				mEscape = i;
			}
		}
	}

	// Now loop through the nodes and try to find more optimisations
	u8 origEscape = mEscape;
	st = mNodes.begin();
	mChangingEscape = false;
	bool doChangingEscapeCheck = false;
	while (st != mNodes.end())
	{
		Node *node = *st++;
		// Optimise the literal escape byte for the remaining literals
		if (node->GetType() == kLiteral)
		{
			Literal *lnode = (Literal*)node;
			if ( ((lnode->mLiteral >> escapeBitShift) & escapeBitMask) == mEscape)
			{
				lnode->mEscapeIt = true;
				if (doChangingEscapeCheck)
				{
					memset(totalEscapes,0,sizeof(u32) * (1<<mNumEscapeBits));
					u32 i;
					std::list<Node *>::iterator forward = st;
					u32 numProcessed = 0;
					while (forward != mNodes.end())
					{
						// Count the number of escape changes for each escape from after this literal
						Node *node2 = *forward++;
						if (node2->GetType() == kLiteral)
						{
							Literal *lnode2 = (Literal*)node2;
							u8 index = (lnode2->mLiteral >> escapeBitShift) & escapeBitMask;
							totalEscapes[index]++;
							numProcessed++;
						}
					}
					u8 theBestEscape = 0;
					for (i = 1 ; i < (1<<mNumEscapeBits) ; i++)
					{
						if (totalEscapes[i] < totalEscapes[theBestEscape])
						{
							theBestEscape = i;
						}
					}

					if ((numProcessed > 8) && (mEscape != theBestEscape))
					{
						mChangingEscape = true;
						mEscape = theBestEscape;
						lnode->mNewEscape = mEscape;
					}
				}
			}
			else
			{
				lnode->mEscapeIt = false;
			}
		}
	}

	// Now do the actual file write
	mEscape = origEscape;

	// MPi: TODO: These following bits don't actually *need* to be saved if we are always using
	// the optimised decompression code that assumes one bit. The code can then discard some of
	// the extra jsrs to read and discard these bits.
	// Save the compression configuration used for the start of decompression
	BitBufferPutBit(mNumEscapeBits & 2);
	BitBufferPutBit(mNumEscapeBits & 1);
	if (mNumEscapeBits > 0)
	{
		BitBufferPutBit(mChangingEscape);
		for (i=mNumEscapeBits-1;i>=0;i--)
		{
			BitBufferPutBit(mEscape & (1<<i));
		}
	}

	st = mNodes.begin();
	while (st != mNodes.end())
	{
		(*st++)->Write(*this);
	}

	// EOF
	// The length will be encoded as a really large value
	for (i=mNumEscapeBits-1;i>=0;i--)
	{
		BitBufferPutBit(mEscape & (1<<i));
	}
	BitBufferPutBit(0);
	BitBufferPutBit(0);
	BitBufferPutBit(0);
	// Trim off ~two bytes from the output since we are only supporting a maximum of 64KB files
	PackValue(FORCE_UNSIGNED(0x100000));
//	PackValue(FORCE_UNSIGNED(0x10000000));
	BitBufferFlushBits(0);

	*outLen = (u32) (mBitBufferNextOut - out);

	// Makes sure the dictionary is freed
	DictionaryFree();

	st = mNodes.begin();
	while (st != mNodes.end())
	{
		Node *node = *st++;
		delete node;
	}
	mNodes.clear();
	return GBA_E_OK;
}

void Compression::DictionaryFree(void)
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
	mValueByPos.clear();
}

void Compression::DictionaryAdd(const u32 fromStartPos)
{
	if ((fromStartPos+1) >= mInLen)
	{
		return;
	}

	// Quicker to do a find rather than allocating the Value block then failing on the first add
	if (mValueByPos.find(fromStartPos) != mValueByPos.end())
	{
		return;
	}

#if 1
	Bytes bytes(mCurBuf[fromStartPos],mCurBuf[fromStartPos+1]);
	Value *value = new Value;
	value->mPos = fromStartPos;
	// Fill in the iterators
	value->mByBytes = mValueByBytes.insert(std::pair<Bytes,Value *>(bytes,value));
	mByAge.push_back(value);
	value->mByAge = --mByAge.end();
	std::pair<std::map<u32,Value *>::iterator,bool> ret = mValueByPos.insert(std::pair<u32,Value *>(value->mPos,value));
	assert(ret.second && "XPCompression couldn't add the pair");
	value->mByPos = ret.first;

	// Expire old entries if we get too many
	// Saves memory at the expense of potential compression block matches
	// In most optimised STL implementation x.size() uses a counter variable so it isn't slow.
	if (mValueByBytes.size() > mMaximumDictionaryBlocks)
	{
		// Get the oldest used block from the head of the list
		Value *value = mByAge.front();
		DictionaryFreeValue(value);
	}
#endif
}

void Compression::DictionaryFreeValue(Value *value)
{
	mValueByBytes.erase(value->mByBytes);
	mByAge.erase(value->mByAge);
	mValueByPos.erase(value->mByPos);
	delete value;
}

bool Compression::DictionaryFindMatch(const u32 fromStartPos,u32 &len,u32 &matchOffset)
{
	u32 bestMatchPos = fromStartPos,bestMatchLen = 0;
	u32 maxNumBytesAhead = mInLen - fromStartPos;
	int bestBitLength = 0;

	// Have we searched for this already? If yes, early out.
	if (mLastMatchPos == fromStartPos)
	{
		len = mLastMatchLen;
		matchOffset = mLastMatchOffset;
		return mLastMatchReturn;
	}

	mLastMatchPos = fromStartPos;
	mLastMatchLen = 0;
	mLastMatchOffset = 0;
	mLastMatchReturn = false;

	DictionaryAdd(fromStartPos);

	if (fromStartPos == 0)
	{
		return false;
	}

#if 1
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

		bool history;
		int proposedBits = CodedMatchLength(proposedMatchLen,fromStartPos - pos,history);
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
#endif

#if 1
	if (mUseDictionary && (maxNumBytesAhead >= 2) && (bestMatchLen < maxNumBytesAhead))
	{
		// Search the current dictionary for byte sequences that might match what we want
		Bytes bytes(mCurBuf[fromStartPos],mCurBuf[fromStartPos+1]);
		std::multimap<Bytes,Value *,ByBytes>::iterator st = mValueByBytes.lower_bound(bytes);
		std::multimap<Bytes,Value *,ByBytes>::iterator en = mValueByBytes.upper_bound(bytes);
//		printf(".\n");
		// MPi: TODO: Check how often we get collisions with the typical data and optimise for needed
		while (st != en)
		{
//			printf(".");
			// Check the byte sequences that are found
			Value *value = (*st++).second;
			u32 pos = value->mPos;
			// Stops us from comparing with ourselves
			if ((fromStartPos - pos) < mMaximumRangeCheck)
			{
				continue;
			}

			// Logic begins here
			const u8 *start = mCurBuf + fromStartPos;
			const u8 *test = mCurBuf + pos;
			bool used = false;
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
				continue;
			}

			bool history;
			int proposedBits = CodedMatchLength(proposedMatchLen,fromStartPos - pos,history);
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
				used = true;
			}

			if (used)
			{
				// Because this entry is used it is put back at the end of the list again
				mByAge.erase(value->mByAge);
				mByAge.push_back(value);
				value->mByAge = --mByAge.end();
			}

		}
	}
#endif

	if (bestMatchLen == 0)
	{
		return false;
	}

	// Do the forward match test with whatever the match is and calculate a new length based on how many times the sequence repeats
	u32 to = mInLen - (fromStartPos + bestMatchLen);
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
	mLastMatchReturn = true;

	return true;
}
