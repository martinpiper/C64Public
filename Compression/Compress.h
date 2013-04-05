#ifndef _COMPRESS_H_
#define _COMPRESS_H_

#include "common.h"
#include <map>
#include <list>
#include <vector>
#include <set>

const int kHistoryBufferSize = 16;
class Node;

/**
 * Implements the LZMPi compression algorithm. The algorithm inserts previous data that did not match into a std::multimap of value pairs and scans these pairs for future matches.
 * The longest matching runs are then encoded with offset and length which are themselves encoded using packed values.
 */
class Compression
{
public:
	Compression();
	virtual ~Compression();

	/**
	 * Compress a part of memory to a destination buffer
	 * \param src the source memory pointer
	 * \param src_len the length of the source in bytes
	 * \param dst the destination memory pointer. This should be at least sourcesize + (sourcesize/8) + 256 in size
	 * \param dst_len the pointer for the length of the destination buffer. 
	 * \param level the compression level from 1 to 10 where 10 is the best compression
	 * \param escapeBits A tweak value to allow the decompression to optimised for space or speed.
	 * \return An error code or 0 for no error
	 */
	int Compress( const u8*src, u32 src_len,u8 *dst, u32 *dst_len,int level,const int escapeBits);

	void BitBufferPutBit(u32 bit);

	void BitBufferPutByte(unsigned char b);

	// Encode values, literals or matches
	void PackValue(u32 i);

	void PackValueLarge(u32 i);

	int mNumEscapeBits;
	u8 mEscape;
	bool mChangingEscape;

	std::vector<u32> mChoicesPos;
	std::set<u32> mIgnoreChoicePos;
	u32 mTotalBitsOut;
	bool mEnableChoicesOutput;

private:
	// Bit buffering
	void BitBufferWriteBits(void);

	void BitBufferFlushBits(unsigned char filler_bit);

	void EncodeMatch(u32 len, const u32 offset);

	void EncodeLiteralRun(const u8 *litRun, u32 currentLitNum);

	// Calculates the proposed match length in bits
	int CodedMatchLength(u32 len, u32 offset,bool &historyMatch);

	u32 mLastOffset;
	u32 mLastLit;
	u32 mInLen;
	const u8 *mCurBuf;
	const u8 *mBufEnd;

	u32 mBitBufferValue;
	unsigned char mBitBufferCount;
	u8 *mBitBufferCurrentOut;
	u8 *mBitBufferNextOut;


	// Dictionary
	struct Bytes
	{
		Bytes(u8 a, u8 b)
		{
			mBytes[0] = a;
			mBytes[1] = b;
		}
		u8 mBytes[2];
	};
	struct ByBytes
	{
		bool operator()(const Bytes &a, const Bytes &b) const
		{
			if (a.mBytes[0] < b.mBytes[0])
			{
				return true;
			}
			else if (a.mBytes[0] == b.mBytes[0])
			{
				return a.mBytes[1] < b.mBytes[1];
			}
			return false;
		}
	};
	struct Value;
	struct Value
	{
		u32 mPos;

		// Iterators
		std::multimap<Bytes,Value*,ByBytes>::iterator mByBytes;
		std::list<Value *>::iterator mByAge;
		std::map<u32,Value*>::iterator mByPos;
	};
	std::multimap<Bytes,Value*,ByBytes> mValueByBytes;
	std::list<Value*> mByAge;
	std::map<u32,Value*> mValueByPos;

	u32 mLastMatchPos;
	u32 mLastMatchLen;
	u32 mLastMatchOffset;
	bool mLastMatchReturn;
	u32 mUpperLimitForwardMatchLen;
	u32 mMaximumRangeCheck;
	bool mUseDictionary;
	bool mDoForwardCheck;
	bool mDoExtendedDictionaryAdd;
	u32 mMaximumDictionaryBlocks;

	void DictionaryFree(void);
	void DictionaryAdd(const u32 fromStartPos);
	bool DictionaryFindMatch(const u32 fromStartPos,u32 &len,u32 &matchOffset);
	void DictionaryFreeValue(Value *value);

	u32 mHistoryBufferOffset[kHistoryBufferSize];
	u32 mHistoryBufferLength[kHistoryBufferSize];
	u32 mHistoryBufferIndex;

	std::list<Node *> mNodes;
};

#endif
