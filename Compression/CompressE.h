/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
#ifndef __GBACOMPRESSE_H__
#define __GBACOMPRESSE_H__

#include "common.h"
#include <map>
#include <list>
#include <set>

namespace RNReplicaNet
{

namespace RNXPCompression
{

/**
 * Implements the LZMPiE compression algorithm. The algorithm inserts previous data that did not match into a std::multimap of value pairs and scans these pairs for future matches.
 * The longest matching runs are then encoded with offset and length which are themselves encoded using packed values.
 */
class CompressionE
{
public:
	CompressionE();
	virtual ~CompressionE();

	/**
	 * Compress a part of memory to a destination buffer
	 * \param src the source memory pointer
	 * \param src_len the length of the source in bytes
	 * \param dst the destination memory pointer. This should be at least sourcesize + (sourcesize/8) + 256 in size
	 * \param dst_len the pointer for the length of the destination buffer. 
	 * \param level the compression level from 1 to 10 where 10 is the best compression
	 * \return An error code or 0 for no error
	 */
	int Compress( const u8*src, u32 src_len,u8 *dst, u32 *dst_len,int level);

	std::vector<u32> mChoicesPos;
	std::set<u32> mIgnoreChoicePos;
	int mTotalBitsOut;

private:
	// Bit buffering
	void BitBufferWriteBits(void);

	void BitBufferPutBit(unsigned char bit);

	void BitBufferPutByte(unsigned char b);

	void BitBufferFlushBits(unsigned char filler_bit);

	// Uses a bit per bit
	void PackValue(u32 i);
	// Uses power of two scheme
	void PackValue2(u32 i);

	void EncodeMatch(u32 len, const u32 offset);

	int LiteralRunLength(u32 currentLitNum);
	void EncodeLiteralRun(const u8 *litRun, u32 currentLitNum);

	// Calculates the proposed match length in bits
	int CodedMatchLength(u32 len, u32 offset);

	u32 mLastOffset;
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

		u16 GetHash(void) const
		{
			return ((u16)mBytes[0]) | (((u16)mBytes[1])<<8);
		}
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
	struct Value
	{
		u32 mPos;

		// Iterators
		std::multimap<Bytes,Value*,ByBytes>::iterator mByBytes;
		std::list<Value *>::iterator mByAge;
		std::set<u32>::iterator mByPos;

		std::set<u32>::iterator mHashPosAdded;
		std::list<Value*>::iterator mHashValues;

		u16 mHashedValue;
	};
	struct HashNode
	{
		std::set<u32> mPosAdded;
		std::list<Value*> mValues;
	};
	std::multimap<Bytes,Value*,ByBytes> mValueByBytes;
	std::list<Value*> mByAge;
	std::set<u32> mByPos;
	HashNode *mHashTable;

	u32 mLastMatchPos;
	u32 mLastMatchLen;
	u32 mLastMatchOffset;
	Value *mLastMatchValue;
	bool mLastMatchReturn;
	u32 mUpperLimitForwardMatchLen;
	u32 mMaximumRangeCheck;
	bool mUseDictionary;
	bool mDoForwardCheck;
	bool mDoExtendedDictionaryAdd;
	u32 mMaximumDictionaryBlocks;

	void DictionaryFree(void);
	void DictionaryAdd(const u32 fromStartPos);
	bool DictionaryFindMatch(const u32 fromStartPos,u32 &len,u32 &matchOffset,Value **pValue);
	void DictionaryFreeValue(Value *value);
	bool CommonValueChecks(Value *value,const u32 fromStartPos, u32 &bestMatchPos, u32 &bestMatchLen, int &bestBitLength, const u32 maxNumBytesAhead);

	u32 mTotalBits;
};

} // namespace RNXPCompression

} // namespace RNReplicaNet

#endif
