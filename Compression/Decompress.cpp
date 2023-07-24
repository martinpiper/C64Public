#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Decompress.h"

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


int Decompress( const u8 * source, u32  sourceLen,u8 * dest, u32 * destLen )
{
	u8 currentBits = 0;
	u32 curLen = 0, outLen = 0, lastworkOffset = 1;
	u8 bitCnt = 0;
	u8 numEscapeBits = 0;
	u8 escapeMask = 0;
	bool changingEscape = false;
	u8 theEscape = 0;
	u32 historyBufferLen[16];
	u32 historyBufferOffset[16];
	u8 historyIndex = 0;

	numEscapeBits = MGetNextBit();
	numEscapeBits = (numEscapeBits<<1) + MGetNextBit();

	if (numEscapeBits)
	{
		changingEscape = MGetNextBit()?true:false;
		int i;
		theEscape = 0;
		for (i=0;i<numEscapeBits;i++)
		{
			theEscape = (theEscape<<1) + MGetNextBit();
		}
		escapeMask = (1<<numEscapeBits)-1;
	}


	// First bit (source[0] & 0x80) encoded should always be 1 to indicate a literal byte because obviously we have no previous literals to copy from since we have no uncompressed data.
	// Future expansion could encode a 0 bit and use that as a flag for something.
	for (;;)
	{
//		printf("%8x    %8x\n",outLen,curLen);
//		if (outLen >= 0x170 || curLen >= 0x7b)
//		{
//			printf("Woah!\n");
//			exit(-1);
//		}
		// Get n escape bits
		u8 potentialLiteral = 0;
		bool isEscaped = true;
		if (numEscapeBits)
		{
			int i;
			for (i=0;i<numEscapeBits;i++)
			{
				potentialLiteral = (potentialLiteral<<1) + MGetNextBit();
			}
			if ((potentialLiteral & escapeMask) != theEscape)
			{
				isEscaped = false;
			}
		}
		if (isEscaped)
		{
			if (MGetNextBit())
			{
				// If escapes are flagged as changing then read in the new escape
				if (changingEscape)
				{
					int i;
					theEscape = 0;
					for (i=0;i<numEscapeBits;i++)
					{
						theEscape = (theEscape<<1) + MGetNextBit();
					}
				}

				if (numEscapeBits)
				{
					int i;
					for (i=0;i<8 - numEscapeBits;i++)
					{
						potentialLiteral = (potentialLiteral<<1) + MGetNextBit();
					}
					dest[outLen++] = potentialLiteral;
//printf("GLe%d\n",potentialLiteral);
					continue;
				}
				else
				{
					dest[outLen++] = source[curLen++];
//printf("GL%d\n",dest[outLen-1]);
					continue;
				}
			}
		}
		else
		{
			int i;
			for (i=0;i<8 - numEscapeBits;i++)
			{
				potentialLiteral = (potentialLiteral<<1) + MGetNextBit();
			}
			dest[outLen++] = potentialLiteral;
//printf("GL%d\n",potentialLiteral);
			continue;
		}

		// The dictionary match is processed here
		u32 workLen = MGetNextBit();
		workLen = (workLen<<1) + MGetNextBit();

		u32 workOffset = 0;
		// History match check
		if (workLen == 1)
		{
			u8 matchIndex = 0;
			int i;
			for (i=0;i<4;i++)
			{
				matchIndex = (matchIndex<<1) + MGetNextBit();
			}
			workLen = historyBufferLen[matchIndex];
			workOffset = historyBufferOffset[matchIndex];
//printf("Read history %d : %d,%d\n",historyIndex,historyBufferLen[matchIndex],historyBufferOffset[matchIndex]);
		}
		else if (workLen == 0)
		{
			// Escape code for the special case
			workLen = 1;
			do
			{
				workLen = (workLen<<1) + MGetNextBit();
			} while (!MGetNextBit());
			workLen += 1;
		}
		else
		{
			workLen--;
		}

//		if (workLen == 0x10000003)
		// This shorter escape code is used because we only support files up to 64K and to
		// save ~two bytes from the end of the file.
		if (workLen == 0x100003)
		{
			break;
		}

		// Decode workOffset if it hasn't been updated by the above history buffer match
		if (workOffset == 0)
		{
			u32 b1,b2;
			b1 = MGetNextBit();
			b2 = MGetNextBit();
			if (!b1 && !b2)
			{
				// Special case
				workOffset = lastworkOffset;
			}
			else
			{
				workOffset = b2;
				if (!b1)
				{
					while (!MGetNextBit())
					{
						workOffset = (workOffset << 1) + MGetNextBit();
					}
					workOffset = (workOffset << 1) + MGetNextBit();
					// Then get the next bits
				}
				int i;
				for (i=0;i<5;i++)
				{
					workOffset = (workOffset << 1) + MGetNextBit();
				}
				workOffset++;

			}

			// Update the history buffer
			historyBufferLen[historyIndex] = workLen;
			historyBufferOffset[historyIndex] = workOffset;
//printf("Store history %d : %d,%d\n",historyIndex,historyBufferLen[historyIndex],historyBufferOffset[historyIndex]);
			historyIndex++;
			historyIndex = historyIndex & 15;
		}

		lastworkOffset = workOffset;

		if (workOffset > LONG_OFFSET_THRESHOLD)
		{
			workLen++;
		}

//printf("GM%d,%d\n",workLen+1,workOffset);

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
