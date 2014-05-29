// Assemble all the banks for the Berzerk Redux cartridge
// -n -b 0 -r ../BerzerkReduxCart/BerzerkReduxCart.prg -c 0 2 $ffff -w -r ../BerzerkRedux/BerzerkRedux.prg -b 1 -c 0 $0001 $ffff -w -b 2 -c 0 $2001 $ffff -w -b 3 -c 0 $4001 $ffff -w -b 4 -c 0 $6001 $ffff -w -b 5 -c 0 $8001 $ffff -w -b 6 -c 0 $a001 $ffff -w -b 7 -r ../BerzerkReduxCart/fw86muscomp.prg -c 0 2 $ffff -w -o test.crt

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../Common/ParamToNum.h"
#include "RNPlatform/Inc/MessageHelper.h"

using namespace RNReplicaNet;

static unsigned char sCartHeader_64K[] = {
	0x43, 0x36, 0x34, 0x20, 0x43, 0x41, 0x52, 0x54, 0x52, 0x49, 0x44, 0x47, 0x45, 0x20, 0x20, 0x20, 
	0x00, 0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0x13, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x43, 0x41, 0x52, 0x54, 0x36, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

static unsigned char sChipData[] = {
	0x43, 0x48, 0x49, 0x50, 0x00, 0x00, 0x20, 0x10, 0x00, 0x00, 0x00, 0 /*bank number low*/, 0x80, 0x00, 0x20, 0x00
};

int main( int argc , char **argv )
{
	static unsigned char bankData[0x2000];
	DynamicMessageHelper output;
	bool displayHelp = false;
	if ( argc <= 1 )
	{
		displayHelp = true;
	}

	DynamicMessageHelper workBuffer;

	argc--;
	argv++;
	while ( argc > 0 )
	{
		if ( argv[0][0] == '-' )
		{
			// A parameter so test for it
			switch ( argv[0][1] )
			{
				case 'h':
				default:
				{
					displayHelp = true;
					break;
				}

				case 'n':
				{
					// New header
					output.FreeBuffer();
					output.AddData( sCartHeader_64K , sizeof( sCartHeader_64K ) ); 
					break;
				}

				case 'b':
				{
					// New bank chunk
					int bankNum = 0;
					argc--;
					argv++;
					if ( argc  > 0 )
					{
						bankNum = ParamToNum( argv[0] );
						memset( bankData , bankNum+1 , sizeof( bankData ) );

						sChipData[ 0x0a ] = (unsigned char) (bankNum >> 8);
						sChipData[ 0x0b ] = (unsigned char) bankNum;

						printf( "New bank $%x\n" , bankNum );
					}
					break;
				}

				case 'r':
				{
					// Read data from a file into a work buffer
					argc--;
					argv++;
					if ( argc  > 0 )
					{
						workBuffer.FreeBuffer();
						if ( workBuffer.Read( argv[0] , true ) )
						{
							printf("File '%s' read $%x\n" , argv[0] , workBuffer.GetBufferSize() );
						}
						else
						{
							printf("Problem reading file '%s'\n" , argv[0] );
						}
					}
					break;
				}

				case 'c':
				{
					// Copy data from the work buffer to the current bank
					int startBank = 0;
					int startFile = 0;
					int endFile = 0;
					argc--;
					argv++;
					if ( argc  >= 3 )
					{
						startBank = ParamToNum( argv[0] );
						startFile = ParamToNum( argv[1] );
						endFile = ParamToNum( argv[2] );
					}

					if ( endFile >= workBuffer.GetBufferSize() )
					{
						endFile = workBuffer.GetBufferSize();
					}

					int toCopy = endFile - startFile;

					if ( ( startBank + toCopy ) > sizeof( bankData ) )
					{
						toCopy = sizeof( bankData ) - startBank;
					}

					if ( (startFile >= 0 ) && ( startFile <= workBuffer.GetBufferSize() ) && ( toCopy > 0 )  && ( ( startBank + toCopy ) <= sizeof( bankData ) ) )
					{
						printf(" Copy from $%x len $%x to $%x\n" , startFile , toCopy , startBank );
						memcpy( bankData + startBank , ( (char *)workBuffer.GetBuffer() ) + startFile , toCopy );
					}

					break;
				}

				case 'w':
				{
					// Write bank
					output.AddData( sChipData , sizeof( sChipData ) );
					output.AddData( bankData , sizeof( bankData ) );
					printf( " Write bank\n" );
					break;
				}


				case 'o':
				{
					// Write the file data
					argc--;
					argv++;
					if ( argc  > 0 )
					{
						if ( output.Write( argv[0] , true ) )
						{
							printf("File '%s' wrote $%x\n" , argv[0] , output.GetSize() );
						}
						else
						{
							printf("Problem writing file '%s'\n" , argv[0] );
						}
						output.FreeBuffer();
					}
					break;
				}
			}
		}

		argc--;
		argv++;
	}

	if ( displayHelp )
	{
		printf("MakeCart help\n\
-n : Output new cartridge header into the cartridge data buffer. This has to be before -b is used.\n\
-b <bank> : Clear the temporary 8K bank data with the current bank number.\n\
-r <file> : Read data file to the internal work buffer.\n\
-c <bank offset> <start work buffer> <end work buffer> : Copy Data from start to end work buffer offsets into the temporary bank data with the bank offset. If the data is over the end of the temporary bank size of 8K then it is truncated. Any data from any file can be written to any offset in the temporary bank data\n\
-w : Write the 8K temporary bank data with the chip/bank number from the preceding -b <bank> to the cartridge data buffer\n\
-o : Output the whole cartridge data buffer to the file.\n\
" );

	}

	return 0;
}
