// Harmless warning about browse information
#pragma warning (disable : 4786)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CompressRLE.h"

void TestRLEPack(u8 *input,const u32 inputLength, u8 *output, u32 *outLen)
{
	u32 foundLeast[256];
	memset(foundLeast,0,sizeof(foundLeast));

	// Total up the occurrences of each byte value
	u32 i;
	for (i=0;i<inputLength;i++)
	{
		foundLeast[input[i]]++;
	}

	// Find the least used byte
	u8 leastIndex = 0;
	for (i=1;i<256;i++)
	{
		if (foundLeast[i] < foundLeast[leastIndex])
		{
			leastIndex = (u8) i;
		}
	}

	u32 outPos = 0;
	// Save the marker
	output[outPos++] = leastIndex;

	// Now RLE pack with the least used byte as a marker
	for (i=0;i<inputLength;i)
	{
		bool match = false;
		if ( (i+3) < inputLength )
		{
			if ( (input[i] == input[i+1]) && (input[i] == input[i+2]))
			{
				match = true;
			}
		}
		// Check to see if we need to output a run
		if ( (input[i] == leastIndex) || match)
		{
			u32 j = 0;
			while ((input[i+j] == input[i]) && (j < 256) && ((i+j) < inputLength))
			{
				j++;
			}
			output[outPos++] = leastIndex;
			output[outPos++] = (u8)j;
			output[outPos++] = input[i];
			i+=j;
		}
		else
		{
			output[outPos++] = input[i++];
		}
	}

	// Last byte, will trigger a test for end of file
	output[outPos++] = leastIndex;

	*outLen = outPos;
}
