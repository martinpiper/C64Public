//..\Data\frm*05d.sch 0 1 1 12 2 $1000 $1ff8 ..\Data\frm*05d.del
// In C:\Work\C64\AnimationBitmap
// When using: ..\Animation\DeltaCompression\Release\DeltaCompression.exe
// -bitmap Data\frm*05d.sch 0 0 1 149 2 $4000 $7c00 Data\frm*05d.del
// -maxframesize 6000 -emptyframebuffer -bitmap Data\frm*05d.sch 0 902 1 903 2 $4000 $7c00 Data\frm*05d.del
// -maxframesize 6000 -emptyframebuffer -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $800 Data\frm*05d.spr -fragment $c00 Data\frm*05d.sno -fragment $1c00 Data\frm*05d.chr 902 1 903 2 $4000 $7c00 Data\frm*05d.del 32
// Produce dictionary:
// -savedictionary $f00 Data\dictionary1.bin -savedictionary $f00 Data\dictionary2.bin -maxframesize 6000 -emptyframebuffer -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $800 Data\frm*05d.spr -fragment $c00 Data\frm*05d.sno -fragment $1c00 Data\frm*05d.chr 2 1 901 2 $4000 $7c00 Data\frm*05d.del 32
// Use dictionary:
// -maxframesize 6000 -emptyframebuffer -bitmap -fragment f$d00 Data\Dictionary1.bin -fragment f$4d00 Data\Dictionary2.bin -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $800 Data\frm*05d.spr -fragment $c00 Data\frm*05d.sno -fragment $1c00 Data\frm*05d.chr 2 1 901 2 $4000 $7c00 Data\frm*05d.del 32
// -savedictionary $f00 Data\dictionary1.bin -savedictionary $f00 Data\dictionary2.bin -maxframesize 16000 -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $800 Data\frm*05d.spr -fragment $c00 Data\frm*05d.sno -fragment $1c00 Data\frm*05d.chr 0 1 289 2 $4000 $7c00 Data\frm*05d.del 32

// del /q Data\*.del && cls && ..\Animation\DeltaCompression\Release\DeltaCompression.exe -savedictionary $f00 Data\dictionary1_1.bin -savedictionary $f00 Data\dictionary1_2.bin -maxframesize 6000 -emptyframebuffer -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $800 Data\frm*05d.spr -fragment $c00 Data\frm*05d.sno -fragment $1c00 Data\frm*05d.chr   2 1 151 2 $4000 $7c00 Data\frm*05d.del 32
// cls && ..\Animation\DeltaCompression\Release\DeltaCompression.exe -maxframesize 6000 -emptyframebuffer -bitmap -fragment f$d00 Data\Dictionary1_1.bin -fragment f$4d00 Data\Dictionary1_2.bin -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $800 Data\frm*05d.spr -fragment $c00 Data\frm*05d.sno -fragment $1c00 Data\frm*05d.chr   2 1 151 2 $4000 $7c00 Data\frm*05d.del 32

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <assert.h>
#include <set>
#include <map>
#include <list>
#include "RNPlatform/Inc/MessageHelper.h"
#include "../../Common/ParamToNum.h"

using namespace RNReplicaNet;

#define ENABLE_DICTIONARY

static bool sDebugSpew_Encoding = false;

// Enough for 2 x (screen, colour, char data)
unsigned char sCurrentData[0x10000];
unsigned char sNewData[0x10000];
unsigned char sWorstCaseDelta[0x10000 + (0x21 * 2) + 2];
unsigned char sTestDecomp[0x10000];
static bool sBitmapMode = false;
std::list<std::pair<int,int>> skipPosEnd;
std::list<std::pair<int,int>> exclusionsPosEnd;

bool sDataMapUsedForCopy[0x10000];

struct pDynamicMessageHelperCompare
{
	bool operator() (const DynamicMessageHelper *lhs, const DynamicMessageHelper *rhs) const
	{
		const unsigned char *leftBuffer = (const unsigned char *) lhs->GetBuffer();
		const unsigned char *rightBuffer = (const unsigned char *) rhs->GetBuffer();

		int len = std::min(lhs->GetSize() , rhs->GetSize());

		for (int i = 0 ; i < len ; i++)
		{
			if (leftBuffer[i] < rightBuffer[i])
			{
				return true;
			}
			if (leftBuffer[i] > rightBuffer[i])
			{
				return false;
			}
		}

		if (lhs->GetSize() < rhs->GetSize())
		{
			return true;
		}
		
		return false;
	}
};

static std::map<DynamicMessageHelper* , int , pDynamicMessageHelperCompare> allBlocks;
static std::map<DynamicMessageHelper* , int , pDynamicMessageHelperCompare> blocksThisFrame;
// This is only enabled when blocks are going to be added for the last calculated delta
static bool addBlocksMode = false;
static void addBlockInternal(const unsigned char *memory , int len)
{
#ifdef ENABLE_DICTIONARY
	if (len < 3)
	{
		return;
	}

	DynamicMessageHelper *block = new DynamicMessageHelper(memory , len);
	std::pair<std::map<DynamicMessageHelper* , int , pDynamicMessageHelperCompare>::iterator,bool> it = blocksThisFrame.insert(std::pair<DynamicMessageHelper*,int>(block,1));
	if (!it.second)
	{
		it.first->second = it.first->second + 1;

		DynamicMessageHelper *temp = it.first->first;
		assert(temp->GetSize() == block->GetSize());
		assert(memcmp(temp->GetBuffer() , block->GetBuffer() , block->GetSize()) == 0);

		delete block;
	}
#endif
}

const int sMaxBlockLen = 254;
static void addBlock(const unsigned char *memory , int len)
{
#ifdef ENABLE_DICTIONARY
	if (!addBlocksMode)
	{
		return;
	}

	while (len >= sMaxBlockLen)
	{
		addBlockInternal(memory , sMaxBlockLen);
		memory += sMaxBlockLen;
		len -= sMaxBlockLen;
	}
	addBlockInternal(memory , len);
#endif
}


static void clearBlocks()
{
	std::map<DynamicMessageHelper* , int , pDynamicMessageHelperCompare>::iterator st = blocksThisFrame.begin();
	while (st != blocksThisFrame.end())
	{
		delete st->first;
		st++;
	}
	blocksThisFrame.clear();
}

// Only add valid blocks from this frame into the all blocks list
static void processBlocks()
{
	std::map<DynamicMessageHelper* , int , pDynamicMessageHelperCompare>::iterator st = blocksThisFrame.begin();
	while (st != blocksThisFrame.end())
	{
		DynamicMessageHelper *block = st->first;

#if 1
		// Check for blocks being sub-blocks
		std::map<DynamicMessageHelper* , int , pDynamicMessageHelperCompare>::iterator it2 = allBlocks.begin();
		while (it2 != allBlocks.end())
		{
			DynamicMessageHelper *fromAllBlocks = it2->first;
			if (block->GetSize() < fromAllBlocks->GetSize())
			{
				if (memcmp(block->GetBuffer() , fromAllBlocks->GetBuffer() , block->GetSize()) == 0)
				{
					st->second++;
					delete block;
					block = 0;
					break;
				}
			}

			if (fromAllBlocks->GetSize() < block->GetSize())
			{
				if (memcmp(fromAllBlocks->GetBuffer() , block->GetBuffer() , fromAllBlocks->GetSize()) == 0)
				{
					allBlocks.insert(std::pair<DynamicMessageHelper* , int>(block,st->second + it2->second));
					allBlocks.erase(it2);
					delete fromAllBlocks;
					block = 0;
					break;
				}
			}

			it2++;
		}

		if (!block)
		{
			st++;
			continue;
		}
#endif

		std::pair<std::map<DynamicMessageHelper* , int , pDynamicMessageHelperCompare>::iterator,bool> it = allBlocks.insert(std::pair<DynamicMessageHelper* , int>(block,1));
		if (!it.second)
		{
			assert(it.first->first->GetSize() == block->GetSize());
			assert(memcmp(it.first->first->GetBuffer() , block->GetBuffer() , block->GetSize()) == 0);
			delete block;

			it.first->second = it.first->second + st->second;
		}

		st++;
	}
	blocksThisFrame.clear();
}


enum
{
	kOpCodeEnd	= 252,
	kOpCodeCopy	= 253,
	kOpCodeRLE	= 254,
	kOpCodeData	= 255
};

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef signed int s32;
typedef signed short s16;
typedef signed char s8;

const int kKeyLength = 3;
static int makeKey(const unsigned char *memory)
{
	return memory[0] | ((int)memory[1])<<8 | ((int)memory[2])<<8;
}

static void updateLiterals(int firstPos , int currentPos, const unsigned char * delta)
{
	if (firstPos != -1 && currentPos != -1)
	{
		addBlock(delta + firstPos , currentPos - firstPos);
	}
}

static int isPosCached(const int pos)
{
	std::list<std::pair<int,int>>::iterator st = skipPosEnd.begin();
	while (st != skipPosEnd.end())
	{
		if (pos >= st->first && pos < st->second)
		{
			//			printf("pos $%x inside $%x $%x\n" , pos , st->first , st->second);
			return st->second - pos;
		}

		st++;
	}

	return 0;
}

static int isPosExclusion(const int pos)
{
	std::list<std::pair<int,int>>::iterator st = exclusionsPosEnd.begin();
	while (st != exclusionsPosEnd.end())
	{
		if (pos >= st->first && pos < st->second)
		{
			//			printf("pos $%x inside $%x $%x\n" , pos , st->first , st->second);
			return st->second - pos;
		}

		st++;
	}
	return 0;
}

static int isPosSkippable(const int pos)
{
	int ret = isPosExclusion(pos);
	if (!ret)
	{
		return ret;
	}

	return isPosCached(pos);
}

int DeltaCompress(unsigned char *output, const unsigned char *input, const unsigned char *delta, const int inputLength , const int bestSizeVar = 4 , const int copyBlockSize = 254 , bool skipCopyFromPrimedBuffer = false)
{
	std::multimap<int,int> inputToPos , deltaToPos;

	u32 foundLeast[256];
	memset(foundLeast,0,sizeof(foundLeast));

	// Total up the occurrences of each byte value
	int i;
	for (i=0;i<inputLength;i++)
	{
		if (isPosSkippable(i))
		{
			continue;
		}
		foundLeast[delta[i]]++;
	}

	for (i=0;i<inputLength - kKeyLength;i++)
	{
		if (input)
		{
			inputToPos.insert(std::pair<int,int>(makeKey(input+i),i));
		}
		deltaToPos.insert(std::pair<int,int>(makeKey(delta+i),i));
	}

	// Find the least used byte for opcodes 128 and above, this improves decompression speed on a 6502 because it can bmi
	u8 leastIndex = 128;
	for (i=128;i<256;i++)
	{
		if (foundLeast[i] < foundLeast[leastIndex])
		{
			leastIndex = (u8) i;
		}
	}

	u8 leastIndex2 = (leastIndex+1)|128;
	for (i=128;i<256;i++)
	{
		if ((leastIndex != i) && (foundLeast[i] < foundLeast[leastIndex2]))
		{
			leastIndex2 = (u8) i;
		}
	}

	u8 leastIndex3 = (leastIndex2+1)|128;
	for (i=128;i<256;i++)
	{
		if ((leastIndex != i) && (leastIndex2 != i) && (foundLeast[i] < foundLeast[leastIndex3]))
		{
			leastIndex3 = (u8) i;
		}
	}

	int outPos = 0;
	// Save the markers/opcodes so the decompression knows what to expect
	// Output number of XX bytes. If the number of bytes is zero then exit the decompression.
	output[outPos++] = leastIndex;
	// Skip number of bytes code, if 0 follows then output the skip code byte.
	output[outPos++] = leastIndex2;
	// Copy from hi/lo offset for num bytes. If the hi offset is $ff then don't fetch the lo byte or num bytes and just output the marker/opcode once.
	output[outPos++] = leastIndex3;

	int emittedLastSkipCodesPos = -1;
	int emittedLastCopyCodesPos = -1;
	int emittedFirstLiteralPos = -1;

	// Now RLE pack with the least used byte as a marker
	for (i=0;i<inputLength;)
	{
		int size;
		int bestSize = -1;
		// Check for no difference with the previous data
		int forceSkip = isPosSkippable(i);
		if (input || forceSkip)
		{
			if (!forceSkip)
			{
				for (size = 2 ; (size < 256) && ((i+size) <= inputLength) ; size++)
				{
					if (memcmp(delta + i, input + i,size) != 0)
					{
						break;
					}
					bestSize = size;
				}
			}
			else
			{
				bestSize = forceSkip;
			}
			if (bestSize > 255)
			{
				bestSize = 255;
			}
			if (bestSize> 0)
			{
				if ( emittedLastSkipCodesPos == -1 )
				{
					emittedLastSkipCodesPos = outPos;
				}
				if (emittedFirstLiteralPos == -1)
				{
					emittedFirstLiteralPos = i;
				}

				if (sDebugSpew_Encoding)
				{
					printf("At $%04x delta $%04x : skip size $%02x           \n" , i , outPos , bestSize);
				}

				emittedLastCopyCodesPos = -1;
				output[outPos++] = leastIndex2;
				output[outPos++] = bestSize;
				i += bestSize;
				continue;
			}
		}

		bool match = false;
		if ( (i+3) < inputLength )
		{
			if ( (delta[i] == delta[i+1]) && (delta[i] == delta[i+2]))
			{
				match = true;
			}
		}
		// Check to see if we need to output a run
		if ( (delta[i] == leastIndex) || (delta[i] == leastIndex2) || (delta[i] == leastIndex3) || match)
		{
			int j = 0;
			while ((delta[i+j] == delta[i]) && (j < 255) && ((i+j) < inputLength))
			{
				j++;
			}
			// Use the byte shorter version for the other two opcodes if it is too short
			if ((delta[i] == leastIndex2) && (j == 1))
			{
				if (emittedFirstLiteralPos == -1)
				{
					emittedFirstLiteralPos = i;
				}
				emittedLastSkipCodesPos = -1;
				emittedLastCopyCodesPos = -1;
				output[outPos++] = leastIndex2;
				output[outPos++] = 0;
				i++;
				continue;
			}

			if ((delta[i] == leastIndex3) && (j == 1))
			{
				if (emittedFirstLiteralPos == -1)
				{
					emittedFirstLiteralPos = i;
				}
				emittedLastSkipCodesPos = -1;
				emittedLastCopyCodesPos = -1;
				output[outPos++] = leastIndex3;
				output[outPos++] = 0xff;
				i++;
				continue;
			}

			if (sDebugSpew_Encoding)
			{
				printf("At $%04x delta $%04x : Run size $%02x of $%02x          \n" , i , outPos , j , delta[i]);
			}

			if (emittedFirstLiteralPos == -1)
			{
				emittedFirstLiteralPos = i;
			}
			emittedLastSkipCodesPos = -1;
			emittedLastCopyCodesPos = -1;
			output[outPos++] = leastIndex;
			output[outPos++] = (u8)j;
			output[outPos++] = delta[i];
			i+=j;
			continue;
		}

#if 1
		// Avoid copying into skippable blocks
		if (!isPosSkippable(i))
		{
			// Search for the longest string to copy in the existing data,
			// if it cannot all be found then output literals for the remainder of the data
			int bestPos = -1;
			bestSize = -1;

			std::list<int> positions;
			std::multimap<int,int>::iterator st = inputToPos.lower_bound(makeKey(delta + i));
			std::multimap<int,int>::iterator en = inputToPos.upper_bound(makeKey(delta + i));
			while (st != en)
			{
				int position = (*st++).second;

				positions.push_back(position);
			}
			st = deltaToPos.lower_bound(makeKey(delta + i));
			en = deltaToPos.upper_bound(makeKey(delta + i));
			while (st != en)
			{
				int position = (*st++).second;
				positions.push_back(position);
			}

			// We can search the entire buffer since we are effectively moving data around in ourselves.
	//		for (; pos < (inputLength-10); pos++)
			std::list<int>::iterator st2 = positions.begin();
			while (positions.end() != st2)
			{
				int pos = *st2++;

				int size;
				bool earlyOut = false;
				for (size = 5 ; (size < 255) && ((pos+size) < inputLength) && ((i+size) < inputLength) && !earlyOut; size++)
				{
					// Avoid copying into skippable blocks
					if (isPosSkippable(i + size - 1))
					{
						break;
					}
					if (skipCopyFromPrimedBuffer)
					{
						if (isPosCached(pos))
						{
							break;
						}
						if (isPosCached(pos+size))
						{
							break;
						}
					}

					// Avoid copying from inside exclusion blocks
					if (isPosExclusion(pos))
					{
						break;
					}
					if (isPosExclusion(pos+size))
					{
						break;
					}

					// Avoid copying from within our own block
					if ( (pos >= i) && (pos <= (i+size)) )
					{
						continue;
					}
					if ( ((pos+size-1) >= i) && ((pos+size-1) <= (i+size)) )
					{
						break;
					}

					if ( (i >= pos) && (i <= (pos+size)) )
					{
						continue;
					}
					if ( ((i+size) >= pos) && ((i+size-1) <= (pos+size)) )
					{
						break;
					}
				
					if (sBitmapMode)
					{
						// Filter out bitmap screen data sprite pointers
						if ( ((pos & 0x3fff) >= 0x7f8) && ((pos & 0x3fff) <= 0x7ff) )
						{
							continue;
						}
						if ( (((pos+size) & 0x3fff) >= 0x7f8) && (((pos+size) & 0x3fff) <= 0x7ff) )
						{
							continue;
						}
						if ( ((pos & 0x3fff) < 0x7f8) && (((pos+size) & 0x3fff) > 0x7ff) )
						{
							continue;
						}

						if ( ((i & 0x3fff) >= 0x7f8) && ((i & 0x3fff) <= 0x7ff) )
						{
							continue;
						}
						if ( (((i+size) & 0x3fff) >= 0x7f8) && (((i+size) & 0x3fff) <= 0x7ff) )
						{
							continue;
						}
						if ( ((i & 0x3fff) < 0x7f8) && (((i+size) & 0x3fff) > 0x7ff) )
						{
							continue;
						}
					}
					else
					{
						// Also avoid copying from any data in $3f8-$3ff for the screen banks
						// This misses any multiple of sprite definitions which are being displayed after the screen memory
						if ( !(pos & 0x800) && !((pos+size-1) & 0x800))
						{
							if ( ((pos & 0x3ff) >= 0x3f8) && ((pos & 0x3ff) <= 0x3ff) )
							{
								continue;
							}
							if ( (((pos+size) & 0x3ff) >= 0x3f8) && (((pos+size) & 0x3ff) <= 0x3ff) )
							{
								continue;
							}

							if ( ((i & 0x3ff) >= 0x3f8) && ((i & 0x3ff) <= 0x3ff) )
							{
								continue;
							}
							if ( (((i+size) & 0x3ff) >= 0x3f8) && (((i+size) & 0x3ff) <= 0x3ff) )
							{
								continue;
							}

						}
					}

					// When copying check for the same data longer than copyBlockSize and check for skip code generation to optimise speed at the expense of some compression
					if (bestSize > 0 && input && size > copyBlockSize)
					{
						if (i + size + 8 < inputLength)
						{
							if (memcmp(delta + i + size, input + i + size,8) == 0)
							{
								earlyOut = true;
							}
						}
					}

					if (pos < i)
					{
						// The comparison is what we would have decompressed already, so compare the same buffer
						if (memcmp(delta + i, delta + pos,size) == 0)
						{
							if (size > bestSize)
							{
								bestSize = size;
								bestPos = pos;
							}
						}
						else
						{
							break;
						}
					}
					else
					{
						// The comparison is not what we would have decompressed already, so compare the other buffer
						if ( (pos - i) < size )
						{
							continue;
						}
						// Don't consider addresses that are too high
						if ( (i + size) >= inputLength)
						{
							break;
						}
						if ( (pos + size) >= inputLength)
						{
							break;
						}
						if (input)
						{
							if (memcmp(delta + i, input + pos,size) == 0)
							{
								if (size > bestSize)
								{
									bestSize = size;
									bestPos = pos;
								}
							}
							else
							{
								break;
							}
						}
						else
						{
							break;
						}
					}
				}
			}

			if (bestSize > bestSizeVar)
			{
				bool stillUseBlock = true;
				std::set<unsigned char> bytesUsedInBlock;
				// Determine if it would be better to store a literal and a skip, calculate entropy
				for (int j=0 ; j < bestSize ; j++)
				{
					bytesUsedInBlock.insert(delta[bestPos + j]);
				}
				if (bytesUsedInBlock.size() <= 2)
				{
					bool changes[256];
					memset(changes , 0 , sizeof(changes));

					for (int j=0 ; j < bestSize ; j++)
					{
						if (delta[i+j] != delta[bestPos + j])
						{
							changes[j] = true;
						}
					}

					int edges = 0;
					for (int j=1 ; j < bestSize ; j++)
					{
						if (changes[j] != changes[j-1])
						{
							edges++;
						}
					}

					if (edges <= 2)
					{
						stillUseBlock = false;
					}
					else
					{
						stillUseBlock = true;
					}
				}
				if (stillUseBlock)
				{
					if (sDebugSpew_Encoding)
					{
						printf("At $%04x delta $%04x : Copy from pos $%04x size $%02x cached%d          \n" , i , outPos , bestPos , bestSize , isPosCached(bestPos)?1:0);
					}

					emittedLastSkipCodesPos = -1;

					updateLiterals(emittedFirstLiteralPos , i, delta);
					emittedFirstLiteralPos = -1;

					emittedLastCopyCodesPos = outPos;
					output[outPos++] = leastIndex3;
					output[outPos++] = bestPos >> 8;
					output[outPos++] = bestPos;
					output[outPos++] = bestSize;

					addBlock(delta + bestPos , bestSize);

					if (addBlocksMode)
					{
						for (int len = 0 ; len < bestSize ; len++)
						{
							sDataMapUsedForCopy[bestPos + len] = true;
						}
					}

					i += bestSize;
					continue;
				}
			}
#endif
		}

		if (sDebugSpew_Encoding)
		{
			printf("At $%04x delta $%04x : Literal $%02x           \n" , i , outPos , delta[i]);
		}
		// Output a literal byte
		if (emittedFirstLiteralPos == -1)
		{
			emittedFirstLiteralPos = i;
		}
		emittedLastSkipCodesPos = -1;
		emittedLastCopyCodesPos = -1;
		output[outPos++] = delta[i++];
	}

	updateLiterals(emittedFirstLiteralPos , i, delta);
	emittedFirstLiteralPos = -1;

	// Remove any trailing skip codes, since they don't actually change any buffer data they can be safely removed.
	if ( emittedLastSkipCodesPos != -1 )
	{
//		printf("Trimmed %d                                             \n" , outPos - emittedLastSkipCodesPos );
		outPos = emittedLastSkipCodesPos;
	}

	output[outPos++] = leastIndex;
	output[outPos++] = 0;

	// Special case for completely empty frame
	if (outPos == 5)
	{
		output[0] = 0;
		output[1] = 0;
		outPos = 2;
	}

	return outPos;
}

void DeltaDecompress(unsigned char *out, const unsigned char *delta)
{
	unsigned char *base = out;
	const unsigned char *origDelta = delta;
	unsigned char opcode1 = *delta++;
	unsigned char opcode2 = *delta++;

	// Special case for completely empty frame
	if (opcode1 == 0 && opcode2 == 0)
	{
		return;
	}

	unsigned char opcode3 = *delta++;
	while(true)
	{
		unsigned char byte = *delta++;
		if (byte == opcode1)
		{
			unsigned char count = *delta++;
			if (count == 0)
			{
				return;
			}
			unsigned char val = *delta++;
			while (count--)
			{
				*out++ = val;
			}
			continue;
		}
		else if (byte == opcode2)
		{
			unsigned char count = *delta++;
			if (count == 0)
			{
				*out++ = opcode2;
				continue;
			}
			out += count;
			continue;
		}
#if 1
		else if (byte == opcode3)
		{
			unsigned char posHi = *delta++;
			if (posHi == 0xff)
			{
				*out++ = opcode3;
				continue;
			}
			unsigned char posLo = *delta++;
			int pos = (((int)posHi) << 8) | posLo;
			unsigned char count = *delta++;
			assert(count);
			assert((pos+count) <= sizeof(sCurrentData));
			memcpy(out,base+pos,count);
			out += count;
			continue;
		}
#endif
		*out++ = byte;
	}
}

void HandleForcePrimeData(std::list< std::pair<std::string , std::string> > &filesToLoad, unsigned char * targetData)
{
	char buffer[1024];
	// Handle first frame forced data
	std::list<std::pair<std::string,std::string>>::iterator stp = filesToLoad.begin();
	while (stp != filesToLoad.end())
	{
		DynamicMessageHelper input;
		int byteOffset = 0;
		if (stp->first.c_str()[0] == 'f')
		{
			byteOffset = ParamToNum(stp->first.c_str()+1);
			if (!input.Read(stp->second.c_str(),true))
			{
				printf("Could not read file '%s'\n",buffer);
				exit(-1);
			}

			if (input.GetBufferSize() > 0)
			{
				skipPosEnd.push_back(std::pair<int,int>(byteOffset , byteOffset + input.GetBufferSize()));
			}

			printf("Force prime with '%s' at %x len %x\n",stp->second.c_str() , byteOffset , input.GetBufferSize());
			memcpy(targetData + byteOffset,input.GetBuffer(),input.GetBufferSize());
		}
		stp++;
	}
}

int main(int argc, char **argv)
{
#ifdef _WIN32
#ifdef _DEBUG
#if _MSC_VER >= 1500
	_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_ALLOC_MEM_DF);
#endif	//< if _MSC_VER >= 1500
#endif	//< ifdef _DEBUG
#endif	//< ifdef _WIN32

	bool noFirstFrame = false;
	bool emptyFrameBuffer = false;
	int maxframesize = -1;
	std::list< std::pair<std::string , std::string> > filesToLoad;
	std::list< std::pair<std::string , std::string> > dictionaryFiles;
	std::list< std::pair<std::string , std::string> > optimiseFiles;
	char *usedMemoryFilename = 0;

	if (argc < 10)
	{
		printf("DeltaCompression : [-savedictionary <max len> <filename>] [-maxframesize <bytes>] [-nofirstframe] [-emptyframebuffer] [-bitmap] <Input file mask in name*05d.ext format> <byte offset for start> <start frame number> <frame step> <end frame number> <number of banks> <first frame size> <other frame size> <output file name mask in name*05d.ext format> [copy size test tweak (254)]\n");
		return -1;
	}

	while (argv[1][0] == '-')
	{
		if (strcmp(argv[1] , "-bitmap") == 0)
		{
			sBitmapMode = true;
		}
		else if (strcmp(argv[1] , "-nofirstframe") == 0)
		{
			noFirstFrame = true;
		}
		else if (strcmp(argv[1] , "-emptyframebuffer") == 0)
		{
			emptyFrameBuffer = true;
		}
		else if (strcmp(argv[1] , "-savedictionary") == 0)
		{
			argv++;
			argc--;
			if (argc > 2)
			{
				dictionaryFiles.push_back(std::pair<std::string,std::string>(argv[1] , argv[2]));
			}
			argv++;
			argc--;
		}
		else if (strcmp(argv[1] , "-optimisefile") == 0)
		{
			argv++;
			argc--;
			if (argc > 2)
			{
				optimiseFiles.push_back(std::pair<std::string,std::string>(argv[1] , argv[2]));
			}
			argv++;
			argc--;
		}
		else if (strcmp(argv[1] , "-excludeposend") == 0)
		{
			argv++;
			argc--;
			if (argc > 2)
			{
				int pos = ParamToNum(argv[1]);
				argv++;
				argc--;
				int end = ParamToNum(argv[1]);
				exclusionsPosEnd.push_back(std::pair<int,int>(pos,end));
			}
		}
		else if (strcmp(argv[1] , "-excludebitmaprect") == 0)
		{
			int coladdr;
			int scraddr;
			int bitmapaddr;
			int x , x2;
			int y , y2;
			argv++;
			argc--;
			if (argc > 5)
			{
				coladdr = ParamToNum(argv[1]);
				argv++;
				argc--;
				scraddr = ParamToNum(argv[1]);
				argv++;
				argc--;
				bitmapaddr = ParamToNum(argv[1]);
				argv++;
				argc--;
				x = ParamToNum(argv[1]);
				argv++;
				argc--;
				x2 = ParamToNum(argv[1]);
				argv++;
				argc--;
				y = ParamToNum(argv[1]);
				argv++;
				argc--;
				y2 = ParamToNum(argv[1]);

				for (int i = y ; i < y2 ; i++)
				{
					exclusionsPosEnd.push_back(std::pair<int,int>(coladdr + x + (i*40) , coladdr + x2 + (i*40)));
					exclusionsPosEnd.push_back(std::pair<int,int>(scraddr + x + (i*40) , scraddr + x2 + (i*40)));
					exclusionsPosEnd.push_back(std::pair<int,int>(bitmapaddr + (x*8) + (i*40*8) , bitmapaddr + (x2*8) + (i*40*8)));
				}
			}
		}
		else if (strcmp(argv[1] , "-maxframesize") == 0)
		{
			argv++;
			argc--;
			if (argc > 1)
			{
				maxframesize = ParamToNum(argv[1]);
			}
		}
		else if (strcmp(argv[1] , "-fragment") == 0)
		{
			argv++;
			argc--;
			if (argc > 2)
			{
				std::string inputNameMask = argv[2];
				std::replace<std::string::iterator,char>(inputNameMask.begin(),inputNameMask.end(),'*','%');
				filesToLoad.push_back(std::pair<std::string,std::string>(argv[1] , inputNameMask));
			}
			argv++;
			argc--;
		}

		argc--;
		argv++;
	}

	if (filesToLoad.empty())
	{
		std::string inputNameMask = argv[1];
		std::replace<std::string::iterator,char>(inputNameMask.begin(),inputNameMask.end(),'*','%');
		filesToLoad.push_back(std::pair<std::string,std::string>(argv[2] , inputNameMask));
		argv+=2;
		argc-=2;
	}

	memset(sCurrentData,0,sizeof(sCurrentData));
	memset(sNewData,0,sizeof(sNewData));
	memset(sDataMapUsedForCopy,0,sizeof(sDataMapUsedForCopy));

	int startFrame = ParamToNum(argv[1]);
	int frameStep = ParamToNum(argv[2]);
	int endFrame = ParamToNum(argv[3]);
	int numBanks = ParamToNum(argv[4]);
	int firstFrameSize = ParamToNum(argv[5]);
	int otherFrameSize = ParamToNum(argv[6]);
	std::string outputNameMask = argv[7];
	int copyBlockSize = 254;
	if (argc >= 9)
	{
		copyBlockSize = ParamToNum(argv[8]);
	}
	std::replace<std::string::iterator,char>(outputNameMask.begin(),outputNameMask.end(),'*','%');

	// Prime all the banks with what the data should look like at the end of the animation sequence.
	int bank = 0;
	int i;
	char buffer[1024];

	if (!noFirstFrame && !emptyFrameBuffer)
	{
		for (i=startFrame;i<=endFrame;i+=frameStep)
		{
			printf("Prime '%d'\n",i);
			std::list<std::pair<std::string,std::string>>::iterator st = filesToLoad.begin();
			while (st != filesToLoad.end())
			{
				DynamicMessageHelper input;
				std::string inputNameMask = st->second;
				int byteOffset = 0;
				if (st->first.c_str()[0] == 'f')
				{
					if (i != startFrame)
					{
						st++;
						continue;
					}
					byteOffset = ParamToNum(st->first.c_str()+1);
				}
				else if (st->first.c_str()[0] == '-')
				{
					byteOffset = ParamToNum(st->first.c_str()+1);
				}
				else
				{
					byteOffset = ParamToNum(st->first.c_str()) + (bank * firstFrameSize);
				}
				sprintf(buffer, inputNameMask.c_str(), i);
				if (!input.Read(buffer,true))
				{
					printf("Could not read file '%s'\n",buffer);
					exit(-1);
				}

				printf("Prime with '%s' at %x len %x\n",buffer , byteOffset , input.GetBufferSize());
				memcpy(sCurrentData + byteOffset,input.GetBuffer(),input.GetBufferSize());

				st++;
			}

			bank++;
			if ( bank >= numBanks )
			{
				bank = 0;
			}
		}

		printf("\n");
	}

	int cannotSkipDeltaWrite = 0;
	bool doneForcePrime = false;
	if (!noFirstFrame && !emptyFrameBuffer)
	{
		HandleForcePrimeData(filesToLoad, sCurrentData);
		doneForcePrime = true;
	}
	else
	{
		cannotSkipDeltaWrite = 2;
	}

	memcpy(sNewData,sCurrentData,sizeof(sCurrentData));

	// Write what the buffer should look like for the animation player to load
	sprintf(buffer, outputNameMask.c_str(), -1);
	FILE *fp = fopen(buffer,"wb");
	if (!fp)
	{
		printf("Could not write file '%s'\n",buffer);
		exit(-1);
	}
	fwrite(sCurrentData,1,otherFrameSize,fp);
	fclose(fp);


	// Now run a full compression loop
	bank = 0;
	int totalBytesIn = 0;
	int totalBytesOut = 0;
	int totalSaved = 0;
	for (i=startFrame;i<=endFrame;i+=frameStep)
	{
		bool forcedSmallerFrame = false;
		int thisBytesIn = 0;
		std::list<std::pair<std::string,std::string>>::iterator st = filesToLoad.begin();
		while (st != filesToLoad.end())
		{
			DynamicMessageHelper input;
			std::string inputNameMask = st->second;
			int byteOffset = 0;
			if (st->first.c_str()[0] == 'f')
			{
				if (i != startFrame)
				{
					st++;
					continue;
				}
				byteOffset = ParamToNum(st->first.c_str()+1);
			}
			else if (st->first.c_str()[0] == '-')
			{
				byteOffset = ParamToNum(st->first.c_str()+1);
			}
			else
			{
				byteOffset = ParamToNum(st->first.c_str()) + (bank * firstFrameSize);
			}
			sprintf(buffer, inputNameMask.c_str(), i);
			if (!input.Read(buffer,true))
			{
				printf("Could not read file '%s'\n",buffer);
				exit(-1);
			}

//			printf("Populate with '%s' at %x len %x\n",buffer , byteOffset , input.GetBufferSize());
			memcpy(sNewData + byteOffset,input.GetBuffer(),input.GetBufferSize());
			totalBytesIn += input.GetBufferSize();
			thisBytesIn += input.GetBufferSize();

			st++;
		}

		int thisFrameSize = 0;
		if (noFirstFrame)
		{
			thisFrameSize = (i==startFrame)?firstFrameSize:otherFrameSize;
		}
		else
		{
			thisFrameSize = otherFrameSize;
		}

		bool retryCompression = false;
		retryCompression = false;
		int bestSizeVar = 3;
		int bestSizeVarResult = -1;
		int bestSizeVarResultVal = -1;
		bool bestSkipCopyFromPrimedBuffer = false;
		int ret = 0;
		do
		{
			addBlocksMode = false;
			retryCompression = false;
			bestSizeVar = 3;
			bestSizeVarResult = -1;
			bestSizeVarResultVal = -1;
			ret = 0;

			// Don't use the previous buffer for the first frame
			//		int ret = DeltaCompress(sWorstCaseDelta,(i==startFrame)?0:sCurrentData,sNewData, (i==startFrame)?firstFrameSize:otherFrameSize/*(bank * 0x1000) + input.GetBufferSize() + byteOffset*/);
			// Use the previous buffer
			while (++bestSizeVar < 20)
			{
				// MPi: TODO: Obvious optimisation here to use threads to execute these best case finding attempts in parallel
				if (noFirstFrame)
				{
					ret = DeltaCompress(sWorstCaseDelta,(i==startFrame)?0:sCurrentData,sNewData, thisFrameSize , bestSizeVar , copyBlockSize , false);
				}
				else
				{
					ret = DeltaCompress(sWorstCaseDelta,sCurrentData,sNewData, otherFrameSize , bestSizeVar , copyBlockSize , false);
				}
				if ( (bestSizeVarResultVal == -1) || (ret < bestSizeVarResult) )
				{
					bestSizeVarResultVal = bestSizeVar;
					bestSizeVarResult = ret;
					bestSkipCopyFromPrimedBuffer = false;
				}

#if 0 // No saving at all
				if (noFirstFrame)
				{
					ret = DeltaCompress(sWorstCaseDelta,(i==startFrame)?0:sCurrentData,sNewData, thisFrameSize , bestSizeVar , copyBlockSize , true);
				}
				else
				{
					ret = DeltaCompress(sWorstCaseDelta,sCurrentData,sNewData, otherFrameSize , bestSizeVar , copyBlockSize , true);
				}
				if ( ret < bestSizeVarResult )
				{
					bestSizeVarResultVal = bestSizeVar;
					bestSizeVarResult = ret;
					bestSkipCopyFromPrimedBuffer = true;
				}
#endif
			}
			addBlocksMode = true;
//			sDebugSpew_Encoding = true;
			clearBlocks();
			if (noFirstFrame)
			{
				ret = DeltaCompress(sWorstCaseDelta,(i==startFrame)?0:sCurrentData,sNewData, thisFrameSize , bestSizeVarResultVal , copyBlockSize , bestSkipCopyFromPrimedBuffer);
			}
			else
			{
				ret = DeltaCompress(sWorstCaseDelta,sCurrentData,sNewData, thisFrameSize , bestSizeVarResultVal , copyBlockSize , bestSkipCopyFromPrimedBuffer);
			}
			sDebugSpew_Encoding = false;
//			printf("Frame %d in %d comp %d bestSizeVarResultVal %d bestSkipCopyFromPrimedBuffer %d                       \n", i , thisBytesIn, ret , bestSizeVarResultVal , bestSkipCopyFromPrimedBuffer);
			printf("Frame %d in %d comp %d bestSizeVarResultVal %d                        \n", i , thisBytesIn, ret , bestSizeVarResultVal);
			if (maxframesize > 0)
			{
				if (ret > maxframesize)
				{
					if (thisFrameSize > 512)
					{
						thisFrameSize -= 256;
						retryCompression = true;
						printf("WARNING: Reducing frame size to %d and retry\n" , thisFrameSize);
						forcedSmallerFrame = true;
					}
					else
					{
						printf("ERROR: Frame size overrun!\n");
					}
				}
			}
		}
		while (retryCompression);

		processBlocks();

		sprintf(buffer, outputNameMask.c_str(), i);

		bool skipNewer = false;
		DynamicMessageHelper lastIterationFrame;
		if (lastIterationFrame.Read(buffer , true))
		{
			if (lastIterationFrame.GetBufferSize() < ret)
			{
				skipNewer = true;
				totalBytesOut += lastIterationFrame.GetBufferSize();
			}
			else
			{
				totalSaved += lastIterationFrame.GetBufferSize() - ret;
			}
		}

		if (skipNewer && cannotSkipDeltaWrite > 0)
		{
			printf("cannotSkipDeltaWrite = %d\n" , cannotSkipDeltaWrite);
			skipNewer = false;
		}
		if (cannotSkipDeltaWrite > 0)
		{
			if (!forcedSmallerFrame)
			{
				cannotSkipDeltaWrite--;
			}
		}
		if (skipNewer)
		{
			printf("Skipping delta write\n");
		}
		else
		{
			FILE *fp = fopen(buffer,"wb");
			if (!fp)
			{
				printf("Could not write file '%s'\n",buffer);
				exit(-1);
			}
			fwrite(sWorstCaseDelta,1,ret,fp);
			fclose(fp);
			totalBytesOut += ret;
		}


#if 1
		DeltaDecompress(sCurrentData,sWorstCaseDelta);
//		if (memcmp(sCurrentData,sNewData, (i==startFrame)?firstFrameSize:otherFrameSize/*(bank * 0x1000) + input.GetBufferSize()*/) != 0)
		int j;
		bool decompressionFailed = false;
		for (j=0; j < thisFrameSize ; j++)
		{
			if (sCurrentData[j] != sNewData[j])
			{
				printf("$%04x : %02x %02x\n", j , sCurrentData[j] , sNewData[j]);
				decompressionFailed = true;
			}
		}
		if (decompressionFailed)
		{
			printf("FAILED: Decompress didn't work for some reason\n");
			exit(-1);
		}
#else
		memcpy(sCurrentData,sNewData,sizeof(sCurrentData));
#endif

		bank++;
		if ( bank >= numBanks )
		{
			bank = 0;
		}
	}

	printf("Total bytes in %d and out %d saved %d\n",totalBytesIn,totalBytesOut , totalSaved);

	std::map<DynamicMessageHelper* , int , pDynamicMessageHelperCompare>::iterator st = allBlocks.begin();
	std::multimap<int,DynamicMessageHelper *> savingsToBlock;
	int totalBlockSizes = 0;
	while (st != allBlocks.end())
	{
		if (st->second > 1)
		{
			DynamicMessageHelper *block = st->first;
			int dictionarySaving = block->GetSize() * st->second;
			totalBlockSizes += block->GetSize();

			savingsToBlock.insert(std::pair<int,DynamicMessageHelper *>(dictionarySaving , block));
		}
		st++;
	}

	int dictionarySavings = 0;
	int dictionarySize = 0;
	int thisDictionarySize = 0;
	int dictionaryMaxSize = 0;
	fp = 0;

	printf("totalBlockSizes = %d\n" , totalBlockSizes);
	// Now pick the biggest savings first
	std::multimap<int,DynamicMessageHelper *>::reverse_iterator st2 = savingsToBlock.rbegin();
	std::list<std::pair<std::string,std::string>>::iterator st3 = dictionaryFiles.begin();
	if (st3 != dictionaryFiles.end())
	{
		dictionaryMaxSize = ParamToNum(st3->first.c_str());
		fp = fopen(st3->second.c_str() , "wb");
	}
	while (dictionaryMaxSize > 0 && st3 != dictionaryFiles.end() && st2 != savingsToBlock.rend())
	{
		const DynamicMessageHelper *block = st2->second;
		int thisBlockSize = block->GetSize();
		unsigned char *thisBlockBuffer = (unsigned char *) block->GetBuffer();
		// Trim the block from the start
		while (thisBlockSize > 3)
		{
			if (thisBlockBuffer[0] == thisBlockBuffer[1] && thisBlockBuffer[0] == thisBlockBuffer[2])
			{
				thisBlockSize--;
				thisBlockBuffer++;
			}
			else
			{
				break;
			}
		}
		while (thisBlockSize > 3)
		{
			if (thisBlockBuffer[thisBlockSize-1] == thisBlockBuffer[thisBlockSize-2] && thisBlockBuffer[thisBlockSize-1] == thisBlockBuffer[thisBlockSize-3])
			{
				thisBlockSize--;
			}
			else
			{
				break;
			}
		}
		const int maxMidLen = 6;
		for (int p = 0 ; p < thisBlockSize - maxMidLen; p++)
		{
			bool same = true;
			for (int p2 = 0 ; p2 < maxMidLen && same; p2++)
			{
				if (thisBlockBuffer[p + p2] != thisBlockBuffer[p + p2 + 1])
				{
					same = false;
					break;
				}
			}
			if (same)
			{
				memmove(thisBlockBuffer + p, thisBlockBuffer + p + maxMidLen, thisBlockSize - p - maxMidLen);
				thisBlockSize -= maxMidLen;
				p -= maxMidLen;
			}
		}
		if (thisBlockSize < 3)
		{
			continue;
		}
		// Maximum dictionary size check
		if ((thisDictionarySize + thisBlockSize) > dictionaryMaxSize)
		{
			fclose(fp);
			fp = 0;
			st3++;
			if (st3 != dictionaryFiles.end())
			{
				dictionaryMaxSize = ParamToNum(st3->first.c_str());
				fp = fopen(st3->second.c_str() , "wb");
			}
			else
			{
				break;
			}
			thisDictionarySize = 0;
		}

		if (fp == 0)
		{
			break;
		}

//		printf("using %d for %d\n" , st2->first , thisBlockSize);
		fwrite(thisBlockBuffer , thisBlockSize , 1 , fp);

		dictionarySize += thisBlockSize;
		thisDictionarySize += thisBlockSize;
		dictionarySavings += st2->first;
		st2++;
	}

	if (fp != 0)
	{
		fclose(fp);
	}

	while (st3 != dictionaryFiles.end())
	{
		st3++;
		if (st3 != dictionaryFiles.end())
		{
			fp = fopen(st3->second.c_str() , "wb");
			fclose(fp);
		}
	}

/*
	fp = fopen("usedMap.bin" , "wb");
	fwrite(sDataMapUsedForCopy , 1 , sizeof(sDataMapUsedForCopy) , fp);
	fclose(fp);
*/

	std::list<std::pair<std::string,std::string>>::iterator st4 = optimiseFiles.begin();
	while (st4 != optimiseFiles.end())
	{
		int memPos = ParamToNum(st4->first.c_str());
		DynamicMessageHelper file;
		if (!file.Read(st4->second.c_str() , true))
		{
			printf("Could not read file '%s'\n",st4->second.c_str());
			exit(-1);
		}
		printf("For file %s previous size = %d\n" , st4->second.c_str() , file.GetBufferSize());

		fp = fopen(st4->second.c_str() , "wb");
		for (int len = 0 ; len < file.GetBufferSize() ; len++)
		{
			if (sDataMapUsedForCopy[memPos+len])
			{
				fputc(((char *)file.GetBuffer())[len] , fp);
			}
		}
		printf("For file %s new size = %d\n" , st4->second.c_str() , (int) ftell(fp));
		fclose(fp);

		st4++;
	}



#ifdef ENABLE_DICTIONARY
	printf("Written dictionary size %d potential savings %d\n" , dictionarySize , dictionarySavings);
#endif

	// New RLE + frame coherence + block move 41212
	return 0;
}
