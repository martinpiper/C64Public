#include <stdlib.h>
#include "Music.h"
#include <list>

static void checkArgs(int argc , char *currentOpt)
{
	if(argc < 0)
	{
		printf("Need at least one more parameter after: %s\n" , currentOpt);
		exit(-1);
	}
}

int main(int argc , char **argv)
{
	Music output;

	while (--argc > 0)
	{
		argv++;
		if (argv[0][0] == '-')
		{
			char *currentOpt = argv[0];

			switch(argv[0][1])
			{
			default:
				{
					printf("Unrecognised option: %s\n" , argv[0]);
					exit(-1);
					break;
				}
			
			case 'i':
				{
					argc--;
					argv++;
					checkArgs(argc , currentOpt);

					Music current;
					current.LoadMusic(argv[0]);
					current.Compact();
					output = current;
					break;
				}

			case 'o':
				{
					argc--;
					argv++;
					checkArgs(argc , currentOpt);

					output.ExportASM(argv[0]);
					break;
				}
			}
		}
	}
	return 0;
}
