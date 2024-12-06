// Harmless warning about browse information
#pragma warning (disable : 4786)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <string>
#include "Compress.h"
#include "CompressE.h"
#include "CompressRLE.h"
#include "CompressU.h"
#include "CompressV.h"
#include "Decompress.h"
#include "DecompressE.h"
#include "DecompressU.h"
#include "DecompressV.h"
#include "../Common/ParamToNum.h"

using namespace RNReplicaNet::RNXPCompression;

// Some test command lines
//-c C:\work\C64\MusicEditorHelp\MusicEditorHelp.prg c:\temp\t.cprg
//-c C:\work\C64\MusicEditorHelp\MusicEditorHelp.prg c:\temp\t.cprg 2
//-d c:\temp\t.cprg c:\temp\t.prg
//-c C:\work\ReplicaNetWork\TestXPCompression\Scroller.prg c:\temp\t.cprg 2
//-c C:\work\C64\CityGame\OriginalData.prg c:\temp\t.cprg 2
//-c C:\work\C64\Decompression\TestScreen.bin c:\temp\t.cprg
//-c64b C:\work\C64\RacingGame\Original\OriginalData.prg c:\temp\tc.prg 2048
//-c64b C:\work\C64\CityGame\OriginalData.prg c:\temp\tc.prg 22016
//-c64b ScrollerUT1024.prg c:\temp\tc.prg 1024
//-c64b ScrollerUT1024.prg c:\temp\tc.prg 0x400
//-c64b ScrollerUT1024.prg c:\temp\tc.prg $400
//-c UnitTest1.dat c:\temp\ut1.cdat
//-c64m C:\work\C64\RacingGame\Original\OriginalData.prg c:\temp\tc.prg 2048
//-c64mb C:\work\C64\RacingGame\Original\OriginalData.prg c:\temp\tc.prg 2048
//-cut c:\temp\t.txt c:\temp\tout.txt 123 $200
//-hex C:\Work\BombJack\output\DebugAudioOutput.txt c:\temp\t.bin 3 4
//-hex C:\Work\BombJack\output\DebugAudioOutput.txt c:\temp\t.bin 0 4

//-ce C:\work\C64\MusicEditorHelp\MusicEditorHelp.prg c:\temp\t.rnzip2
//c:\ReplicaNet\ReplicaNetPublic\RNZip\RNZip.exe -c C:\work\C64\MusicEditorHelp\MusicEditorHelp.prg c:\temp\t.rnzip
//Input length = 24372 output length = 14107

//34760
//19516 : c:\temp\exomizer.exe raw TestData\5.bin -o c:\temp\t.out
//20362 : ..\bin\LZMPi.exe -c TestData\5.bin c:\temp\t.bin
//846

// -c64b "C:\temp\wizball_ocean_6389_min.prg" "C:\temp\wizball_ocean_6389_min_lzmpi.prg" $6389
// Input length = $f900 output length = $9266
// -c "C:\temp\wizball_ocean_6389_min.prg" "c:\temp\t.cmp" 2
// Input length = $f900 output length = $90a8
// -o1 -o2 -o3 -c "C:\temp\wizball_ocean_6389_min.prg" "c:\temp\t.cmp" 2
// Input length = $f900 output length = $8ffb
// -o2 -c "C:\temp\wizball_ocean_6389_min.prg" "c:\temp\t.cmp" 2
// -o1 -o2 -c "C:\temp\wizball_ocean_6389_min.prg" "c:\temp\t.cmp" 2
// Input length = $f900 output length = $9017

// -ce "C:\temp\wizball_ocean_6389_min.prg" "c:\temp\t.cmp" 2
// Input length = $f900 output length = $927a


// -cu "C:\temp\wizball_ocean_6389_min.prg" "c:\temp\t.cmp4" 2
// Input length = $f900 output length = $8836
// -c64mub "C:\temp\wizball_ocean_6389_min.prg" "C:\temp\wizball_ocean_6389_min_lzmpiu.prg" $6389
// Input length = $f900 output length = $893f


// -o4 2 2 3 1 -c UnitTest2.dat c:\temp\ut2.cdat

// TODO: Add an option to compress and run BASIC code.
// Related^^ TODO: Update $2d and $2e properly so that other applications that expect them will work.
//-c64b SEUCK.prg SEUCKComp.prg 2064
//>Actually what can be done is to add to the loaded input data some code at some out of the way address (like $f000 or $c000)
// Then that will warmboot/restore BASIC and the correct expected values in $2d $2e etc.
// Then this code can call the BASIC code without needing any decompress code changes in the binary blobs below. Also it'll mean the booting code is also compressed.

// Created by ..\Decompression\BuildData.bat
#include "../Decompression/PrgData.cpp"

const u32 sStartOfBASIC = 0x801;

int main(int argc,char **argv)
{
	int machineMemoryTop = 0x10000;

	if (argc < 4)
	{
		printf("LZMPi : A very simple file compressor or decompressor suitable for C64 files.\n\n"
			"To compress a binary file using LZMPi mode use:\n"
			" -c <input file> <outfile file> [offset from the start of the file]\n"
			"To decompress files compressed with -c use:\n"
			" -d <input file> <outfile file>\n"
			"To use LZMPiE mode (Simpler compression with faster decompression) then use:"
			" -ce <input file> <outfile file> [offset from the start of the file]\n"
			"To use LZMPiU mode (Better compression, slightly more complex decompression) then use:"
			" -cu <input file> <outfile file> [offset from the start of the file]\n"
			"To use LZMPiV mode (Better compression, much slower and more complex decompression) then use:"
			" -cv <input file> <outfile file> [offset from the start of the file]\n"
			" The -c64 with vh option will use $600 of tables at $fa00, whereas the v option will use tables at $200. When using vh, the top most memory address is reduced appropriately. The vh option allows meory to be used starting from $200, the v option allows memory to be used starting from $800.\n"
			"To use RLE mode then use:"
			" -cr <input file> <outfile file> [offset from the start of the file]\n"
			"To create a C64 self extracting binary use:\n"
			" -c64[m][b][r][e][u][v][vh] <input file> <outfile file> <run address> [start address] [skip start bytes]\n\n"
			"-c64 will compress a C64 prg file with the start address being the first two\n"
			"bytes of the file. The optional start address will override the start address\n"
			"read from the first two bytes of the prg file.\n"
			" -c64b will cause the border to flash during decompression.\n"
			" -c64 or -c64b allows memory in the range $400-$fff9 to be used and is less destructive to zeropage.\n"
			" -c64m or -c64mb will use max mode ($200-$fff9 available) without or with border flashing.\n"
			" Adding one of e/u/r will use the corresponding compression method.\n"
			"By default the border will not flash.\n\n"
			"Use the following to skip a number of bytes from the start of the file and\n"
			"set a length. If the length equals 0 the length is still set from the file: \n"
			" -c <input file> <outfile file> <offset> [length]\n\n"
			" -cut <input file> <outfile file> <offset> [length]\n"
			"-cut will not compress the file, meaning data is just effectively cut and written to the output file.\n"
			"-hex <input file containing hex data> <output file> <start offset> <skip>\n"
			"-t <address>\n"
			" The default value is $10000. Must be first in the parameter list. This will set the top most memory address used by the decompression code when using the -c64 self extracting code. This is useful to segregate the top pages of memory so they can be used for saved data between file loads.\n"
			" -o1 Enable permutation optimise compression pass (slow). Must be before other compression options.\n"
			" -o2 Enable shuffle optimise compression pass (very slow). Must be before other compression options.\n"
			" -o3 Enable long threshold optimise compression pass (quite slow). Must be before other compression options.\n"
			" -o4 <num1> <num2> <num3> <num4> Specifies the four tweak values displayed during -o1 which avoids the slow permutation search. Must be before other compression options.\n"
			" -ol <typical number 1-10> Default 10. Sets the compression optimisation level, from 1 being the least to 10 being a sensible maximum. Numbers larger than 10 will probably significantly increase compression times with little benefit.\n"
			" -no Do not output the original file size. Must be before other options.\n"
			" -yo Output the original load address (first two bytes of the file). Must be before other options.\n"
		);
		exit(-1);
	}

	u8 input[65536];
	u8 output[73728];
	int offset = 0;
	int wantedLength = 0;
	bool outputC64Header = false;
	bool doNotOutputOriginalSize = false;
	bool outputOriginalLoadAddress = false;
	int originalLoadAddress = 0;
	bool useRLE = false;
	u32 startC64Code = 0x900;
	u32 loadC64Code = 0;
	u32 loadC64CodeSkipStart = 2;
	bool flashBorder = false;
	bool maxMode = false;
	bool useRNZipMode = false;
	bool enablePermutation = false;
	bool enableShuffle = false;
	bool enableOffsetThreshold = false;
	bool handledPreParam = false;
	bool useCompressionU = false;
	bool useCompressionV = false;
	bool useCompressionWithHigh = false;
	int compressionLevel = 10;

	do
	{
		handledPreParam = false;

		if (argv[1][0] == '-' && argv[1][1] == 'n' && argv[1][2] == 'o')
		{
			doNotOutputOriginalSize = true;
			argc--;
			argv++;
			handledPreParam = true;
		}

		if (argv[1][0] == '-' && argv[1][1] == 'y' && argv[1][2] == 'o')
		{
			outputOriginalLoadAddress = true;
			argc--;
			argv++;
			handledPreParam = true;
		}

		if (argv[1][0] == '-' && argv[1][1] == 'o' && argv[1][2] == 'l')
		{
			compressionLevel = ParamToNum(argv[2]);

			argc-=2;
			argv+=2;
			handledPreParam = true;
		}

		if (argv[1][0] == '-' && argv[1][1] == 'o' && argv[1][2] == '1')
		{
			enablePermutation = true;
			argc--;
			argv++;
			handledPreParam = true;
		}

		if (argv[1][0] == '-' && argv[1][1] == 'o' && argv[1][2] == '2')
		{
			enableShuffle = true;
			argc--;
			argv++;
			handledPreParam = true;
		}

		if (argv[1][0] == '-' && argv[1][1] == 'o' && argv[1][2] == '3')
		{
			enableOffsetThreshold = true;
			argc--;
			argv++;
			handledPreParam = true;
		}

		if (argv[1][0] == '-' && argv[1][1] == 'o' && argv[1][2] == '4')
		{
			gXPCompressionTweak1 = ParamToNum(argv[2]);
			gXPCompressionTweak2 = ParamToNum(argv[3]);
			gXPCompressionTweak3 = ParamToNum(argv[4]);
			gXPCompressionTweak4 = ParamToNum(argv[5]);

			argc-=5;
			argv+=5;
			handledPreParam = true;
		}

		if (argv[1][1] == 't')
		{
			machineMemoryTop = ParamToNum(argv[2]);
			argc-=2;
			argv+=2;
			handledPreParam = true;
		}
	} while (handledPreParam);

	if (argv[1][1] == 'c')
	{
		printf("Compression level %d\n" , compressionLevel);
	}

	if (((argv[1][1] == 'c') || (argv[1][1] == 'd')) && (argv[1][2] == 'r'))
	{
		useRLE = true;
	}
	if (((argv[1][1] == 'c') || (argv[1][1] == 'd')) && (argv[1][2] == 'u'))
	{
		useCompressionU = true;
	}
	if (((argv[1][1] == 'c') || (argv[1][1] == 'd')) && (argv[1][2] == 'v'))
	{
		useCompressionV = true;
	}
	if (((argv[1][1] == 'c') || (argv[1][1] == 'd')) && (argv[1][2] == 'e'))
	{
		useRNZipMode = true;
	}

	if ((argv[1][1] == 'c') && (argv[1][2] == '6') && (argv[1][3] == '4'))
	{
		if (argc >= 5)
		{
			std::string subOption(argv[1]+4);
			outputC64Header = true;

			if (subOption.find('h') != std::string::npos)
			{
				printf("Using high mode\n");
				useCompressionWithHigh = true;
			}

			if (subOption.find('e') != std::string::npos)
			{
				printf("Using LZMPiE mode\n");
				useRNZipMode = true;
			}

			if (subOption.find('m') != std::string::npos)
			{
				printf("Using max mode\n");
				maxMode = true;
			}

			if (subOption.find('b') != std::string::npos)
			{
				printf("Making the border flash.\n");
				flashBorder = true;
			}

			if (subOption.find('r') != std::string::npos)
			{
				printf("Using RLE mode\n");
				useRLE = true;
			}

			if (subOption.find('u') != std::string::npos)
			{
				printf("Using LZMPiU mode\n");
				useCompressionU = true;
			}

			if (subOption.find('v') != std::string::npos)
			{
				printf("Using LZMPiV mode\n");
				useCompressionV = true;
				if (useCompressionWithHigh)
				{
					machineMemoryTop = std::min(0xfa00 , machineMemoryTop);
					printf("Mem top now $%x\n" , machineMemoryTop);
				}
			}

			startC64Code = ParamToNum(argv[4]);
			printf("Using run address $%04x\n",startC64Code);

			if (argc >= 6)
			{
				loadC64Code = ParamToNum(argv[5]);
				printf("Using load address $%04x\n",loadC64Code);
			}

			if (argc >= 7)
			{
				loadC64CodeSkipStart = ParamToNum(argv[6]);
				printf("Using skip start bytes $%04x\n",loadC64CodeSkipStart);
			}
		}
		else
		{
			printf("%s: Missing a start address?\n",argv[1]);
			return -1;
		}
	}
	else if (argv[1][1] == 'c')
	{
		if (argc >= 5)
		{	
			offset = ParamToNum(argv[4]);
		}
		if (argc >= 6)
		{
			wantedLength = ParamToNum(argv[5]);
		}
	}

	int machineMemoryTopMinus256 = machineMemoryTop - 0x100;

	FILE *fp;
	printf("Opening '%s' for reading...\n",argv[2]);
	fp = fopen(argv[2],"rb");
	if (!fp)
	{
		printf("Problem opening '%s' for reading\n",argv[2]);
		exit(-1);
	}

	// Always read this, then reset the file pointer...
	originalLoadAddress = fgetc(fp);
	originalLoadAddress |= fgetc(fp) << 8;
	fseek(fp , 0 , SEEK_SET);

	// -hex
	if (argv[1][1] == 'h' && argv[1][2] == 'e' && argv[1][3] == 'x')
	{
		int skip = ParamToNum(argv[4]);
		int skipDefault = ParamToNum(argv[5]);
		FILE *out = fopen(argv[3],"wb");
		if (!out)
		{
			printf("Problem opening '%s' for writing\n",argv[3]);
			exit(-1);
		}

		while (!feof(fp))
		{
			int a = tolower(fgetc(fp));
			if ((a >= '0' && a <= '9') || (a >= 'a' && a <= 'f'))
			{
				if (a >= 'a' && a <= 'f')
				{
					a = 10 + (a - 'a');
				} else {
					a = a - '0';
				}
				int b = tolower(fgetc(fp));
				if ((b >= '0' && b <= '9') || (b >= 'a' && b <= 'f'))
				{
					if (b >= 'a' && b <= 'f')
					{
						b = 10 + (b - 'a');
					} else {
						b = b - '0';
					}
					int output = (a << 4) | b;
					if (skip > 0)
					{
						skip--;
						continue;
					}
					fputc(output , out);
					skip = skipDefault-1;
				}
			}
		}
		fclose(fp);
		printf("output len = %d\n" , ftell(out));
		fclose(out);
		return 0;
	}

	if (outputC64Header)
	{
		if (!loadC64Code)
		{
			loadC64Code = fgetc(fp);
			loadC64Code |= fgetc(fp) << 8;
			printf("Read load address (from first two bytes) as $%04x\n",loadC64Code);
		}
		else
		{
			printf("Skipping start bytes $%04x\n",loadC64CodeSkipStart);
			fseek(fp,loadC64CodeSkipStart,SEEK_SET);
		}
	}

	if (offset)
	{
		fseek(fp,offset,SEEK_SET);
	}

	u16 inputSize = fread(input,1,sizeof(input),fp);

	if (wantedLength)
	{
		inputSize = wantedLength;
	}

	fclose(fp);

	int endAddress = (int)loadC64Code+(int)inputSize;

	printf("End address = $%04x\n",endAddress);

	int adjust = endAddress - 0xfffa;
	if (adjust > 0)
	{
		inputSize -= adjust;
		endAddress = (int)loadC64Code+(int)inputSize;
		printf("New end address = $%04x adjusted by $%x\n",endAddress , adjust);
	}


	printf("Opening '%s' for writing...\n",argv[3]);
	fp = fopen(argv[3],"wb");
	if (!fp)
	{
		printf("Problem opening '%s' for writing\n",argv[3]);
		exit(-1);
	}

	printf("Working...\n");

	if (argv[1][1] == 'c')
	{
		// -cut
		if (argv[1][2] == 'u' && argv[1][3] == 't')
		{
			fwrite(input , 1 , inputSize , fp);
			fclose(fp);
			printf("offset = $%04x length = $%04x\n",(int)offset,(int)inputSize);
			return 0;
		}

#if 0
		u16 j;
		for (j=0;j<inputSize;j++)
		{
			if ( (j % 16) == 0)
			{
				printf("\n");
			}
			printf("0x%0.2x,",input[j]);
		}
		printf("\n");
		exit(0);
#endif

		if (!outputC64Header)
		{
			if (!doNotOutputOriginalSize)
			{
				fwrite(&inputSize,1,sizeof(inputSize),fp);
			}
			if (outputOriginalLoadAddress)
			{
				u16 theLoadAddress = (u16) originalLoadAddress;
				fwrite(&theLoadAddress,1,sizeof(theLoadAddress),fp);
			}
		}

		u32 outSize;
		u32 outBitSize;
		int bestBits = 0;
#if 1
		// For now we will always use one escape bit because it seems to consistently generate
		// the best results.
		if (useCompressionU)
		{
			CompressionU comp;
			comp.Compress(input,inputSize,output,&outSize,compressionLevel);
		}
		else if (useCompressionV)
		{
			CompressionV comp;
			comp.Compress(input,inputSize,output,&outSize,compressionLevel);
		}
		else if (useRLE)
		{
			TestRLEPack(input,inputSize,output,&outSize);
		}
		else
		{
			int outBitSizeTrue;
			int outBitSizeFalse;

			printf("Compression stage: Dictionary test\n");
			gXPCompressionTweak6 = true;
			if (useRNZipMode)
			{
				CompressionE comp;
				comp.Compress(input,inputSize,output,&outSize,compressionLevel);
				outBitSizeTrue = comp.mTotalBitsOut;
			}
			else
			{
				Compression comp;
				comp.Compress(input,inputSize,output,&outSize,compressionLevel,1);
				outBitSizeTrue = comp.mTotalBitsOut;
			}
			gXPCompressionTweak6 = false;
			if (useRNZipMode)
			{
				CompressionE comp;
				comp.Compress(input,inputSize,output,&outSize,compressionLevel);
				outBitSizeFalse = comp.mTotalBitsOut;
			}
			else
			{
				Compression comp;
				comp.Compress(input,inputSize,output,&outSize,compressionLevel,1);
				outBitSizeFalse = comp.mTotalBitsOut;
			}
			printf("Out bit sizes: %d %d\n" , outBitSizeTrue , outBitSizeFalse);
			if (outBitSizeTrue <= outBitSizeFalse)
			{
				printf("Compression dictionary enabled\n");
				gXPCompressionTweak6 = true;
			}
			else
			{
				printf("Compression dictionary disabled\n");
				gXPCompressionTweak6 = false;
			}

			printf("Compression stage: Ratio test\n");
			gXPCompressionTweak5 = true;
			if (useRNZipMode)
			{
				CompressionE comp;
				comp.Compress(input,inputSize,output,&outSize,compressionLevel);
				outBitSizeTrue = comp.mTotalBitsOut;
			}
			else
			{
				Compression comp;
				comp.Compress(input,inputSize,output,&outSize,compressionLevel,1);
				outBitSizeTrue = comp.mTotalBitsOut;
			}
			gXPCompressionTweak5 = false;
			if (useRNZipMode)
			{
				CompressionE comp;
				comp.Compress(input,inputSize,output,&outSize,compressionLevel);
				outBitSizeFalse = comp.mTotalBitsOut;
			}
			else
			{
				Compression comp;
				comp.Compress(input,inputSize,output,&outSize,compressionLevel,1);
				outBitSizeFalse = comp.mTotalBitsOut;
			}
			printf("Out bit sizes: %d %d\n" , outBitSizeTrue , outBitSizeFalse);
			if (outBitSizeTrue <= outBitSizeFalse)
			{
				printf("Compression ratio enabled\n");
				gXPCompressionTweak5 = true;
			}
			else
			{
				printf("Compression ratio disabled\n");
				gXPCompressionTweak5 = false;
			}

			if (enablePermutation)
			{
				printf("Compression stage: Baseline\n");
				if (useRNZipMode)
				{
					CompressionE comp;
					comp.Compress(input,inputSize,output,&outSize,compressionLevel);
					outBitSize = comp.mTotalBitsOut;
				}
				else
				{
					Compression comp;
					comp.Compress(input,inputSize,output,&outSize,compressionLevel,1);
					outBitSize = comp.mTotalBitsOut;
				}
				printf("Out bytes: %d\n" , outSize);

				printf("Compression stage: Permutation\n");
				int besta = gXPCompressionTweak1,bestb = gXPCompressionTweak2,bestc = gXPCompressionTweak3,bestd = gXPCompressionTweak4;

				int a,b,c,d;
				for (a=1 ; a < 10 ; a++)
				{
					for (b=a ; b < 10 ; b++)
					{
						for (c=b ; c < 10 ; c++)
						{
							for (d=1 ; d < 10 ; d++)
							{
								printf("Trying : %d %d %d %d                \r", a , b ,c , d);
								int ret;
								gXPCompressionTweak1 = a;
								gXPCompressionTweak2 = b;
								gXPCompressionTweak3 = c;
								gXPCompressionTweak4 = d;

								u32 comp2mTotalBitsOut;
								if (useRNZipMode)
								{
									CompressionE comp2;
									ret = comp2.Compress(input,inputSize,output,&outSize,compressionLevel);
									comp2mTotalBitsOut = comp2.mTotalBitsOut;
								}
								else
								{
									Compression comp2;
									comp2.mEarlyOut = outBitSize;
									ret = comp2.Compress(input,inputSize,output,&outSize,compressionLevel,1);
									comp2mTotalBitsOut = comp2.mTotalBitsOut;
								}
								printf("Working %d : %d %d %d %d                \r",outBitSize - comp2mTotalBitsOut , a , b ,c , d);
								if (ret == -1)
								{
									continue;
								}

								if (comp2mTotalBitsOut < outBitSize)
								{
									printf("\nGot size %d\n",outSize);
									printf("New best tweak %d : %d %d %d %d\n",outBitSize - comp2mTotalBitsOut , a , b ,c , d);
									outBitSize = comp2mTotalBitsOut;
									besta = gXPCompressionTweak1;
									bestb = gXPCompressionTweak2;
									bestc = gXPCompressionTweak3;
									bestd = gXPCompressionTweak4;
								}
							}
						}
					}
				}
				gXPCompressionTweak1 = besta;
				gXPCompressionTweak2 = bestb;
				gXPCompressionTweak3 = bestc;
				gXPCompressionTweak4 = bestd;
			}

			if(enableOffsetThreshold)
			{
				printf("Compression stage: Baseline for threshold\n");
				if (useRNZipMode)
				{
					CompressionE comp;
					comp.Compress(input,inputSize,output,&outSize,compressionLevel);
					outBitSize = comp.mTotalBitsOut;
				}
				else
				{
					Compression comp;
					comp.Compress(input,inputSize,output,&outSize,compressionLevel,1);
					outBitSize = comp.mTotalBitsOut;
				}
				printf("Out bit size threshold: %d\n" , outBitSize);

				u32 bestThreshold = LONG_OFFSET_THRESHOLD;

				for (LONG_OFFSET_THRESHOLD = 0x100 ; LONG_OFFSET_THRESHOLD < (u32)(inputSize/2); LONG_OFFSET_THRESHOLD += 0x100)
				{
					printf("Trying... Bit size threshold: $%x\r" , LONG_OFFSET_THRESHOLD);
					u32 comp2mTotalBitsOut;
					if (useRNZipMode)
					{
						CompressionE comp;
						comp.Compress(input,inputSize,output,&outSize,compressionLevel);
						comp2mTotalBitsOut = comp.mTotalBitsOut;
					}
					else
					{
						Compression comp;
						comp.Compress(input,inputSize,output,&outSize,compressionLevel,1);
						comp2mTotalBitsOut = comp.mTotalBitsOut;
					}
					if (comp2mTotalBitsOut < outBitSize)
					{
						printf("\nNew out bit size threshold: %d for threshold $%x\n" , comp2mTotalBitsOut , LONG_OFFSET_THRESHOLD);
						outBitSize = comp2mTotalBitsOut;
						bestThreshold = LONG_OFFSET_THRESHOLD;
					}
				}

				LONG_OFFSET_THRESHOLD = bestThreshold;

				if (useRNZipMode)
				{
					CompressionE comp;
					comp.Compress(input,inputSize,output,&outSize,compressionLevel);
					outBitSize = comp.mTotalBitsOut;
				}
				else
				{
					Compression comp;
					comp.Compress(input,inputSize,output,&outSize,compressionLevel,1);
					outBitSize = comp.mTotalBitsOut;
				}

				printf("\nFinal out bit size threshold: %d for threshold $%x\n" , outBitSize , LONG_OFFSET_THRESHOLD);
			}

			if (useRNZipMode)
			{
				CompressionE comp3;
				comp3.Compress(input,inputSize,output,&outSize,10);
				outBitSize = comp3.mTotalBitsOut;

				// This tries to optimise the compression choices even more by optionally trying each
				// dictionary and literal choice one at a time to get the best output length.
				if (enableShuffle)
				{
					printf("Compression stage: Shuffle\n");
					size_t choiceIndex;
					for (choiceIndex = 0 ; choiceIndex < comp3.mChoicesPos.size() ; /*Deliberate no incr*/)
					{
						printf("Considering pos %d/%d with len %d exceptions %d\r",choiceIndex,comp3.mChoicesPos.size(),outSize,comp3.mIgnoreChoicePos.size());
						CompressionE comp2;
						comp2.mIgnoreChoicePos = comp3.mIgnoreChoicePos;
						comp2.mIgnoreChoicePos.insert(comp3.mChoicesPos[choiceIndex]);
						u32 outSize2;
						u32 outBitSize2;
						comp2.Compress(input,inputSize,output,&outSize2,compressionLevel);
						outBitSize2 = comp2.mTotalBitsOut;
						if (outBitSize2 < outBitSize)
						{
							printf("\nGot size %d\n",outSize2);
							printf("\nFound %d bits at pos %d/%d\n",outBitSize - outBitSize2,choiceIndex,comp3.mChoicesPos.size());
							comp3.mIgnoreChoicePos = comp2.mIgnoreChoicePos;
							comp3.mChoicesPos = comp2.mChoicesPos;
							outBitSize = outBitSize2;
							outSize = outSize2;
							// loop and check again
							continue;
						}
						choiceIndex++;
					}

					printf("Compression stage: Final\n");
					// Final compress with the choices
					comp3.Compress(input,inputSize,output,&outSize,compressionLevel);
				}
			}
			else
			{
				printf("Compression stage: First attempt\n");
				Compression comp3;
				comp3.Compress(input,inputSize,output,&outSize,compressionLevel,1);
				outBitSize = comp3.mTotalBitsOut;

				// This tries to optimise the compression choices even more by optionally trying each
				// dictionary and literal choice one at a time to get the best output length.
				// It saves 149 bytes with C:\work\C64\CityGame\OriginalData.prg but takes ages.
				if (enableShuffle)
				{
					printf("Compression stage: Shuffle\n");
					size_t choiceIndex;
					for (choiceIndex = 0 ; choiceIndex < comp3.mChoicesPos.size() ; /*Deliberate no incr*/)
					{
						printf("Considering pos %d/%d with len %d exceptions %d\r",choiceIndex,comp3.mChoicesPos.size(),outSize,comp3.mIgnoreChoicePos.size());
						Compression comp2;
						comp2.mIgnoreChoicePos = comp3.mIgnoreChoicePos;
						comp2.mIgnoreChoicePos.insert(comp3.mChoicesPos[choiceIndex]);
						u32 outSize2;
						u32 outBitSize2;
						comp2.Compress(input,inputSize,output,&outSize2,compressionLevel,1);
						outBitSize2 = comp2.mTotalBitsOut;
						if (outBitSize2 < outBitSize)
						{
							printf("\nGot size %d\n",outSize2);
							printf("\nFound %d bits at pos %d/%d\n",outBitSize - outBitSize2,choiceIndex,comp3.mChoicesPos.size());
							comp3.mIgnoreChoicePos = comp2.mIgnoreChoicePos;
							comp3.mChoicesPos = comp2.mChoicesPos;
							outBitSize = outBitSize2;
							outSize = outSize2;
							// loop and check again
							continue;
						}
						choiceIndex++;
					}

					printf("Compression stage: Final\n");
					// Final compress with the choices
					comp3.mEarlyOut = -1;
					comp3.mEnableChoicesOutput = true;
					comp3.Compress(input,inputSize,output,&outSize,compressionLevel,1);
				}
			}
		}

#else
		comp.Compress(input,inputSize,output,&outSize,compressionLevel,0);
		int i;
		for (i = 1; i < 4 ; i++)
		{
			u32 tSize;
			printf("Trying optimise %d... ",i);
			comp.Compress(input,inputSize,output,&tSize,compressionLevel,i);
			if (tSize < outSize)
			{
				printf("Better!\n",i);
				bestBits = i;
				outSize = tSize;
			}
			else
			{
				printf("\n",i);
			}
		}
		comp.Compress(input,inputSize,output,&outSize,compressionLevel,bestBits);
#endif

		if (outputC64Header)
		{
			fputc(1,fp);
			fputc(8,fp);
			u8 *theC64Code;
			u32 endOfMemory;
			size_t sizeToWrite;

			// Update the offsets in the code header to save out
			// As luck would have it the two bits of code have the same offsets early on, it is
			// only later in the code with the extra border update code that produces different code.

			// Check our assumptions about addresses are correct
#if sC64DecompNoEffect_LauncherAddress_startC64Code != sC64DecompBorderEffect_LauncherAddress_startC64Code
#error sC64DecompNoEffect_LauncherAddress_startC64Code
#endif
#if sC64DecompNoEffect_LauncherAddress_endMinusOutSize != sC64DecompBorderEffect_LauncherAddress_endMinusOutSize
#error sC64DecompNoEffect_LauncherAddress_endMinusOutSize
#endif
#if sC64DecompNoEffect_LauncherAddress_compressedDataEndMinus256 != sC64DecompBorderEffect_LauncherAddress_compressedDataEndMinus256
#error sC64DecompNoEffect_LauncherAddress_compressedDataEndMinus256
#endif
#if sC64DecompNoEffect_LauncherAddress_endOfMemoryMinus256 != sC64DecompBorderEffect_LauncherAddress_endOfMemoryMinus256
#error sC64DecompNoEffect_LauncherAddress_endOfMemoryMinus256
#endif
#if sC64DecompNoEffect_LauncherAddress_loadC64Code != sC64DecompBorderEffect_LauncherAddress_loadC64Code
#error sC64DecompNoEffect_LauncherAddress_loadC64Code
#endif

#if sC64DecompNoEffectMax_LauncherAddress_startC64Code != sC64DecompBorderEffectMax_LauncherAddress_startC64Code
#error sC64DecompNoEffectMax_LauncherAddress_startC64Code
#endif
#if sC64DecompNoEffectMax_LauncherAddress_endMinusOutSize != sC64DecompBorderEffectMax_LauncherAddress_endMinusOutSize
#error sC64DecompNoEffectMax_LauncherAddress_endMinusOutSize
#endif
#if sC64DecompNoEffectMax_LauncherAddress_compressedDataEndMinus256 != sC64DecompBorderEffectMax_LauncherAddress_compressedDataEndMinus256
#error sC64DecompNoEffectMax_LauncherAddress_compressedDataEndMinus256
#endif
#if sC64DecompNoEffectMax_LauncherAddress_endOfMemoryMinus256 != sC64DecompBorderEffectMax_LauncherAddress_endOfMemoryMinus256
#error sC64DecompNoEffectMax_LauncherAddress_endOfMemoryMinus256
#endif
#if sC64DecompNoEffectMax_LauncherAddress_loadC64Code != sC64DecompBorderEffectMax_LauncherAddress_loadC64Code
#error sC64DecompNoEffectMax_LauncherAddress_loadC64Code
#endif

#if sC64DecompNoEffectRNZip_LauncherAddress_startC64Code != sC64DecompBorderEffectRNZip_LauncherAddress_startC64Code
#error sC64DecompNoEffectRNZip_LauncherAddress_startC64Code
#endif
#if sC64DecompNoEffectRNZip_LauncherAddress_endMinusOutSize != sC64DecompBorderEffectRNZip_LauncherAddress_endMinusOutSize
#error sC64DecompNoEffectRNZip_LauncherAddress_endMinusOutSize
#endif
#if sC64DecompNoEffectRNZip_LauncherAddress_compressedDataEndMinus256 != sC64DecompBorderEffectRNZip_LauncherAddress_compressedDataEndMinus256
#error sC64DecompNoEffectRNZip_LauncherAddress_compressedDataEndMinus256
#endif
#if sC64DecompNoEffectRNZip_LauncherAddress_endOfMemoryMinus256 != sC64DecompBorderEffectRNZip_LauncherAddress_endOfMemoryMinus256
#error sC64DecompNoEffectRNZip_LauncherAddress_endOfMemoryMinus256
#endif
#if sC64DecompNoEffectRNZip_LauncherAddress_loadC64Code != sC64DecompBorderEffectRNZip_LauncherAddress_loadC64Code
#error sC64DecompNoEffectRNZip_LauncherAddress_loadC64Code
#endif

#if sC64DecompNoEffectMaxRNZip_LauncherAddress_startC64Code != sC64DecompBorderEffectMaxRNZip_LauncherAddress_startC64Code
#error sC64DecompNoEffectMaxRNZip_LauncherAddress_startC64Code
#endif
#if sC64DecompNoEffectMaxRNZip_LauncherAddress_endMinusOutSize != sC64DecompBorderEffectMaxRNZip_LauncherAddress_endMinusOutSize
#error sC64DecompNoEffectMaxRNZip_LauncherAddress_endMinusOutSize
#endif
#if sC64DecompNoEffectMaxRNZip_LauncherAddress_compressedDataEndMinus256 != sC64DecompBorderEffectMaxRNZip_LauncherAddress_compressedDataEndMinus256
#error sC64DecompNoEffectMaxRNZip_LauncherAddress_compressedDataEndMinus256
#endif
#if sC64DecompNoEffectMaxRNZip_LauncherAddress_endOfMemoryMinus256 != sC64DecompBorderEffectMaxRNZip_LauncherAddress_endOfMemoryMinus256
#error sC64DecompNoEffectMaxRNZip_LauncherAddress_endOfMemoryMinus256
#endif
#if sC64DecompNoEffectMaxRNZip_LauncherAddress_loadC64Code != sC64DecompBorderEffectMaxRNZip_LauncherAddress_loadC64Code
#error sC64DecompNoEffectMaxRNZip_LauncherAddress_loadC64Code
#endif

#if sC64DecompNoEffectMaxRLE_LauncherAddress_startC64Code != sC64DecompBorderEffectMaxRLE_LauncherAddress_startC64Code
#error sC64DecompNoEffectMaxRLE_LauncherAddress_startC64Code
#endif
#if sC64DecompNoEffectMaxRLE_LauncherAddress_endMinusOutSize != sC64DecompBorderEffectMaxRLE_LauncherAddress_endMinusOutSize
#error sC64DecompNoEffectMaxRLE_LauncherAddress_endMinusOutSize
#endif
#if sC64DecompNoEffectMaxRLE_LauncherAddress_compressedDataEndMinus256 != sC64DecompBorderEffectMaxRLE_LauncherAddress_compressedDataEndMinus256
#error sC64DecompNoEffectMaxRLE_LauncherAddress_compressedDataEndMinus256
#endif
#if sC64DecompNoEffectMaxRLE_LauncherAddress_endOfMemoryMinus256 != sC64DecompBorderEffectMaxRLE_LauncherAddress_endOfMemoryMinus256
#error sC64DecompNoEffectMaxRLE_LauncherAddress_endOfMemoryMinus256
#endif
#if sC64DecompNoEffectMaxRLE_LauncherAddress_loadC64Code != sC64DecompBorderEffectMaxRLE_LauncherAddress_loadC64Code
#error sC64DecompNoEffectMaxRLE_LauncherAddress_loadC64Code
#endif

#if sC64DecompNoEffectMaxRNZipU_LauncherAddress_startC64Code != sC64DecompBorderEffectMaxRNZipU_LauncherAddress_startC64Code
#error sC64DecompNoEffectMaxRNZipU_LauncherAddress_startC64Code
#endif
#if sC64DecompNoEffectMaxRNZipU_LauncherAddress_endMinusOutSize != sC64DecompBorderEffectMaxRNZipU_LauncherAddress_endMinusOutSize
#error sC64DecompNoEffectMaxRNZipU_LauncherAddress_endMinusOutSize
#endif
#if sC64DecompNoEffectMaxRNZipU_LauncherAddress_compressedDataEndMinus256 != sC64DecompBorderEffectMaxRNZipU_LauncherAddress_compressedDataEndMinus256
#error sC64DecompNoEffectMaxRNZipU_LauncherAddress_compressedDataEndMinus256
#endif
#if sC64DecompNoEffectMaxRNZipU_LauncherAddress_endOfMemoryMinus256 != sC64DecompBorderEffectMaxRNZipU_LauncherAddress_endOfMemoryMinus256
#error sC64DecompNoEffectMaxRNZipU_LauncherAddress_endOfMemoryMinus256
#endif
#if sC64DecompNoEffectMaxRNZipU_LauncherAddress_loadC64Code != sC64DecompBorderEffectMaxRNZipU_LauncherAddress_loadC64Code
#error sC64DecompNoEffectMaxRNZipU_LauncherAddress_loadC64Code
#endif

// Check the low and high variants of this are the same
#if sC64DecompNoEffectMaxRNZipV_LauncherAddress_startC64Code != sC64DecompNoEffectMaxRNZipVH_LauncherAddress_startC64Code
#error sC64DecompNoEffectMaxRNZipV_LauncherAddress_startC64Code
#endif
#if sC64DecompNoEffectMaxRNZipV_LauncherAddress_endMinusOutSize != sC64DecompNoEffectMaxRNZipVH_LauncherAddress_endMinusOutSize
#error sC64DecompNoEffectMaxRNZipV_LauncherAddress_endMinusOutSize
#endif
#if sC64DecompNoEffectMaxRNZipV_LauncherAddress_compressedDataEndMinus256 != sC64DecompNoEffectMaxRNZipVH_LauncherAddress_compressedDataEndMinus256
#error sC64DecompNoEffectMaxRNZipV_LauncherAddress_compressedDataEndMinus256
#endif
#if sC64DecompNoEffectMaxRNZipV_LauncherAddress_endOfMemoryMinus256 != sC64DecompNoEffectMaxRNZipVH_LauncherAddress_endOfMemoryMinus256
#error sC64DecompNoEffectMaxRNZipV_LauncherAddress_endOfMemoryMinus256
#endif
#if sC64DecompNoEffectMaxRNZipV_LauncherAddress_loadC64Code != sC64DecompNoEffectMaxRNZipVH_LauncherAddress_loadC64Code
#error sC64DecompNoEffectMaxRNZipV_LauncherAddress_loadC64Code
#endif

#if sC64DecompBorderEffectMaxRNZipV_LauncherAddress_startC64Code != sC64DecompBorderEffectMaxRNZipVH_LauncherAddress_startC64Code
#error sC64DecompBorderEffectMaxRNZipV_LauncherAddress_startC64Code
#endif
#if sC64DecompBorderEffectMaxRNZipV_LauncherAddress_endMinusOutSize != sC64DecompBorderEffectMaxRNZipVH_LauncherAddress_endMinusOutSize
#error sC64DecompBorderEffectMaxRNZipV_LauncherAddress_endMinusOutSize
#endif
#if sC64DecompBorderEffectMaxRNZipV_LauncherAddress_compressedDataEndMinus256 != sC64DecompBorderEffectMaxRNZipVH_LauncherAddress_compressedDataEndMinus256
#error sC64DecompBorderEffectMaxRNZipV_LauncherAddress_compressedDataEndMinus256
#endif
#if sC64DecompBorderEffectMaxRNZipV_LauncherAddress_endOfMemoryMinus256 != sC64DecompBorderEffectMaxRNZipVH_LauncherAddress_endOfMemoryMinus256
#error sC64DecompBorderEffectMaxRNZipV_LauncherAddress_endOfMemoryMinus256
#endif
#if sC64DecompBorderEffectMaxRNZipV_LauncherAddress_loadC64Code != sC64DecompBorderEffectMaxRNZipVH_LauncherAddress_loadC64Code
#error sC64DecompBorderEffectMaxRNZipV_LauncherAddress_loadC64Code
#endif

			// The commented values are for the super expanded decompression code that makes $200-$fff9 available
			if (maxMode && useRLE)
			{
				outSize += 2;	//	For the 2 byte original size header

				theC64Code = sC64DecompNoEffectMaxRLE_Data;
				endOfMemory = sStartOfBASIC + sizeof(sC64DecompNoEffectMaxRLE_Data) + outSize;
				sizeToWrite = sizeof(sC64DecompNoEffectMaxRLE_Data);

				if (flashBorder)
				{
					theC64Code = sC64DecompBorderEffectMaxRLE_Data;
					endOfMemory = sStartOfBASIC + sizeof(sC64DecompBorderEffectMaxRLE_Data) + outSize;
					sizeToWrite = sizeof(sC64DecompBorderEffectMaxRLE_Data);
				}


				theC64Code[sC64DecompNoEffectMaxRLE_LauncherAddress_startC64Code - sStartOfBASIC] = (u8) (startC64Code & 0xff);
				theC64Code[sC64DecompNoEffectMaxRLE_LauncherAddress_startC64Code+1 - sStartOfBASIC] = (u8) ((startC64Code>>8) & 0xff);

				theC64Code[sC64DecompNoEffectMaxRLE_LauncherAddress_compressedDataEndMinus256 - sStartOfBASIC] = (u8) ((endOfMemory-0x100) & 0xff);
				theC64Code[sC64DecompNoEffectMaxRLE_LauncherAddress_compressedDataEndMinus256+1 - sStartOfBASIC] = (u8) (((endOfMemory-0x100)>>8) & 0xff);

				theC64Code[sC64DecompNoEffectMaxRLE_LauncherAddress_endMinusOutSize - sStartOfBASIC] = (u8) ((machineMemoryTop - outSize) & 0xff);
				theC64Code[sC64DecompNoEffectMaxRLE_LauncherAddress_endMinusOutSize+1 - sStartOfBASIC] = (u8) (((machineMemoryTop - outSize)>>8) & 0xff);

				theC64Code[sC64DecompNoEffectMaxRLE_LauncherAddress_endOfMemoryMinus256 - sStartOfBASIC] = (u8) (machineMemoryTopMinus256 & 0xff);
				theC64Code[sC64DecompNoEffectMaxRLE_LauncherAddress_endOfMemoryMinus256+1 - sStartOfBASIC] = (u8) ((machineMemoryTopMinus256>>8) & 0xff);

				theC64Code[sC64DecompNoEffectMaxRLE_LauncherAddress_loadC64Code - sStartOfBASIC] = (u8) (loadC64Code & 0xff);
				theC64Code[sC64DecompNoEffectMaxRLE_LauncherAddress_loadC64Code+1 - sStartOfBASIC] = (u8) ((loadC64Code>>8) & 0xff);

				outSize -= 2;	//	For the 2 byte original size header
			}
			else if (!maxMode && useRLE)
			{
				printf("!!RLE and non-max mode not supported!!\n");
				exit(-1);
			}
			else if (useCompressionU && maxMode)
			{
				theC64Code = sC64DecompNoEffectMaxRNZipU_Data;
				endOfMemory = sStartOfBASIC + sizeof(sC64DecompNoEffectMaxRNZipU_Data) + outSize;
				sizeToWrite = sizeof(sC64DecompNoEffectMaxRNZipU_Data);
				if (flashBorder)
				{
					theC64Code = sC64DecompBorderEffectMaxRNZipU_Data;
					endOfMemory = sStartOfBASIC + sizeof(sC64DecompBorderEffectMaxRNZipU_Data) + outSize;
					sizeToWrite = sizeof(sC64DecompBorderEffectMaxRNZipU_Data);
				}

				theC64Code[sC64DecompNoEffectMaxRNZipU_LauncherAddress_startC64Code - sStartOfBASIC] = (u8) (startC64Code & 0xff);
				theC64Code[sC64DecompNoEffectMaxRNZipU_LauncherAddress_startC64Code+1 - sStartOfBASIC] = (u8) ((startC64Code>>8) & 0xff);

				theC64Code[sC64DecompNoEffectMaxRNZipU_LauncherAddress_compressedDataEndMinus256 - sStartOfBASIC] = (u8) ((endOfMemory-0x100) & 0xff);
				theC64Code[sC64DecompNoEffectMaxRNZipU_LauncherAddress_compressedDataEndMinus256+1 - sStartOfBASIC] = (u8) (((endOfMemory-0x100)>>8) & 0xff);

				theC64Code[sC64DecompNoEffectMaxRNZipU_LauncherAddress_endMinusOutSize - sStartOfBASIC] = (u8) ((machineMemoryTop - outSize) & 0xff);
				theC64Code[sC64DecompNoEffectMaxRNZipU_LauncherAddress_endMinusOutSize+1 - sStartOfBASIC] = (u8) (((machineMemoryTop - outSize)>>8) & 0xff);

				theC64Code[sC64DecompNoEffectMaxRNZipU_LauncherAddress_endOfMemoryMinus256 - sStartOfBASIC] = (u8) (machineMemoryTopMinus256 & 0xff);
				theC64Code[sC64DecompNoEffectMaxRNZipU_LauncherAddress_endOfMemoryMinus256+1 - sStartOfBASIC] = (u8) ((machineMemoryTopMinus256>>8) & 0xff);

				theC64Code[sC64DecompNoEffectMaxRNZipU_LauncherAddress_loadC64Code - sStartOfBASIC] = (u8) (loadC64Code & 0xff);
				theC64Code[sC64DecompNoEffectMaxRNZipU_LauncherAddress_loadC64Code+1 - sStartOfBASIC] = (u8) ((loadC64Code>>8) & 0xff);
			}
			else if (useCompressionV && !maxMode && !flashBorder)
			{
				if (!useCompressionWithHigh && loadC64Code < 0x800)
				{
					printf("!!CompressionV does not support load addresses less than $0800!!\n");
					exit(-1);
				}

				theC64Code = sC64DecompNoEffectRNZipV_Data;
				if (useCompressionWithHigh)
				{
					theC64Code = sC64DecompNoEffectRNZipVH_Data;
				}
				endOfMemory = sStartOfBASIC + sizeof(sC64DecompNoEffectRNZipV_Data) + outSize;
				sizeToWrite = sizeof(sC64DecompNoEffectRNZipV_Data);

				theC64Code[sC64DecompNoEffectRNZipV_LauncherAddress_startC64Code - sStartOfBASIC] = (u8) (startC64Code & 0xff);
				theC64Code[sC64DecompNoEffectRNZipV_LauncherAddress_startC64Code+1 - sStartOfBASIC] = (u8) ((startC64Code>>8) & 0xff);

				theC64Code[sC64DecompNoEffectRNZipV_LauncherAddress_compressedDataEndMinus256 - sStartOfBASIC] = (u8) ((endOfMemory-0x100) & 0xff);
				theC64Code[sC64DecompNoEffectRNZipV_LauncherAddress_compressedDataEndMinus256+1 - sStartOfBASIC] = (u8) (((endOfMemory-0x100)>>8) & 0xff);

				theC64Code[sC64DecompNoEffectRNZipV_LauncherAddress_endMinusOutSize - sStartOfBASIC] = (u8) ((machineMemoryTop - outSize) & 0xff);
				theC64Code[sC64DecompNoEffectRNZipV_LauncherAddress_endMinusOutSize+1 - sStartOfBASIC] = (u8) (((machineMemoryTop - outSize)>>8) & 0xff);

				theC64Code[sC64DecompNoEffectRNZipV_LauncherAddress_endOfMemoryMinus256 - sStartOfBASIC] = (u8) (machineMemoryTopMinus256 & 0xff);
				theC64Code[sC64DecompNoEffectRNZipV_LauncherAddress_endOfMemoryMinus256+1 - sStartOfBASIC] = (u8) ((machineMemoryTopMinus256>>8) & 0xff);

				theC64Code[sC64DecompNoEffectRNZipV_LauncherAddress_loadC64Code - sStartOfBASIC] = (u8) (loadC64Code & 0xff);
				theC64Code[sC64DecompNoEffectRNZipV_LauncherAddress_loadC64Code+1 - sStartOfBASIC] = (u8) ((loadC64Code>>8) & 0xff);
			}
			else if (useCompressionV && !maxMode && flashBorder)
			{
				if (!useCompressionWithHigh && loadC64Code < 0x800)
				{
					printf("!!CompressionV does not support load addresses less than $0800!!\n");
					exit(-1);
				}

				theC64Code = sC64DecompBorderEffectRNZipV_Data;
				if (useCompressionWithHigh)
				{
					theC64Code = sC64DecompBorderEffectRNZipVH_Data;
				}
				endOfMemory = sStartOfBASIC + sizeof(sC64DecompBorderEffectRNZipV_Data) + outSize;
				sizeToWrite = sizeof(sC64DecompBorderEffectRNZipV_Data);

				theC64Code[sC64DecompBorderEffectRNZipV_LauncherAddress_startC64Code - sStartOfBASIC] = (u8) (startC64Code & 0xff);
				theC64Code[sC64DecompBorderEffectRNZipV_LauncherAddress_startC64Code+1 - sStartOfBASIC] = (u8) ((startC64Code>>8) & 0xff);

				theC64Code[sC64DecompBorderEffectRNZipV_LauncherAddress_compressedDataEndMinus256 - sStartOfBASIC] = (u8) ((endOfMemory-0x100) & 0xff);
				theC64Code[sC64DecompBorderEffectRNZipV_LauncherAddress_compressedDataEndMinus256+1 - sStartOfBASIC] = (u8) (((endOfMemory-0x100)>>8) & 0xff);

				theC64Code[sC64DecompBorderEffectRNZipV_LauncherAddress_endMinusOutSize - sStartOfBASIC] = (u8) ((machineMemoryTop - outSize) & 0xff);
				theC64Code[sC64DecompBorderEffectRNZipV_LauncherAddress_endMinusOutSize+1 - sStartOfBASIC] = (u8) (((machineMemoryTop - outSize)>>8) & 0xff);

				theC64Code[sC64DecompBorderEffectRNZipV_LauncherAddress_endOfMemoryMinus256 - sStartOfBASIC] = (u8) (machineMemoryTopMinus256 & 0xff);
				theC64Code[sC64DecompBorderEffectRNZipV_LauncherAddress_endOfMemoryMinus256+1 - sStartOfBASIC] = (u8) ((machineMemoryTopMinus256>>8) & 0xff);

				theC64Code[sC64DecompBorderEffectRNZipV_LauncherAddress_loadC64Code - sStartOfBASIC] = (u8) (loadC64Code & 0xff);
				theC64Code[sC64DecompBorderEffectRNZipV_LauncherAddress_loadC64Code+1 - sStartOfBASIC] = (u8) ((loadC64Code>>8) & 0xff);
			}
			else if (useCompressionV && maxMode)
			{
				printf("!!CompressionV max mode not supported!!\n");
				exit(-1);
			}
			else if (!maxMode && useCompressionU)
			{
				printf("!!CompressionU and non-max mode not supported!!\n");
				exit(-1);
			}
			else if (useRNZipMode && maxMode)
			{
				theC64Code = sC64DecompNoEffectMaxRNZip_Data;
				endOfMemory = sStartOfBASIC + sizeof(sC64DecompNoEffectMaxRNZip_Data) + outSize;
				sizeToWrite = sizeof(sC64DecompNoEffectMaxRNZip_Data);
				if (flashBorder)
				{
					theC64Code = sC64DecompBorderEffectMaxRNZip_Data;
					endOfMemory = sStartOfBASIC + sizeof(sC64DecompBorderEffectMaxRNZip_Data) + outSize;
					sizeToWrite = sizeof(sC64DecompBorderEffectMaxRNZip_Data);
				}

				theC64Code[sC64DecompNoEffectMaxRNZip_LauncherAddress_startC64Code - sStartOfBASIC] = (u8) (startC64Code & 0xff);
				theC64Code[sC64DecompNoEffectMaxRNZip_LauncherAddress_startC64Code+1 - sStartOfBASIC] = (u8) ((startC64Code>>8) & 0xff);

				theC64Code[sC64DecompNoEffectMaxRNZip_LauncherAddress_compressedDataEndMinus256 - sStartOfBASIC] = (u8) ((endOfMemory-0x100) & 0xff);
				theC64Code[sC64DecompNoEffectMaxRNZip_LauncherAddress_compressedDataEndMinus256+1 - sStartOfBASIC] = (u8) (((endOfMemory-0x100)>>8) & 0xff);

				theC64Code[sC64DecompNoEffectMaxRNZip_LauncherAddress_endMinusOutSize - sStartOfBASIC] = (u8) ((machineMemoryTop - outSize) & 0xff);
				theC64Code[sC64DecompNoEffectMaxRNZip_LauncherAddress_endMinusOutSize+1 - sStartOfBASIC] = (u8) (((machineMemoryTop - outSize)>>8) & 0xff);

				theC64Code[sC64DecompNoEffectMaxRNZip_LauncherAddress_endOfMemoryMinus256 - sStartOfBASIC] = (u8) (machineMemoryTopMinus256 & 0xff);
				theC64Code[sC64DecompNoEffectMaxRNZip_LauncherAddress_endOfMemoryMinus256+1 - sStartOfBASIC] = (u8) ((machineMemoryTopMinus256>>8) & 0xff);

				theC64Code[sC64DecompNoEffectMaxRNZip_LauncherAddress_loadC64Code - sStartOfBASIC] = (u8) (loadC64Code & 0xff);
				theC64Code[sC64DecompNoEffectMaxRNZip_LauncherAddress_loadC64Code+1 - sStartOfBASIC] = (u8) ((loadC64Code>>8) & 0xff);
			}
			else if (maxMode)
			{
				theC64Code = sC64DecompNoEffectMax_Data;
				endOfMemory = sStartOfBASIC + sizeof(sC64DecompNoEffectMax_Data) + outSize;
				sizeToWrite = sizeof(sC64DecompNoEffectMax_Data);
				if (flashBorder)
				{
					theC64Code = sC64DecompBorderEffectMax_Data;
					endOfMemory = sStartOfBASIC + sizeof(sC64DecompBorderEffectMax_Data) + outSize;
					sizeToWrite = sizeof(sC64DecompBorderEffectMax_Data);
				}

				theC64Code[sC64DecompNoEffectMax_LauncherAddress_startC64Code - sStartOfBASIC] = (u8) (startC64Code & 0xff);
				theC64Code[sC64DecompNoEffectMax_LauncherAddress_startC64Code+1 - sStartOfBASIC] = (u8) ((startC64Code>>8) & 0xff);

				theC64Code[sC64DecompNoEffectMax_LauncherAddress_compressedDataEndMinus256 - sStartOfBASIC] = (u8) ((endOfMemory-0x100) & 0xff);
				theC64Code[sC64DecompNoEffectMax_LauncherAddress_compressedDataEndMinus256+1 - sStartOfBASIC] = (u8) (((endOfMemory-0x100)>>8) & 0xff);

				theC64Code[sC64DecompNoEffectMax_LauncherAddress_endMinusOutSize - sStartOfBASIC] = (u8) ((machineMemoryTop - outSize) & 0xff);
				theC64Code[sC64DecompNoEffectMax_LauncherAddress_endMinusOutSize+1 - sStartOfBASIC] = (u8) (((machineMemoryTop - outSize)>>8) & 0xff);

				theC64Code[sC64DecompNoEffectMax_LauncherAddress_endOfMemoryMinus256 - sStartOfBASIC] = (u8) (machineMemoryTopMinus256 & 0xff);
				theC64Code[sC64DecompNoEffectMax_LauncherAddress_endOfMemoryMinus256+1 - sStartOfBASIC] = (u8) ((machineMemoryTopMinus256>>8) & 0xff);

				theC64Code[sC64DecompNoEffectMax_LauncherAddress_loadC64Code - sStartOfBASIC] = (u8) (loadC64Code & 0xff);
				theC64Code[sC64DecompNoEffectMax_LauncherAddress_loadC64Code+1 - sStartOfBASIC] = (u8) ((loadC64Code>>8) & 0xff);
			}
			else if (useRNZipMode)
			{
				theC64Code = sC64DecompNoEffectRNZip_Data;
				endOfMemory = sStartOfBASIC + sizeof(sC64DecompNoEffectRNZip_Data) + outSize;
				sizeToWrite = sizeof(sC64DecompNoEffectRNZip_Data);
				if (flashBorder)
				{
					theC64Code = sC64DecompBorderEffectRNZip_Data;
					endOfMemory = sStartOfBASIC + sizeof(sC64DecompBorderEffectRNZip_Data) + outSize;
					sizeToWrite = sizeof(sC64DecompBorderEffectRNZip_Data);
				}

				theC64Code[sC64DecompNoEffectRNZip_LauncherAddress_startC64Code - sStartOfBASIC] = (u8) (startC64Code & 0xff);
				theC64Code[sC64DecompNoEffectRNZip_LauncherAddress_startC64Code+1 - sStartOfBASIC] = (u8) ((startC64Code>>8) & 0xff);

				theC64Code[sC64DecompNoEffectRNZip_LauncherAddress_compressedDataEndMinus256 - sStartOfBASIC] = (u8) ((endOfMemory-0x100) & 0xff);
				theC64Code[sC64DecompNoEffectRNZip_LauncherAddress_compressedDataEndMinus256+1 - sStartOfBASIC] = (u8) (((endOfMemory-0x100)>>8) & 0xff);

				theC64Code[sC64DecompNoEffectRNZip_LauncherAddress_endMinusOutSize - sStartOfBASIC] = (u8) ((machineMemoryTop - outSize) & 0xff);
				theC64Code[sC64DecompNoEffectRNZip_LauncherAddress_endMinusOutSize+1 - sStartOfBASIC] = (u8) (((machineMemoryTop - outSize)>>8) & 0xff);

				theC64Code[sC64DecompNoEffectRNZip_LauncherAddress_endOfMemoryMinus256 - sStartOfBASIC] = (u8) (machineMemoryTopMinus256 & 0xff);
				theC64Code[sC64DecompNoEffectRNZip_LauncherAddress_endOfMemoryMinus256+1 - sStartOfBASIC] = (u8) ((machineMemoryTopMinus256>>8) & 0xff);

				theC64Code[sC64DecompNoEffectRNZip_LauncherAddress_loadC64Code - sStartOfBASIC] = (u8) (loadC64Code & 0xff);
				theC64Code[sC64DecompNoEffectRNZip_LauncherAddress_loadC64Code+1 - sStartOfBASIC] = (u8) ((loadC64Code>>8) & 0xff);
			}
			else
			{
				theC64Code = sC64DecompNoEffect_Data;
				endOfMemory = sStartOfBASIC + sizeof(sC64DecompNoEffect_Data) + outSize;
				sizeToWrite = sizeof(sC64DecompNoEffect_Data);
				if (flashBorder)
				{
					theC64Code = sC64DecompBorderEffect_Data;
					endOfMemory = sStartOfBASIC + sizeof(sC64DecompBorderEffect_Data) + outSize;
					sizeToWrite = sizeof(sC64DecompBorderEffect_Data);
				}

				theC64Code[sC64DecompNoEffect_LauncherAddress_startC64Code - sStartOfBASIC] = (u8) (startC64Code & 0xff);
				theC64Code[sC64DecompNoEffect_LauncherAddress_startC64Code+1 - sStartOfBASIC] = (u8) ((startC64Code>>8) & 0xff);

				theC64Code[sC64DecompNoEffect_LauncherAddress_compressedDataEndMinus256 - sStartOfBASIC] = (u8) ((endOfMemory-0x100) & 0xff);
				theC64Code[sC64DecompNoEffect_LauncherAddress_compressedDataEndMinus256+1 - sStartOfBASIC] = (u8) (((endOfMemory-0x100)>>8) & 0xff);

				theC64Code[sC64DecompNoEffect_LauncherAddress_endMinusOutSize - sStartOfBASIC] = (u8) ((machineMemoryTop - outSize) & 0xff);
				theC64Code[sC64DecompNoEffect_LauncherAddress_endMinusOutSize+1 - sStartOfBASIC] = (u8) (((machineMemoryTop - outSize)>>8) & 0xff);

				theC64Code[sC64DecompNoEffect_LauncherAddress_endOfMemoryMinus256 - sStartOfBASIC] = (u8) (machineMemoryTopMinus256 & 0xff);
				theC64Code[sC64DecompNoEffect_LauncherAddress_endOfMemoryMinus256+1 - sStartOfBASIC] = (u8) ((machineMemoryTopMinus256>>8) & 0xff);

				theC64Code[sC64DecompNoEffect_LauncherAddress_loadC64Code - sStartOfBASIC] = (u8) (loadC64Code & 0xff);
				theC64Code[sC64DecompNoEffect_LauncherAddress_loadC64Code+1 - sStartOfBASIC] = (u8) ((loadC64Code>>8) & 0xff);
			}

			fwrite(theC64Code,1,sizeToWrite,fp);

			if (useRLE)
			{
				fwrite(&inputSize,1,sizeof(inputSize),fp);
			}
		}

		fwrite(output,1,outSize,fp);

		printf("Input length = $%04x (%d) output length = $%04x (%d)\n",(int)inputSize,(int)inputSize,(int)ftell(fp),(int)ftell(fp));
		fclose(fp);

		// Enable the following code to paranoia check the compression is OK
#if 0
		if (!useRLE && !useRNZipMode)
		{
			u8 temp[65536];
			u32 tempLen;
			if (Decompress(output,outSize,temp,&tempLen) || (tempLen != inputSize) || (memcmp(temp,input,tempLen) != 0))
			{
				printf("Problem during decompression\n");
				exit(-1);
			}
		}
#endif

	}

	if (argv[1][1] == 'd')
	{
		// Setup some defaults that can get overridden by the self decompressing header check
		u8 *compressedDataStart = input + sizeof(u16);
		u16 origSize = *(u16*)input;
		bool outputHeader = false;
		// Check for a compressed file with a known self decompressing header
		const size_t kLaterCodeOffset = 0x8f0 - sStartOfBASIC;
		if ((inputSize >= sizeof(sC64DecompNoEffect_Data)) && (memcmp(input+2+kLaterCodeOffset,sC64DecompNoEffect_Data+kLaterCodeOffset,sizeof(sC64DecompNoEffect_Data)-kLaterCodeOffset)==0))
		{
			printf("Detected: sC64DecompNoEffect_Data\n");
			compressedDataStart = input + 2 + sizeof(sC64DecompNoEffect_Data);
			outputHeader = true;
		}
		else if ((inputSize >= sizeof(sC64DecompBorderEffect_Data)) && (memcmp(input+2+kLaterCodeOffset,sC64DecompBorderEffect_Data+kLaterCodeOffset,sizeof(sC64DecompBorderEffect_Data)-kLaterCodeOffset)==0))
		{
			printf("Detected: sC64DecompBorderEffect_Data\n");
			compressedDataStart = input + 2 + sizeof(sC64DecompBorderEffect_Data);
			outputHeader = true;
		}
		if (outputHeader)
		{
			size_t loadAddress = *(u16*)(input + 2 + (0x83a - sStartOfBASIC));
			printf("Original load address = $%04x\n",loadAddress);
			fputc(loadAddress & 0xff,fp);
			fputc(loadAddress >> 8,fp);

			// Fix the inputSize in case the prg has extra crap added to the end of it
			inputSize = 65536 - (*(u16*)(input + 2 + (0x844 - sStartOfBASIC)));
		}
		else
		{
			inputSize -= compressedDataStart - input;
			printf("Input length = $%04x (%d) Original output length = $%04x (%d)\n",(int)inputSize,(int)inputSize,(int)origSize,(int)origSize);
		}


		u32 outDecomp;
		if (useCompressionV)
		{
			if (DecompressV(compressedDataStart,inputSize,output,&outDecomp))
			{
				printf("Problem during decompressionV\n");
				exit(-1);
			}
		}
		else if (useCompressionU)
		{
			if (DecompressU(compressedDataStart,inputSize,output,&outDecomp))
			{
				printf("Problem during decompressionU\n");
				exit(-1);
			}
		}
		else if (useRNZipMode)
		{
			if (DecompressE(compressedDataStart,inputSize,output,&outDecomp))
			{
				printf("Problem during decompressionE\n");
				exit(-1);
			}
		}
		else
		{
			if (Decompress(compressedDataStart,inputSize,output,&outDecomp))
			{
				printf("Problem during decompression\n");
				exit(-1);
			}
		}
		printf("Decompressed length = $%04x\n",outDecomp);

		fwrite(output,1,outDecomp,fp);
		fclose(fp);
	}


	return 0;
}
