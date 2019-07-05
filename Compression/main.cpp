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
#include "Decompress.h"
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

//-ce C:\work\C64\MusicEditorHelp\MusicEditorHelp.prg c:\temp\t.rnzip2
//c:\ReplicaNet\ReplicaNetPublic\RNZip\RNZip.exe -c C:\work\C64\MusicEditorHelp\MusicEditorHelp.prg c:\temp\t.rnzip
//Input length = 24372 output length = 14107

//34760
//19516 : c:\temp\exomizer.exe raw TestData\5.bin -o c:\temp\t.out
//20362 : ..\bin\LZMPi.exe -c TestData\5.bin c:\temp\t.bin
//846

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
			"To use LZMPiE mode (RNZip with simpler but faster compression) then use:"
			" -ce <input file> <outfile file> [offset from the start of the file]\n"
			"To use RLE mode then use:"
			" -cr <input file> <outfile file> [offset from the start of the file]\n"
			"To create a C64 self extracting binary use:\n"
			" -c64[m][b][r][e] <input file> <outfile file> <run address> [start address]\n\n"
			"-c64 will compress a C64 prg file with the start address being the first two\n"
			"bytes of the file. The optional start address will override the start address\n"
			"read from the first two bytes of the prg file.\n"
			" -c64b will cause the border to flash during decompression.\n"
			" -c64m or -c64mb will use max mode ($200-$ffff available) without or with border flashing.\n"
			" -c64mr or -c64mrb will use RLE max mode ($200-$ffff available) without or with border flashing.\n"
			"By default the border will not flash.\n\n"
			"Use the following to skip a number of bytes from the start of the file and\n"
			"set a length. If the length equals 0 the length is still set from the file: \n"
			" -c <input file> <outfile file> <offset> [length]\n\n"
			" -cut <input file> <outfile file> <offset> [length]\n"
			"-cut will not compress the file, meaning data is just effectively cut and written to the output file.\n"
			"-t <address>\n"
			" The default value is $10000. Must be first in the parameter list. This will set the top most memory address used by the decompression code when using the -c64 self extracting code. This is useful to segregate the top pages of memory so they can be used for saved data between file loads.\n"
		);
		exit(-1);
	}

	u8 input[65536];
	u8 output[73728];
	int offset = 0;
	int wantedLength = 0;
	bool outputC64Header = false;
	bool useRLE = false;
	u32 startC64Code = 0x900;
	u32 loadC64Code = 0;
	bool flashBorder = false;
	bool maxMode = false;
	bool useRNZipMode = false;


	if (argv[1][1] == 't')
	{
		machineMemoryTop = ParamToNum(argv[2]);
		argc-=2;
		argv+=2;
	}

	int machineMemoryTopMinus256 = machineMemoryTop - 0x100;


	if ((argv[1][1] == 'c') && (argv[1][2] == 'r'))
	{
		useRLE = true;
	}
	if ((argv[1][1] == 'c') && (argv[1][2] == 'e'))
	{
		useRNZipMode = true;
	}

	if ((argv[1][1] == 'c') && (argv[1][2] == '6') && (argv[1][3] == '4'))
	{
		if (argc >= 5)
		{
			std::string subOption(argv[1]+4);
			outputC64Header = true;
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

			startC64Code = ParamToNum(argv[4]);
			printf("Using run address $%04x\n",startC64Code);

			if (argc >= 6)
			{
				loadC64Code = ParamToNum(argv[5]);
				printf("Using load address $%04x\n",loadC64Code);
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

	FILE *fp;
	printf("Opening '%s' for reading...\n",argv[2]);
	fp = fopen(argv[2],"rb");
	if (!fp)
	{
		printf("Problem opening '%s' for reading\n",argv[2]);
		exit(-1);
	}

	if (outputC64Header)
	{
		if (!loadC64Code)
		{
			loadC64Code = fgetc(fp);
			loadC64Code |= fgetc(fp) << 8;
			printf("Read load address as $%04x\n",loadC64Code);
		}
		else
		{
			// Skip two bytes
			fseek(fp,2,SEEK_SET);
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
			fwrite(&inputSize,1,sizeof(inputSize),fp);
		}

		u32 outSize;
		u32 outBitSize;
		int bestBits = 0;
#if 1
		// For now we will always use one escape bit because it seems to consistently generate
		// the best results.
		if (useRLE)
		{
			TestRLEPack(input,inputSize,output,&outSize);
		}
		else
		{
#if 0
			if (useRNZipMode)
			{
				CompressionE comp;
				comp.Compress(input,inputSize,output,&outSize,10);
				outBitSize = outSize*8;
			}
			else
			{
				Compression comp;
				comp.Compress(input,inputSize,output,&outSize,10,1);
				outBitSize = comp.mTotalBitsOut;
			}

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
								ret = comp2.Compress(input,inputSize,output,&outSize,10);
								comp2mTotalBitsOut = outSize*8;
							}
							else
							{
								Compression comp2;
								comp2.mEarlyOut = outBitSize;
								ret = comp2.Compress(input,inputSize,output,&outSize,10,1);
								comp2mTotalBitsOut = comp2.mTotalBitsOut;
							}
							printf("Working %d : %d %d %d %d                \r",outBitSize - comp2mTotalBitsOut , a , b ,c , d);
							if (ret == -1)
							{
								continue;
							}

							if (comp2mTotalBitsOut < outBitSize)
							{
								printf("New best %d : %d %d %d %d                \n",outBitSize - comp2mTotalBitsOut , a , b ,c , d);
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
#endif
			if (useRNZipMode)
			{
				CompressionE comp3;
				comp3.Compress(input,inputSize,output,&outSize,10);
				outBitSize = outSize*8;
			}
			else
			{
				Compression comp3;
				comp3.Compress(input,inputSize,output,&outSize,10,1);
				outBitSize = comp3.mTotalBitsOut;
			}

			// This tries to optimise the compression choices even more by optionally trying each
			// dictionary and literal choice one at a time to get the best output length.
			// It saves 149 bytes with C:\work\C64\CityGame\OriginalData.prg but takes ages.
#if 0
			size_t choiceIndex;
			for (choiceIndex = 0 ; choiceIndex < comp3.mChoicesPos.size() ; /*Deliberate no incr*/)
			{
				printf("Considering pos %d/%d with len %d exceptions %d\r",choiceIndex,comp3.mChoicesPos.size(),outSize,comp3.mIgnoreChoicePos.size());
				Compression comp2;
				comp2.mIgnoreChoicePos = comp3.mIgnoreChoicePos;
				comp2.mIgnoreChoicePos.insert(comp3.mChoicesPos[choiceIndex]);
				u32 outSize2;
				u32 outBitSize2;
				comp2.Compress(input,inputSize,output,&outSize2,10,1);
				outBitSize2 = comp2.mTotalBitsOut;
	//			printf("   Got size %d\n",outSize2);
				if (outBitSize2 < outBitSize)
				{
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

			// Final compress with the choices
			comp3.mEarlyOut = -1;
			comp3.mEnableChoicesOutput = true;
			comp3.Compress(input,inputSize,output,&outSize,10,1);
#endif
		}

#else
		comp.Compress(input,inputSize,output,&outSize,10,0);
		int i;
		for (i = 1; i < 4 ; i++)
		{
			u32 tSize;
			printf("Trying optimise %d... ",i);
			comp.Compress(input,inputSize,output,&tSize,10,i);
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
		comp.Compress(input,inputSize,output,&outSize,10,bestBits);
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

			// The commented values are for the super expanded decompression code that makes $200-$fff available
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

		printf("Input length = $%04x output length = $%04x\n",(int)inputSize,(int)ftell(fp));
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
			printf("Input length = $%04x Original output length = $%04x\n",(int)inputSize,(int)origSize);
		}


		u32 outDecomp;
		if (Decompress(compressedDataStart,inputSize,output,&outDecomp))
		{
			printf("Problem during decompression\n");
			exit(-1);
		}
		printf("Decompressed length = $%04x\n",outDecomp);

		fwrite(output,1,outDecomp,fp);
		fclose(fp);
	}


	return 0;
}
