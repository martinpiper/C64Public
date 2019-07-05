// Assemble all the banks for the Berzerk Redux cartridge
// -n -b 0 -r ../BerzerkReduxCart/BerzerkReduxCart.prg -c 0 2 $ffff -w -r ../BerzerkRedux/BerzerkRedux.prg -b 1 -c 0 $0001 $ffff -w -b 2 -c 0 $2001 $ffff -w -b 3 -c 0 $4001 $ffff -w -b 4 -c 0 $6001 $ffff -w -b 5 -c 0 $8001 $ffff -w -b 6 -c 0 $a001 $ffff -w -b 7 -r ../BerzerkReduxCart/fw86muscomp.prg -c 0 2 $ffff -w -o test.crt
// EasyFlash example:
// -te -n -b 0 -r ../BerzerkReduxCart/BerzerkReduxCart.prg -c 0 2 $ffff -w -r ../BerzerkRedux/BerzerkRedux.prg -a $a000 -b 0 -c 0 $0001 $ffff -w -a $8000 -b 1 -c 0 $2001 $ffff -w -a $a000 -b 1 -c 0 $4001 $ffff -w -o testEasy.crt
// -te -n -a $8000 -b 4 -m C:\work\c64\AnimationBitmap\data\frm0*.del 16384 -o c:\temp\t.crt
// -te -n -a $8000 -b 4 -f 16384 18 C:\work\c64\AnimationBitmap\data\frm00000.del+47 C:\work\c64\AnimationBitmap\data\frm00001.del C:\work\c64\AnimationBitmap\data\frm00002.del C:\work\c64\AnimationBitmap\data\frm00003.del C:\work\c64\AnimationBitmap\data\frm00004.del C:\work\c64\AnimationBitmap\data\frm00005.del C:\work\c64\AnimationBitmap\data\frm00006.del C:\work\c64\AnimationBitmap\data\frm00007.del C:\work\c64\AnimationBitmap\data\frm00008.del C:\work\c64\AnimationBitmap\data\frm00009.del C:\work\c64\AnimationBitmap\data\frm00010.del C:\work\c64\AnimationBitmap\data\frm00011.del C:\work\c64\AnimationBitmap\data\frm00012.del C:\work\c64\AnimationBitmap\data\frm00013.del C:\work\c64\AnimationBitmap\data\frm00014.del C:\work\c64\AnimationBitmap\data\frm00015.del C:\work\c64\AnimationBitmap\data\frm00016.del C:\work\c64\AnimationBitmap\data\frm00017.del -o c:\temp\t.crt
// -tg -n -a $8000 -r c:\Downloads\ItsMagic2.bin -b 00 -c 0 $00000 $100000 -w -b 01 -c 0 $02000 $100000 -w -b 02 -c 0 $04000 $100000 -w -b 03 -c 0 $06000 $100000 -w -b 04 -c 0 $08000 $100000 -w -b 05 -c 0 $0a000 $100000 -w -b 06 -c 0 $0c000 $100000 -w -b 07 -c 0 $0e000 $100000 -w -b 08 -c 0 $10000 $100000 -w -b 09 -c 0 $12000 $100000 -w -b 10 -c 0 $14000 $100000 -w -b 11 -c 0 $16000 $100000 -w -b 12 -c 0 $18000 $100000 -w -b 13 -c 0 $1a000 $100000 -w -b 14 -c 0 $1c000 $100000 -w -b 15 -c 0 $1e000 $100000 -w -b 16 -c 0 $20000 $100000 -w -b 17 -c 0 $22000 $100000 -w -b 18 -c 0 $24000 $100000 -w -b 19 -c 0 $26000 $100000 -w -b 20 -c 0 $28000 $100000 -w -b 21 -c 0 $2a000 $100000 -w -b 22 -c 0 $2c000 $100000 -w -b 23 -c 0 $2e000 $100000 -w -b 24 -c 0 $30000 $100000 -w -b 25 -c 0 $32000 $100000 -w -b 26 -c 0 $34000 $100000 -w -b 27 -c 0 $36000 $100000 -w -b 28 -c 0 $38000 $100000 -w -b 29 -c 0 $3a000 $100000 -w -b 30 -c 0 $3c000 $100000 -w -b 31 -c 0 $3e000 $100000 -w -b 32 -c 0 $40000 $100000 -w -b 33 -c 0 $42000 $100000 -w -b 34 -c 0 $44000 $100000 -w -b 35 -c 0 $46000 $100000 -w -b 36 -c 0 $48000 $100000 -w -b 37 -c 0 $4a000 $100000 -w -b 38 -c 0 $4c000 $100000 -w -b 39 -c 0 $4e000 $100000 -w -b 40 -c 0 $50000 $100000 -w -b 41 -c 0 $52000 $100000 -w -b 42 -c 0 $54000 $100000 -w -b 43 -c 0 $56000 $100000 -w -b 44 -c 0 $58000 $100000 -w -b 45 -c 0 $5a000 $100000 -w -b 46 -c 0 $5c000 $100000 -w -b 47 -c 0 $5e000 $100000 -w -b 48 -c 0 $60000 $100000 -w -b 49 -c 0 $62000 $100000 -w -b 50 -c 0 $64000 $100000 -w -b 51 -c 0 $66000 $100000 -w -b 52 -c 0 $68000 $100000 -w -b 53 -c 0 $6a000 $100000 -w -b 54 -c 0 $6c000 $100000 -w -b 55 -c 0 $6e000 $100000 -w -b 56 -c 0 $70000 $100000 -w -b 57 -c 0 $72000 $100000 -w -b 58 -c 0 $74000 $100000 -w -b 59 -c 0 $76000 $100000 -w -b 60 -c 0 $78000 $100000 -w -b 61 -c 0 $7a000 $100000 -w -b 62 -c 0 $7c000 $100000 -w -b 63 -c 0 $7e000 $100000 -w -o c:\temp\t.crt
// In C:\work\c64\AnimationBitmap\
// -te -n -a $8000 -b 0 -r ..\Citadel2\Citadel2Cart.prg -c 0 2 $ffff -w -a $a000 -b 0 -c $1ffc 2 4 -w -r AnimationBitmap.prg -a $8000 -b 1 -c 0 $0001 $ffff -w -a $a000 -b 1 -c 0 $2001 $ffff -w -a $8000 -b 2 -c 0 $4001 $ffff -w -a $a000 -b 2 -c 0 $6001 $ffff -w -a $8000 -b 3 -c 0 $8001 $ffff -w -a $a000 -b 3 -c 0 $a001 $ffff -w -a $8000 -b 4 -m data\frm0*.del $4000 -o AnimationBitmap.crt
// -te -n -a $8000 -b 0 -r ..\Citadel2\Citadel2Cart_8K.prg -c 0 2 $ffff -w -a $a000 -b 0 -c $1ffc 2 4 -w -r AnimationBitmap.prg -a $8000 -b 1 -c 0 $0001 $ffff -w -a $8000 -b 2 -c 0 $2001 $ffff -w -a $8000 -b 3 -c 0 $4001 $ffff -w -a $8000 -b 4 -c 0 $6001 $ffff -w -a $8000 -b 5 -c 0 $8001 $ffff -w -a $8000 -b 6 -c 0 $a001 $ffff -w -a $8000 -b 7 -m data\frm0*.del $2000 -o AnimationBitmap.crt
// -tg -n -a $8000 -b 0 -r ..\Citadel2\Citadel2Cart_8K.prg -c 0 2 $ffff -w -a $8000 -r AnimationBitmap.prg -a $8000 -b 1 -c 0 $0001 $ffff -w -a $8000 -b 2 -c 0 $2001 $ffff -w -a $8000 -b 3 -c 0 $4001 $ffff -w -a $8000 -b 4 -c 0 $6001 $ffff -w -a $8000 -b 5 -c 0 $8001 $ffff -w -a $8000 -b 6 -c 0 $a001 $ffff -w -a $8000 -b 22 -m data\frm0*.del $2000 -o AnimationBitmap.crt
// -tg -n -a $8000 -b 0 -r ..\Citadel2\Citadel2Cart_8K.prg -c 0 2 $ffff -w -a $8000 -r AnimationBitmap.prg -a $8000 -b 1 -c 0 $0001 $ffff -w -a $8000 -b 2 -c 0 $2001 $ffff -w -a $8000 -b 3 -c 0 $4001 $ffff -w -a $8000 -b 4 -c 0 $6001 $ffff -w -a $8000 -b 5 -c 0 $8001 $ffff -w -a $8000 -b 6 -c 0 $a001 $ffff -w -a $8000 -b 22 -l 32 $800 2 ..\bin\LZMPi.exe "-ce " " " -m data\frm0*.del $2000 -lc -o AnimationBitmap.crt
// Dragon's lair (Build2_11.bat)
// -i _f_index1.a -te -n -a $8000 -b 0 -r ..\Citadel2\Citadel2Cart.prg -c 0 2 $ffff -w -a $a000 -b 0 -c $1ffc 2 4 -w -r AnimationBitmap.prg -a $8000 -b 1 -c 0 $0001 $ffff -w -a $a000 -b 1 -c 0 $2001 $ffff -w -a $8000 -b 2 -c 0 $4001 $ffff -w -a $a000 -b 2 -c 0 $6001 $ffff -w -a $8000 -b 3 -c 0 $8001 $ffff -w -a $a000 -b 3 -c 0 $a001 $ffff -w -a $8000 -b 4 -m data\frm0*.del $4000 -o AnimationBitmap.crt
// -i _f_index1.a -te -n -a $8000 -b 0 -r ..\Citadel2\Citadel2Cart.prg -c 0 2 $ffff -w -a $a000 -b 0 -c $1ffc 2 4 -w -r AnimationBitmap.prg -a $8000 -b 1 -c 0 $0001 $ffff -w -a $a000 -b 1 -c 0 $2001 $ffff -w -a $8000 -b 2 -c 0 $4001 $ffff -w -a $a000 -b 2 -c 0 $6001 $ffff -w -a $8000 -b 3 -c 0 $8001 $ffff -w -a $a000 -b 3 -c 0 $a001 $ffff -w -a $8000 -b 4 -m data\frm0*.del $4000 s:data\smp_*.raw -o AnimationBitmap.crt

// Reference:
// https://ist.uwaterloo.ca/~schepers/formats/CRT.TXT
// http://ar.c64.org/wiki/CRT_ID
// https://github.com/svn2github/vice-emu/blob/master/vice/src/cartridge.h
// https://github.com/svn2github/vice-emu/blob/ea98cee1eaa42d5a8611d50a4b406db67ce53374/vice/src/c64/cart/crt.c

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <set>
#include <vector>
#include <algorithm>
#include "../Common/ParamToNum.h"
#include "RNPlatform/Inc/MessageHelper.h"
#include "RNLobby/Inc/ScanPath.h"

using namespace RNReplicaNet;
using namespace RNLobby;

static unsigned char sCartHeader_64K[] = {
	0x43, 0x36, 0x34, 0x20, 0x43, 0x41, 0x52, 0x54,   0x52, 0x49, 0x44, 0x47, 0x45, 0x20, 0x20, 0x20, 
	0x00, 0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0x13,   0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x43, 0x41, 0x52, 0x54, 0x36, 0x34, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

static unsigned char sChipData[] = {
	0x43, 0x48, 0x49, 0x50, 0x00, 0x00, 0x20, 0x10, 0x00, 0x00, 0x00, 0 /*bank number low*/, 0x80, 0x00, 0x20, 0x00
};

static unsigned char sBankData[0x2000];

static FILE *indexFP = 0;
static int outNameIndex = 0;

void ReplaceAll(std::string &str, const std::string& from, const std::string& to)
{
	size_t start_pos = 0;
	while((start_pos = str.find(from, start_pos)) != std::string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}

void ReplaceAllFirstOf(std::string &str, const std::string& from, const std::string& to)
{
	size_t start_pos = 0;
	while((start_pos = str.find_first_of(from, start_pos)) != std::string::npos)
	{
		str.replace(start_pos, 1, to);
		start_pos += to.length();
	}
}

std::string StripFilename(const char *filename)
{
	std::string ret = filename;

	size_t pos = ret.find("+");
	if (std::string::npos != pos)
	{
		ret = ret.substr(0,pos);
	}
	return ret;
}

int FilenameOffset(const char *filename)
{
	int iret = 0;
	std::string ret = filename;

	size_t pos = ret.rfind("+~");
	if (std::string::npos != pos)
	{
		ret = ret.substr(0,pos);
	}
	pos = ret.rfind("+!");
	if (std::string::npos != pos)
	{
		ret = ret.substr(0,pos);
	}
	pos = ret.rfind("+");
	if (std::string::npos != pos)
	{
		ret = ret.substr(pos+1);
		iret = ParamToNum(ret.c_str());
	}

	return iret;
}

int FilenameXOR(const char *filename)
{
	int iret = 0;
	std::string ret = filename;

	size_t pos = ret.rfind("+!");
	if (std::string::npos != pos)
	{
		ret = ret.substr(0,pos);
	}
	pos = ret.rfind("+~");
	if (std::string::npos != pos)
	{
		ret = ret.substr(pos+2);
		iret = ParamToNum(ret.c_str());
	}

	return iret;
}

bool FilenameResetOffset(const char *filename)
{
	std::string ret = filename;

	size_t pos = ret.rfind("+!");
	if (std::string::npos != pos)
	{
		return true;
	}

	return false;
}

void WriteFileIndex(const char * filename, int theBank, int theAddress, int fileLen, int fileXOR)
{
	if (!indexFP)
	{
		return;
	}

	std::string safeName = filename;
	size_t pos = safeName.rfind("\\");
	if (std::string::npos != pos)
	{
		safeName = safeName.substr(pos+1);
	}
	ReplaceAllFirstOf(safeName , "\\/ .:" , "_");

	fprintf(indexFP , "; %s\n" , filename);
	fprintf(indexFP , "CartFile%d_Bank_%s = $%x\n" , outNameIndex, safeName.c_str() , theBank);
	fprintf(indexFP , "CartFile%d_Start_%s = $%x\n" , outNameIndex, safeName.c_str() , theAddress);
	fprintf(indexFP , "CartFile%d_Size_%s = $%x\n" , outNameIndex, safeName.c_str() , fileLen);
	fprintf(indexFP , "CartFile%d_XOR_%s = $%x\n" , outNameIndex, safeName.c_str() , fileXOR);

	fprintf(indexFP , "\n");
}

int WriteChunkToBanks( int startBankNum, int maxSize, DynamicMessageHelper &chunkData, int startAddress, DynamicMessageHelper &output )
{
	sChipData[ 0x0a ] = (unsigned char) (startBankNum >> 8);
	sChipData[ 0x0b ] = (unsigned char) startBankNum;
	printf( "New bank $%x\n" , startBankNum );

	memset( sBankData , startBankNum , sizeof( sBankData ) );

	int currentSize = 0;
	int theDataPos = 0;
	while (currentSize < maxSize)
	{
		int remain = chunkData.GetSize() - theDataPos;
		if (remain > 0)
		{
			int thisBankSize = sizeof(sBankData);
			if (remain > thisBankSize)
			{
				remain = thisBankSize;
			}

			memcpy( sBankData , ( (char *)chunkData.GetBuffer() ) + theDataPos , remain );
			theDataPos += remain;
		}

		int newAddress = startAddress + currentSize;
		sChipData[ 0x0c ] = (unsigned char) (newAddress >> 8);
		sChipData[ 0x0d ] = (unsigned char) newAddress;

		output.AddData( sChipData , sizeof( sChipData ) );
		output.AddData( sBankData , sizeof( sBankData ) );
		printf( " Write multi-bank %d $%x\n" , startBankNum , newAddress );

		currentSize += sizeof(sBankData);
		memset( sBankData , startBankNum , sizeof( sBankData ) );
	}

	chunkData.FreeBuffer();
	startBankNum++;

	// Set new values for the next potential multi-bank operation
	sChipData[ 0x0a ] = (unsigned char) (startBankNum >> 8);
	sChipData[ 0x0b ] = (unsigned char) startBankNum;
	sChipData[ 0x0c ] = (unsigned char) (startAddress >> 8);
	sChipData[ 0x0d ] = (unsigned char) startAddress;	return startBankNum;
}

int MultiWriteBlock(int startBankNum, std::vector<int> &offsets, int startAddress, int maxSize, DynamicMessageHelper &chunkData, DynamicMessageHelper &output)
{
	sChipData[ 0x0a ] = (unsigned char) (startBankNum >> 8);
	sChipData[ 0x0b ] = (unsigned char) startBankNum;
	printf( "New bank $%x\n" , startBankNum );

	memset( sBankData , startBankNum , sizeof( sBankData ) );
	int tableTweak = 2 + (offsets.size() * 2);
	int i;
	for (i = 0 ; i < (int)offsets.size() ; i++)
	{
		int offset = offsets[i];
		offset += startAddress + tableTweak;
		sBankData[(i*2)] = (unsigned char) offset;
		sBankData[(i*2)+1] = (unsigned char) (offset >> 8);
	}
	sBankData[(i*2)] = 0;
	sBankData[(i*2)+1] = 0;
	i++;

	int currentSize = 0;
	int theDataPos = 0;
	while (currentSize < maxSize)
	{
		int remain = chunkData.GetSize() - theDataPos;
		if (remain > 0)
		{
			int thisBankSize = sizeof(sBankData) - tableTweak;
			if (remain > thisBankSize)
			{
				remain = thisBankSize;
			}

			memcpy( sBankData + tableTweak , ( (char *)chunkData.GetBuffer() ) + theDataPos , remain );
			theDataPos += remain;
		}

		int newAddress = startAddress + currentSize;
		sChipData[ 0x0c ] = (unsigned char) (newAddress >> 8);
		sChipData[ 0x0d ] = (unsigned char) newAddress;

		output.AddData( sChipData , sizeof( sChipData ) );
		output.AddData( sBankData , sizeof( sBankData ) );
		printf( " Write multi-bank %d $%x\n" , startBankNum , newAddress );

		currentSize += sizeof(sBankData);
		tableTweak = 0;
		memset( sBankData , startBankNum , sizeof( sBankData ) );
	}

	offsets.clear();
	chunkData.FreeBuffer();
	startBankNum++;

	// Set new values for the next potential multi-bank operation
	sChipData[ 0x0a ] = (unsigned char) (startBankNum >> 8);
	sChipData[ 0x0b ] = (unsigned char) startBankNum;
	sChipData[ 0x0c ] = (unsigned char) (startAddress >> 8);
	sChipData[ 0x0d ] = (unsigned char) startAddress;
	return startBankNum;
}

unsigned char * AddDynamicBlockToBins(const char *filename, std::list<DynamicMessageHelper *> &blocks, int startBankNum, MessageHelper *workBuffer, int maxSize, int startAddress, unsigned char * lastBlockPos , const bool isCompressed , const bool addEndAddress)
{
	int sizeTweak = 0;
	if (addEndAddress)
	{
		sizeTweak = 2;
	}
	DynamicMessageHelper *foundBlock = 0;
	std::list<DynamicMessageHelper *>::iterator st3 = blocks.begin();
	int theBank = startBankNum;
	while (st3 != blocks.end())
	{
		DynamicMessageHelper *checkBlock = *st3++;
		if ((checkBlock->GetSize() + 3 + sizeTweak + workBuffer->GetSize()) <= maxSize)
		{
			foundBlock = checkBlock;
			break;
		}
		theBank++;
	}

	if (!foundBlock)
	{
		foundBlock = new DynamicMessageHelper();
		foundBlock->SetBufferSize(maxSize);
		foundBlock->EnsureBufferAllocated();
		foundBlock->SetGuardSize(maxSize);
		theBank = startBankNum + blocks.size();
		blocks.push_back(foundBlock);
	}

	int realAddress = startAddress + foundBlock->GetSize();
	if (lastBlockPos)
	{
		lastBlockPos[0] = (unsigned char) theBank;
		lastBlockPos[1] = (unsigned char) realAddress;
		// Retain the compressed flag from the previous block
		lastBlockPos[2] = (unsigned char) ((realAddress >> 8) & 0x7f) | (lastBlockPos[2] & 0x80);
	}
	lastBlockPos = (unsigned char *) foundBlock->GetCurrentPosition();
	printf("   Saving $%x to bank $%x at $%x\n" , workBuffer->GetSize() , theBank , realAddress);
	int zero = 0;
	foundBlock->AddData(&zero , 2);
	if (isCompressed)
	{
		// Flag for current block compression
		zero = -1;
		foundBlock->AddData(&zero , 1);
	}
	else
	{
		foundBlock->AddData(&zero , 1);
	}

	if (addEndAddress)
	{
		int endAddress = realAddress + 5 + workBuffer->GetSize();
		
		unsigned char theValue = (unsigned char) endAddress;
		foundBlock->AddData(&theValue , 1);
		theValue = (unsigned char) (endAddress >> 8);
		foundBlock->AddData(&theValue , 1);
	}
	foundBlock->AddData(workBuffer->GetBuffer() , workBuffer->GetSize());


	WriteFileIndex(filename, theBank, realAddress, workBuffer->GetSize(), 0);

	return lastBlockPos;
}

int GetFileLength(std::string &filename)
{
	FILE *fp = fopen(filename.c_str() , "rb");
	if (!fp)
	{
		return -1;
	}
	fseek(fp,0,SEEK_END);
	int size = ftell(fp);
	fseek(fp,0,SEEK_SET);
	fclose(fp);
	return size;
}

void UpdateSortedList(std::set<std::string> &sortedSection, std::list<std::string> &sorted, std::string &prefix)
{
	std::set<std::string>::iterator sts = sortedSection.begin();
	while (sts != sortedSection.end())
	{
		std::string entry = *sts++;
		sorted.push_back(prefix + entry);
	}
}

void SortSectionFromResult(std::set<std::string> &sortedSection, std::list<ScanPath::Entry> &result)
{
	sortedSection.clear();
	std::list<ScanPath::Entry>::iterator st = result.begin();
	while (st != result.end())
	{
		ScanPath::Entry &entry = *st++;
		sortedSection.insert(entry.mName);
	}
}

int main( int argc , char **argv )
{
	int compressionLowerThreshold = -1;
	int compressionUpperThreshold = -1;
	int compressionTrimStart = 0;
	std::string theCompressionToolPath = "..\\bin\\LZMPi.exe";
	std::string theCompressionPreCommand = "-ce ";
	std::string theCompressionPostCommand = " ";

	DynamicMessageHelper output;
	bool displayHelp = false;
	if ( argc <= 1 )
	{
		displayHelp = true;
	}

	strcpy((char*) sCartHeader_64K + 0x20 , "MakeCart");

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

				case 't':
				{
					switch ( argv[0][2] )
					{
						default:
							sCartHeader_64K[0x17] = 0x13;
							sCartHeader_64K[0x19] = 0x01;
							sCartHeader_64K[0x1b] = 0x00;
							break;
						case 'e':
							// https://github.com/svn2github/vice-emu/blob/master/vice/src/cartridge.h
							printf("Setting EasyFlash mode\n");
							sCartHeader_64K[0x17] = 0x20;
							sCartHeader_64K[0x18] = 0x01;
							sCartHeader_64K[0x19] = 0x00;
							break;
						case 'g':
							// https://sourceforge.net/p/vice-emu/code/HEAD/tree/trunk/vice/src/c64/cart/gmod2.c#l473
							// https://github.com/svn2github/vice-emu/blob/ea98cee1eaa42d5a8611d50a4b406db67ce53374/vice/src/c64/cart/crt.c
							printf("Setting GMod2 mode\n");
							sCartHeader_64K[0x17] = 60;
							sCartHeader_64K[0x18] = 0x00;
							sCartHeader_64K[0x19] = 0x01;
							break;
					}
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
					if ( argc  >= 1 )
					{
						bankNum = ParamToNum( argv[0] );
						memset( sBankData , bankNum+1 , sizeof( sBankData ) );

						sChipData[ 0x0a ] = (unsigned char) (bankNum >> 8);
						sChipData[ 0x0b ] = (unsigned char) bankNum;

						printf( "New bank $%x\n" , bankNum );
					}
					break;
				}

				case 'a':
				{
					// New bank address
					int address = 0;
					argc--;
					argv++;
					if ( argc  >= 1 )
					{
						address = ParamToNum( argv[0] );

						sChipData[ 0x0c ] = (unsigned char) (address >> 8);
						sChipData[ 0x0d ] = (unsigned char) address;

						printf( "New address $%x\n" , address );
					}
					break;
				}

				case 'i':
				{
					argc--;
					argv++;
					if ( argc  >= 1 )
					{
						if (indexFP)
						{
							fclose(indexFP);
						}
						indexFP = fopen(argv[0] , "w");
						outNameIndex++;
						if (!indexFP)
						{
							printf("Problem writing file '%s'\n" , argv[0] );
						}
					}
					break;
				}

				case 'r':
				{
					// Read data from a file into a work buffer
					argc--;
					argv++;
					if ( argc  >= 1 )
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
					if ( argc  >= 2 )
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

					if ( ( startBank + toCopy ) > sizeof( sBankData ) )
					{
						toCopy = sizeof( sBankData ) - startBank;
					}

					if ( (startFile >= 0 ) && ( startFile <= workBuffer.GetBufferSize() ) && ( toCopy > 0 )  && ( ( startBank + toCopy ) <= sizeof( sBankData ) ) )
					{
						printf(" Copy from $%x len $%x to $%x\n" , startFile , toCopy , startBank );
						memcpy( sBankData + startBank , ( (char *)workBuffer.GetBuffer() ) + startFile , toCopy );
					}

					break;
				}

				case 'w':
				{
					// Write bank
					output.AddData( sChipData , sizeof( sChipData ) );
					output.AddData( sBankData , sizeof( sBankData ) );
					printf( " Write bank\n" );
					break;
				}

				case 'l':
				{
					if (argv[0][2] == 'c')
					{
						compressionLowerThreshold = -1;
						compressionTrimStart = 0;
					}
					else
					{
						// enable compression tool
						compressionLowerThreshold = ParamToNum( argv[1] );
						compressionUpperThreshold = ParamToNum( argv[2] );
						compressionTrimStart = ParamToNum( argv[3] );
						theCompressionToolPath = argv[4];
						theCompressionPreCommand = argv[5];
						theCompressionPostCommand = argv[6];
						argc -= 6;
						argv += 6;
					}
					break;
				}

				case 'm':
				{
					int compressionSavedBytes = 0;
					int totalbyteSize = 0;
					argc--;
					argv++;
					if ( argc  > 2 )
					{
						int maxSize = ParamToNum( argv[1] );

						ScanPath scanner;
						std::list<ScanPath::Entry> result;
						scanner.Start(argv[0] , result , true , true);

						std::list<std::string> sorted;

						std::set<std::string> sortedSection;
						SortSectionFromResult(sortedSection, result);
						UpdateSortedList(sortedSection, sorted , std::string("n:"));


						std::list<DynamicMessageHelper *> blocks;
						std::list<std::string>::iterator st2 = sorted.begin();

						int startAddress = (((int)sChipData[ 0x0c ]) << 8) | sChipData[ 0x0d ];
						int startBankNum = (((int)sChipData[ 0x0a ]) << 8) | sChipData[ 0x0b ];

						unsigned char *lastBlockPos = 0;

						while (argv[2] != 0 && argv[2][1] == ':')
						{
							// Extension list of filenames
							std::string thePrefix;
							thePrefix += argv[2][0];
							thePrefix += argv[2][1];

							ScanPath scanner;
							std::list<ScanPath::Entry> result;
							scanner.Start(argv[2]+2 , result , true , true);

							SortSectionFromResult(sortedSection, result);
							UpdateSortedList(sortedSection, sorted , thePrefix);

							argv++;
							argc--;
						}

						while (st2 != sorted.end())
						{
							std::string filename = *st2++;
							bool spanMode = false;
							if (filename.c_str()[1] != ':')
							{
								printf("Internal error: I'm expecting : here\n");
								exit(-1);
							}
							if (filename.c_str()[0] == 's')
							{
								spanMode = true;
								lastBlockPos = 0;
							}

							filename = filename.substr(2);

							workBuffer.FreeBuffer();

							// Get the length
							int origLen = GetFileLength(filename);
							int newLen = origLen;

							// Try compression
							std::string newCompressedFile = filename + "_t.tcmp";
							if(compressionLowerThreshold > 0 && origLen > compressionLowerThreshold && origLen < compressionUpperThreshold)
							{
								newLen = GetFileLength(newCompressedFile);

								if (newLen <= 0)
								{
									STARTUPINFOA			si;
									PROCESS_INFORMATION		pi;

									memset( &si, 0, sizeof(si) );
									si.cb = sizeof(si);

									std::string path = theCompressionToolPath;
									path += " ";
									path += theCompressionPreCommand;
									path += filename;
									path += theCompressionPostCommand;
									path += newCompressedFile;

									// We create the new process
									BOOL ret2;
									DWORD theLastError = 0;
									if ( !(ret2 = CreateProcessA(NULL,(LPSTR) path.c_str(),NULL,NULL,FALSE,CREATE_NO_WINDOW,NULL,NULL,&si,&pi )) )
									{
										theLastError= GetLastError();
									}
									else
									{
										WaitForSingleObject(pi.hProcess,INFINITE);

										CloseHandle( pi.hProcess );
										CloseHandle( pi.hThread );
									}
								}


								newLen = GetFileLength(newCompressedFile);
							}

							bool isCompressed = false;
							bool gotData = false;

							if (newLen < origLen)
							{
								compressionSavedBytes += origLen - newLen;
								printf("Replacing '%s' with compressed version saving %d bytes\n" , filename.c_str() , origLen - newLen);
								filename = newCompressedFile;
								isCompressed = true;

								if (compressionTrimStart > 0)
								{
									DynamicMessageHelper workBuffer2;
									if ( workBuffer2.Read( filename.c_str() , true ) )
									{
										printf("File '%s' read $%x adjusting..." , filename.c_str() , workBuffer2.GetBufferSize() );
									}
									else
									{
										printf("Problem reading file '%s'\n" , filename.c_str() );
									}

									int size = workBuffer2.GetBufferSize() - compressionTrimStart;

									workBuffer.SetBufferSize(size);
									workBuffer.EnsureBufferAllocated();
									workBuffer.AddData(((char *)workBuffer2.GetBuffer()) + compressionTrimStart , size);
									workBuffer.SetSize(0);
									gotData = true;
								}
							}
							if (!gotData)
							{
								if ( workBuffer.Read( filename.c_str() , true ) )
								{
									printf("File '%s' read $%x" , filename.c_str() , workBuffer.GetBufferSize() );
								}
								else
								{
									printf("Problem reading file '%s'\n" , filename.c_str() );
								}
							}

							totalbyteSize += workBuffer.GetBufferSize();

							if (spanMode)
							{
								int startPos = 0;
								int fragmentNum = 0;
								while (startPos < workBuffer.GetBufferSize())
								{
									std::string filenameFragment = filename;
									char ending[64];
									sprintf(ending , "_fragment%d" , fragmentNum++);
									filenameFragment += ending;
									
									MessageHelper thisFragment;

									int detectedFree = 0;

									std::list<DynamicMessageHelper *>::iterator st3 = blocks.begin();
									while (st3 != blocks.end())
									{
										DynamicMessageHelper *checkBlock = *st3++;
										int thisSize = maxSize - checkBlock->GetSize() - 5;
										if (thisSize > detectedFree)
										{
											detectedFree = thisSize;
										}
									}

									if (detectedFree <= 64)	// 64 bytes is the smallest amount of memory we want to squeeze data in to
									{
										detectedFree = maxSize - 5;	// bank/lo next/hi next/end lo/end hi
									}

									int theBestSize = min(workBuffer.GetBufferSize() - startPos,detectedFree);

									thisFragment.SetBuffer(((char*)workBuffer.GetBuffer())+startPos);
									thisFragment.SetSize(theBestSize);

									lastBlockPos = AddDynamicBlockToBins(filenameFragment.c_str(), blocks, startBankNum, &thisFragment, maxSize, startAddress, lastBlockPos , isCompressed , true);

									startPos += theBestSize;
								}

								lastBlockPos = 0;
							}
							else
							{
								MessageHelper thisFragment;
								thisFragment.SetBuffer(workBuffer.GetBuffer());
								thisFragment.SetSize(workBuffer.GetBufferSize());

								lastBlockPos = AddDynamicBlockToBins(filename.c_str(), blocks, startBankNum, &thisFragment, maxSize, startAddress, lastBlockPos , isCompressed , false);
							}
						}

						std::list<DynamicMessageHelper *>::iterator st3 = blocks.begin();
						int wasteage = 0 , wasteage2 = 0;
						while (st3 != blocks.end())
						{
							DynamicMessageHelper *checkBlock = *st3++;
							if (st3 != blocks.end())
							{
								wasteage += maxSize - checkBlock->GetSize();
							}
							wasteage2 += maxSize - checkBlock->GetSize();
							startBankNum = WriteChunkToBanks(startBankNum, maxSize , *checkBlock , startAddress, output);
							delete checkBlock;
						}

						printf("Finished adding chunks wasteage=%d wasteage2=%d totalbyteSize=%d compressionSavedBytes=%d\n" , wasteage , wasteage2 , totalbyteSize , compressionSavedBytes);
					}
					break;
				}

				case 'f':
				{
					argc--;
					argv++;
					if ( argc  > 3 )
					{
						int maxSize = ParamToNum( argv[0] );
						int numEntries = ParamToNum( argv[1] );

						argc-=2;
						argv+=2;

						int startAddress = (((int)sChipData[ 0x0c ]) << 8) | sChipData[ 0x0d ];
						int startBankNum = (((int)sChipData[ 0x0a ]) << 8) | sChipData[ 0x0b ];

						DynamicMessageHelper chunkData;
						chunkData.FreeBuffer();

						int theBank = startBankNum;
						int bankSize = 0;
						int entry = 0;
						while (entry < numEntries)
						{
							std::string tempName = StripFilename(argv[entry]);
							const char *filename = tempName.c_str();
							int tweakOffset = FilenameOffset(argv[entry]);
							int fileXOR = FilenameXOR(argv[entry]);
							bool fileReset = FilenameResetOffset(argv[entry]);
							entry++;

							FILE *fp = fopen(filename , "rb");

							if (0 == fp)
							{
								printf("Problem reading file '%s'\n" , filename );
								continue;
							}

							fseek(fp,0,SEEK_END);
							int fileLen = ftell(fp) - tweakOffset;
							fclose(fp);

							if (fileLen <= 0)
							{
								printf("Skip file '%s' due to zero or negative length\n" , filename);
								continue;
							}
							if (fileLen > maxSize)
							{
								fileLen = maxSize;
							}

							printf("File '%s' offset $%x xor $%x len $%x\n" , filename , tweakOffset , fileXOR , fileLen);

							if ( (fileReset && (bankSize > 0)) || (bankSize + fileLen) >= maxSize )
							{
								theBank++;
								bankSize = 0;
							}

							WriteFileIndex(filename, theBank, startAddress + bankSize, fileLen, fileXOR);

							bankSize += fileLen;
						}

						entry = 0;
						while (entry < numEntries)
						{
							std::string tempName = StripFilename(argv[entry]);
							const char *filename = tempName.c_str();
							int tweakOffset = FilenameOffset(argv[entry]);
							int fileXOR = FilenameXOR(argv[entry]);
							bool fileReset = FilenameResetOffset(argv[entry]);
							entry++;

							DynamicMessageHelper fileData;
							fileData.FreeBuffer();

							if ( fileData.Read( filename , true ) )
							{
								printf("File '%s' read $%x to use $%x\n" , filename , fileData.GetBufferSize() , fileData.GetBufferSize() - tweakOffset);
							}
							else
							{
								printf("Problem 2 reading file '%s'\n" , filename );
								continue;
							}

							int fileLen = fileData.GetBufferSize() - tweakOffset;
							if (fileLen <= 0)
							{
								continue;
							}
							if (fileLen > maxSize)
							{
								fileLen = maxSize;
							}

							int i;
							char *theData = (char*)fileData.GetBuffer();
							for (i = 0 ; i < fileData.GetBufferSize() ; i++)
							{
								theData[i] = theData[i] ^ fileXOR;
							}


							// Decide when to flush the current buffer
							if ( (fileReset && (chunkData.GetSize() > 0)) || (chunkData.GetSize() + fileLen) >= maxSize )
							{
								startBankNum = WriteChunkToBanks(startBankNum, maxSize, chunkData, startAddress, output);
							}

							chunkData.AddData((char*)fileData.GetBuffer() + tweakOffset , fileLen);
						}


						if ( chunkData.GetSize() > 0 )
						{
							startBankNum = WriteChunkToBanks(startBankNum, maxSize, chunkData, startAddress, output);
						}
						printf("Finished adding chunks\n");

						argc-=numEntries;
						argv+=numEntries;

						continue;
					}
					break;
				}
					
				case 'o':
				{
					// Write the file data
					argc--;
					argv++;
					if ( argc  >= 1 )
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

	if (indexFP)
	{
		fclose(indexFP);
		indexFP = 0;
	}

	if ( displayHelp )
	{
		printf("MakeCart V1.2 help\n\
-te : Change the cartridge type to EasyFlash.\n\
-tg : Change the cartridge type to GMod2.\n\
-n : Output new cartridge header into the cartridge data buffer. This has to be before -b is used.\n\
-b <bank> : Clear the temporary 8K bank data with the current bank number.\n\
-i <file> : Start writing a file index to the file.\n\
-r <file> : Read data file to the internal work buffer.\n\
-c <bank offset> <start work buffer> <end work buffer> : Copy Data from start to end work buffer offsets into the temporary bank data with the bank offset. If the data is over the end of the temporary bank size of 8K then it is truncated. Any data from any file can be written to any offset in the temporary bank data\n\
-a <address> : Sets the bank load address.\n\
-w : Write the 8K temporary bank data with the chip/bank number from the preceding -b <bank> to the cartridge data buffer\n\
-m <file path wildcard> <max size> : Imports multiple files across banks using the file wildcard and max size per chunk.\n\
-f <max size> <num entries> <file name entries> : Imports multiple files across banks using the max size per chunk and file entries.\n\
-l <compression lower threshold> <compression upper threshold> <path to compression tool> <compression tool command line options before input file> <compression tool command line options before output file>\n\
-lc Cancels any optional compression configuration\n\
-o : Output the whole cartridge data buffer to the file.\n\
" );

	}

	return 0;
}
