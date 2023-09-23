#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DecompressU.h"
#include "CompressUInternals.h"

static inline int DecompressionU_readBit(const unsigned char** ppInBlock, const unsigned char* pDataEnd, int* currentBitMask, unsigned char* bits)
{
	int nBit;
	const unsigned char* pInBlock = *ppInBlock;

	if ((*currentBitMask) == 0)
	{
		if (pInBlock >= pDataEnd) return -1;
		(*bits) = *pInBlock++;
		(*currentBitMask) = 128;
	}

	nBit = ((*bits) & 128) ? 1 : 0;

	(*bits) <<= 1;
	(*currentBitMask) >>= 1;

	*ppInBlock = pInBlock;
	return nBit;
}

static inline int DecompressionU_readPackedValue(const unsigned char** ppInBlock, const unsigned char* pDataEnd, const int initialValue, int* currentBitMask, unsigned char* bits)
{
	int nValue = initialValue;

	while (!DecompressionU_readBit(ppInBlock, pDataEnd, currentBitMask, bits))
	{
		nValue = (nValue << 1) | DecompressionU_readBit(ppInBlock, pDataEnd, currentBitMask, bits);
	}

	return nValue;
}

static inline int DecompressionU_readPackedValuePrefix(const unsigned char** ppInBlock, const unsigned char* pDataEnd, const int initialValue, int* currentBitMask, unsigned char* bits, unsigned int firstBit)
{
	int nValue = initialValue;

	if (!firstBit)
	{
		nValue = (nValue << 1) | DecompressionU_readBit(ppInBlock, pDataEnd, currentBitMask, bits);
		while (!DecompressionU_readBit(ppInBlock, pDataEnd, currentBitMask, bits))
		{
			nValue = (nValue << 1) | DecompressionU_readBit(ppInBlock, pDataEnd, currentBitMask, bits);
		}
	}

	return nValue;
}

static size_t DecompressionU_decompress(const unsigned char* pInputData, unsigned char* pOutData, size_t inputSize, size_t maxOutBufferSize)
{
	const unsigned char* pInputDataEnd = pInputData + inputSize;
	unsigned char* pCurOutData = pOutData;
	const unsigned char* pOutDataEnd = pCurOutData + maxOutBufferSize;
	int currentBitMask = 0;
	unsigned char bits = 0;
	int nMatchOffset = 1;
	int nIsFirstCommand = 1;

	if (pInputData >= pInputDataEnd && pCurOutData < pOutDataEnd)
		return -1;

	while (1)
	{
		unsigned int nIsMatchWithOffset;

		if (nIsFirstCommand)
		{
			// The first command is always literals
			nIsFirstCommand = 0;
			nIsMatchWithOffset = 0;
		}
		else
		{
			// Read match with offset / literals bit
			nIsMatchWithOffset = DecompressionU_readBit(&pInputData, pInputDataEnd, &currentBitMask, &bits);
			if (nIsMatchWithOffset == -1)
				return -1;
		}

		if (nIsMatchWithOffset == 0)
		{
			unsigned int numLiterals = DecompressionU_readPackedValue(&pInputData, pInputDataEnd, 1, &currentBitMask, &bits);

			// Copy literals
			if ((pInputData + numLiterals) <= pInputDataEnd && (pCurOutData + numLiterals) <= pOutDataEnd)
			{
				memcpy(pCurOutData, pInputData, numLiterals);
				pInputData += numLiterals;
				pCurOutData += numLiterals;
			}
			else
			{
				return -1;
			}

			// Read match with offset / rep match bit
			nIsMatchWithOffset = DecompressionU_readBit(&pInputData, pInputDataEnd, &currentBitMask, &bits);
			if (nIsMatchWithOffset == -1)
			{
				return -1;
			}
		}

		unsigned int nMatchLen;

		if (nIsMatchWithOffset)
		{
			// Match with offset
			unsigned int nMatchOffsetHighByte = DecompressionU_readPackedValue(&pInputData, pInputDataEnd, 1, &currentBitMask, &bits);

			if (nMatchOffsetHighByte == COMPU_EOD)
			{
				break;
			}
			if (nMatchOffsetHighByte > COMPU_EOD)
			{
				return -1;
			}

			nMatchOffsetHighByte--;

			if (pInputData >= pInputDataEnd)
			{
				return -1;
			}

			unsigned int nMatchOffsetLowByte = (unsigned int)(*pInputData++);
			nMatchOffset = (nMatchOffsetHighByte << 7) | (127 - (nMatchOffsetLowByte >> 1));
			nMatchOffset++;

			nMatchLen = DecompressionU_readPackedValuePrefix(&pInputData, pInputDataEnd, 1, &currentBitMask, &bits, nMatchOffsetLowByte & 1);

			nMatchLen += (2 - 1);
		}
		else
		{
			// Rep-match
			nMatchLen = DecompressionU_readPackedValue(&pInputData, pInputDataEnd, 1, &currentBitMask, &bits);
		}

		// Copy matched bytes
		const unsigned char* pSrc = pCurOutData - nMatchOffset;
		if (pSrc >= pOutData)
		{
			if ((pSrc + nMatchLen) <= pOutDataEnd)
			{
				if ((pCurOutData + nMatchLen) <= pOutDataEnd)
				{
//					// Note: Debug
//					printf("nMatchOffset $%x nMatchLen $%x\n" , nMatchOffset , nMatchLen);
					while (nMatchLen)
					{
//						// Note: Debug
//						if ( (pCurOutData - pOutData) == 0xac45 - 0x400 )
//						{
//							int i = 0;
//						}
						*pCurOutData++ = *pSrc++;
						nMatchLen--;
					}
				}
				else
				{
					return -1;
				}
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}

	return (size_t)(pCurOutData - pOutData);
}


int DecompressU( const u8 * source, u32 sourceLen ,u8 * dest, u32 * destLen )
{
	size_t ret = DecompressionU_decompress(source , dest , sourceLen , 0x10000);

	if (ret == (size_t) -1)
	{
		return GBA_E_ERROR;
	}

	*destLen = (u32) ret;

	return GBA_E_OK;
}
