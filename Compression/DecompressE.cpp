#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DecompressE.h"

static inline u32 GetNextBit(u8 &currentBits, u8 &bitCnt, const u8 * source, u32 &curLen)
{
	if (bitCnt)
	{
		// If there are bits left then shift up by one
		currentBits = currentBits << 1;
		bitCnt--;
	}
	else
	{
		currentBits = (u32)source[curLen];
		curLen++;
		bitCnt=7;
	}

	return (u32) ((currentBits >> 7) & 1);
}

#define MGetNextBit() GetNextBit(currentBits,bitCnt,source,curLen)

static inline u8 GetNextByte(const u8 * source, u32 &curLen)
{
	u8 ret = source[curLen];
	curLen++;

	return ret;
}

#define MGetNextByte() GetNextByte(source,curLen)


int DecompressE( const u8 * source, u32  sourceLen,u8 * dest, u32 * destLen )
{
	u8 currentBits = 0;
	u32 curLen = 0, outLen = 0, lastworkOffset = 1;
	u8 bitCnt = 0;

	for (;;)
	{
		u32 theBit = MGetNextBit();
		if (theBit)
		{
			dest[outLen++] = MGetNextByte();
			continue;
		}

		// The dictionary match is processed here
		u32 workOffset = 1;

		while (true)
		{
			theBit = MGetNextBit();

			workOffset = (workOffset<<1) | theBit;
			// While the 6502 code does escape early (by noticing the high bit carry shift), this code does not escape early since the input buffer pointer (curLen) is used for validation
//			if (workOffset >= 0x100000)
//			{
//				break;
//			}

			theBit = MGetNextBit();
			if (theBit)
			{
				break;
			}
		}

		if (workOffset >= 0x100000)
		{
			break;
		}

		if (workOffset == 2)
		{
			workOffset = lastworkOffset;
		}
		else
		{
			workOffset -= 3;
			workOffset = (workOffset << 8) | MGetNextByte();
			workOffset++;
			lastworkOffset = workOffset;
		}

		u32 workLen = MGetNextBit();
		workLen = (workLen<<1) | MGetNextBit();

		if (workLen == 0)
		{
			workLen = 1;

			while (true)
			{
				workLen = (workLen << 1) | MGetNextBit();

				theBit = MGetNextBit();
				if (theBit)
				{
					break;
				}
			}

			workLen += 2;
		}

		if (workOffset > LONG_OFFSET_THRESHOLD)
		{
			workLen++;
		}

		const u8 *workPos = dest + outLen - workOffset;
		if (workPos < dest)
		{
			printf("Serious workPos error\n");
			return GBA_E_ERROR;
		}
		dest[outLen] = *workPos;
		outLen++;
		workPos++;
		do
		{
			dest[outLen] = *workPos;
			outLen++;
			workPos++;
			workLen--;
		} while (workLen > 0);
	}
	*destLen = outLen;
	return curLen == sourceLen ? (GBA_E_OK) : (GBA_E_ERROR);
}
