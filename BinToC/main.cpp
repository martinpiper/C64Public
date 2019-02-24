// From the "I don't believe I had to write this" tools writing guide.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Common/ParamToNum.h"

int main(int argc, char **argv)
{
	if (argc <= 1)
	{
		printf("bin2c.exe <input binary file>");
		exit(-1);
	}

	FILE *fp = fopen(argv[1] , "rb");
	if (NULL == fp)
	{
		printf("Could not open file for reading\n");
		exit(-1);
	}

	int skip = 0;
	int keep = 0x7fffffff;

	// Skip the first 2 parameters
	argc -= 2;
	argv += 2;

	while (argc > 0)
	{
		if (_stricmp(argv[0] , "-skip") == 0)
		{
			if (argc < 2)
			{
				printf("-skip needs a value\n");
				exit(-1);
			}
			skip = ParamToNum(argv[1]);

			argc-=2;
			argv+=2;
			continue;
		}
		if (_stricmp(argv[0] , "-keep") == 0)
		{
			if (argc < 2)
			{
				printf("-keep needs a value\n");
				exit(-1);
			}
			keep = ParamToNum(argv[1]);

			argc-=2;
			argv+=2;
			continue;
		}

		printf("Unrecognised option: %s\n" , argv[0]);
		exit(-1);
	}

	while (skip-- > 0)
	{
		fgetc(fp);
	}

	int bytesPerLine = -1;
	bool done = false;
	while (!feof(fp) && (keep-- > 0))
	{
		int c = fgetc(fp);

		if (EOF != c)
		{
			if (done)
			{
				printf(",");
				if (bytesPerLine < 15)
				{
					printf(" ");
				}
			}

			if (++bytesPerLine >= 16)
			{
				printf("\n");
				bytesPerLine = 0;
			}

			if (bytesPerLine == 0)
			{
				printf("\t");
			}

			printf("0x%02x" , c);

			done = true;
		}

	}
	printf("\n");

	return 0;
}
