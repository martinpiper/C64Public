//..\Data\frm*05d.sch 0 1 1 12 2 $1000 $1ff8 ..\Data\frm*05d.del
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <assert.h>
#include <map>
#include <list>
#include "RNPlatform/Inc/MessageHelper.h"
#include "../../Common/ParamToNum.h"

using namespace RNReplicaNet;

// Enough for 2 x (screen, colour, char data)
unsigned char sCurrentData[0x10000];
unsigned char sNewData[0x10000];
unsigned char sWorstCaseDelta[0x10000 + (0x21 * 2) + 2];
unsigned char sTestDecomp[0x10000];
static bool sBitmapMode = false;

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

int DeltaCompress(unsigned char *output, const unsigned char *input, const unsigned char *delta, const int inputLength)
{
	std::multimap<int,int> inputToPos , deltaToPos;

	u32 foundLeast[256];
	memset(foundLeast,0,sizeof(foundLeast));

	// Total up the occurrences of each byte value
	int i;
	for (i=0;i<inputLength;i++)
	{
		foundLeast[delta[i]]++;
	}

	for (i=0;i<inputLength - kKeyLength;i++)
	{
		inputToPos.insert(std::pair<int,int>(makeKey(input+i),i));
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

	// Now RLE pack with the least used byte as a marker
	for (i=0;i<inputLength;i)
	{
		int size;
		int bestSize = -1;
		// Check for no difference with the previous data
		if (input)
		{
			for (size = 2 ; (size < 256) && ((i+size) < inputLength) ; size++)
			{
				if (memcmp(delta + i, input + i,size) != 0)
				{
					break;
				}
				bestSize = size;
			}
			if (bestSize> 0)
			{
				if ( emittedLastSkipCodesPos == -1 )
				{
					emittedLastSkipCodesPos = outPos;
				}
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
				emittedLastSkipCodesPos = -1;
				output[outPos++] = leastIndex2;
				output[outPos++] = 0;
				i++;
				continue;
			}

			if ((delta[i] == leastIndex3) && (j == 1))
			{
				emittedLastSkipCodesPos = -1;
				output[outPos++] = leastIndex3;
				output[outPos++] = 0xff;
				i++;
				continue;
			}

			emittedLastSkipCodesPos = -1;
			output[outPos++] = leastIndex;
			output[outPos++] = (u8)j;
			output[outPos++] = delta[i];
			i+=j;
			continue;
		}

#if 1
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
			for (size = 5 ; (size < 255) && ((pos+size) < inputLength) && ((i+size) < inputLength) ; size++)
			{
				// Avoid copying from within our own block
				if ( (pos >= i) && (pos <= (i+size)) )
				{
					continue;
				}
				if ( ((pos+size-1) >= i) && ((pos+size-1) <= (i+size)) )
				{
					continue;
				}

				if ( (i >= pos) && (i <= (pos+size)) )
				{
					continue;
				}
				if ( ((i+size) >= pos) && ((i+size-1) <= (pos+size)) )
				{
					continue;
				}
				
				if (sBitmapMode)
				{
					// MPi: TODO: Filter out bitmap screen data sprite pointers?
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
				}
				else
				{
					// The comparison is not what we would have decompressed already, so compare the other buffer
					if ( (pos - i) < size )
					{
						continue;
					}
					// Don't consider addresses that are too high
					if (i >= (inputLength-256))
					{
						break;
					}
					if (pos >= (inputLength-256))
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
					}
					else
					{
						break;
					}
				}
			}
		}

		if (bestSize > 4)
		{
			printf("Using %d / %d   pos %d size %d           \r",i,inputLength,bestPos,bestSize);
			emittedLastSkipCodesPos = -1;
			output[outPos++] = leastIndex3;
			output[outPos++] = bestPos >> 8;
			output[outPos++] = bestPos;
			output[outPos++] = bestSize;

			i += bestSize;
			continue;
		}
#endif

		output[outPos++] = delta[i++];
	}

	// Remove any trailing skip codes, since they don't actually change any buffer data they can be safely removed.
	if ( emittedLastSkipCodesPos != -1 )
	{
		printf("Trimmed %d                                             \n" , outPos - emittedLastSkipCodesPos );
		outPos = emittedLastSkipCodesPos;
	}

	output[outPos++] = leastIndex;
	output[outPos++] = 0;

	return outPos;
}

void DeltaDecompress(unsigned char *out, const unsigned char *delta)
{
	unsigned char *base = out;
	const unsigned char *origDelta = delta;
	unsigned char opcode1 = *delta++;
	unsigned char opcode2 = *delta++;
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

int main(int argc, char **argv)
{
	if (argc < 10)
	{
		printf("DeltaCompression : <-bitmap> <Input file mask in name*05d.ext format> <byte offset for start> <start frame number> <frame step> <end frame number> <number of banks> <first frame size> <other frame size> <output file name mask in name*05d.ext format>\n");
		return -1;
	}

	if (strcmp(argv[1] , "-bitmap") == 0)
	{
		sBitmapMode = true;
		argc--;
		argv++;
	}

	memset(sCurrentData,0,sizeof(sCurrentData));
	memset(sNewData,0,sizeof(sNewData));

	std::string inputNameMask = argv[1];
	std::replace<std::string::iterator,char>(inputNameMask.begin(),inputNameMask.end(),'*','%');
	int byteOffset = ParamToNum(argv[2]);
	int startFrame = ParamToNum(argv[3]);
	int frameStep = ParamToNum(argv[4]);
	int endFrame = ParamToNum(argv[5]);
	int numBanks = ParamToNum(argv[6]);
	int firstFrameSize = ParamToNum(argv[7]);
	int otherFrameSize = ParamToNum(argv[8]);
	std::string outputNameMask = argv[9];
	std::replace<std::string::iterator,char>(outputNameMask.begin(),outputNameMask.end(),'*','%');

	// Prime all the banks with what the data should look like at the end of the animation sequence.
	int bank = 0;
	int i;
	char buffer[1024];
	for (i=startFrame;i<=endFrame;i+=frameStep)
	{
		DynamicMessageHelper input;
		sprintf(buffer, inputNameMask.c_str(), i);
		printf("Prime frame '%s'\r",buffer);
		if (!input.Read(buffer,true))
		{
			printf("Could not read file '%s'\n",buffer);
			exit(-1);
		}

		memcpy(sCurrentData + byteOffset + (bank * firstFrameSize),input.GetBuffer(),input.GetBufferSize());

		bank++;
		if ( bank >= numBanks )
		{
			bank = 0;
		}
	}

	printf("\n");

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
	for (i=startFrame;i<=endFrame;i+=frameStep)
	{
		DynamicMessageHelper input;
		sprintf(buffer, inputNameMask.c_str(), i);
		if (!input.Read(buffer,true))
		{
			printf("Could not read file '%s'\n",buffer);
			exit(-1);
		}

		// MPi: TODO: File extent and range checking
		memcpy(sNewData + byteOffset + (bank * firstFrameSize),input.GetBuffer(),input.GetBufferSize());

		totalBytesIn += input.GetBufferSize();
		// Don't use the previous buffer for the first frame
//		int ret = DeltaCompress(sWorstCaseDelta,(i==startFrame)?0:sCurrentData,sNewData, (i==startFrame)?firstFrameSize:otherFrameSize/*(bank * 0x1000) + input.GetBufferSize() + byteOffset*/);
		// Use the previous buffer
		int ret = DeltaCompress(sWorstCaseDelta,sCurrentData,sNewData, otherFrameSize);
		printf("Frame %d in %d comp %d                         \n",i,input.GetBufferSize(),ret);
		sprintf(buffer, outputNameMask.c_str(), i);
		FILE *fp = fopen(buffer,"wb");
		if (!fp)
		{
			printf("Could not write file '%s'\n",buffer);
			exit(-1);
		}
		fwrite(sWorstCaseDelta,1,ret,fp);
		fclose(fp);
		totalBytesOut += ret;

#if 0
		DeltaDecompress(sCurrentData,sWorstCaseDelta);
//		if (memcmp(sCurrentData,sNewData, (i==startFrame)?firstFrameSize:otherFrameSize/*(bank * 0x1000) + input.GetBufferSize()*/) != 0)
		int j;
		bool decompressionFailed = false;
		for (j=0; j<otherFrameSize ; j++)
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

	// Old RLE 42677 bytes
	printf("Total bytes in %d and out %d\n",totalBytesIn,totalBytesOut);
	// New RLE + frame coherence + block move 41212
	return 0;
}
