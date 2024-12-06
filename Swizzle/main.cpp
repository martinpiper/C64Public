// From the "I don't believe I had to write this" tools writing guide.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Common/ParamToNum.h"

int main(int argc, char **argv)
{
	if (argc <= 5)
	{
		printf("swizzle.exe <input binary file> <output binary file> <skip bytes> <rows> <columns>\n");
		exit(-1);
	}

	FILE *infp = fopen(argv[1] , "rb");
	if (NULL == infp)
	{
		printf("Could not open file for reading\n");
		exit(-1);
	}

	FILE *outfp = fopen(argv[2] , "wb");
	if (NULL == outfp)
	{
		printf("Could not open file for writing\n");
		exit(-1);
	}

	int skip = ParamToNum(argv[3]);
	int rows = ParamToNum(argv[4]);
	int columns = ParamToNum(argv[5]);


	fseek(infp, skip, SEEK_SET);

	size_t expectedBytes = rows * columns;

	char *theData = (char *) calloc(expectedBytes , 1);
	int numRead = fread(theData, 1, expectedBytes , infp);
	if (numRead != expectedBytes)
	{
		printf("Expected %d bytes but read %d bytes\n" , expectedBytes , numRead);
	}
	fclose(infp);

	// Output swizzled data
	for (int x = 0 ; x < columns ; x++)
	{
		for (int y = 0 ; y < rows ; y++)
		{
			fputc(theData[x + (y * columns)] , outfp);
		}
	}

	fclose(outfp);

	return 0;
}
