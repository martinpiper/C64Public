#include <stdio.h>
#include <process.h>
#include <assert.h>
#include "MusicFile.h"
#include "RNPlatform/Inc/MessageHelper.h"
#include <Windows.h>

using namespace RNReplicaNet;
using namespace MusicStudio1;

static void DumpAsASM(FILE *fp,const unsigned char *data,const int len,const unsigned char rightShift = 0,const unsigned char mask = 0xff,const char *numberPrefix = "$")
{
	if (len > 0)
	{
		fprintf(fp,"\t!by ");
		int i;
		for (i = 0; i < len-1 ; i++)
		{
			fprintf(fp,"%s%02x,",numberPrefix,(data[i] >> rightShift) & mask);
		}
		fprintf(fp,"%s%02x\n",numberPrefix,(data[i] >> rightShift) & mask);
	}
	else
	{
		fprintf(fp,"\n");
	}
}


static unsigned char sDefaultDrumData[] = {
	'D', 'M' , 'P' ,
	0 ,
	43,  8, 12,143,130, 13,  0,  0 ,
	128, 64, 64,128,128, 64,  0,  0 ,
	6,3 ,
	40,  8,  7,  5,  3,  1,  1,  1 ,
	128, 64, 64, 64, 64, 64, 64, 64 ,
	8,7 ,
	8,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe ,
	128, 64, 64, 64, 64, 64, 64, 64 ,
	8,3 ,
	8,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,  9 ,
	128, 64, 64, 64, 64, 64, 64,128 ,
	8,3
};

Envelope::Envelope() :	mAttackDecay(0) ,
						mSustainRelease(0) ,
						mWaveControl(0) ,
						mPulseWidthVariance(0) ,
						mVibratoUp(0) ,
						mVibratoDown(0) ,
						mDrumVibratoControl(0) ,
						mWaveMix(0) ,
						mIsUsed(false) ,
						mRealIndex(0) ,
						mTranspose(0) ,
						mTableWave(0) , mTableNote(0) , mTablePulse(0) , mTableFilter(0) ,
						mActiveTableWave(false) , mActiveTableNote(false) , mActiveTablePulse(false) , mActiveTableFilter(false) ,
						mHardRestartTime(1) , mHardRestartAttackDecay(0) , mHardRestartSustainRelease(0xf0) , mHardRestartWave(9) ,
						mAllowVoiceEffects(true)
{
}

Envelope::~Envelope()
{
}

DrumData::DrumData() : mStartLoop(0) , mEndLoop(0) , mHighestStartPos(0) , mGateLength(1) , mUsed(false)
{
	memset(mHiNotes,0,sizeof(mHiNotes));
	memset(mWaveControl,0,sizeof(mWaveControl));
}

DrumData::~DrumData()
{
}


const char* notes[] = {
	"C-",
	"C#",
	"D-",
	"D#",
	"E-",
	"F-",
	"F#",
	"G-",
	"G#",
	"A-",
	"A#",
	"B-"
};



BlockEntry::BlockEntry() : mMusic(0) , mBlockNum(0) , mTheLine(-1) , mBlockTime(-1)
{
}

BlockEntry::~BlockEntry()
{
}

std::string BlockEntry::GetNoteFromNumber(const unsigned char number)
{
	int oct = number / 12;
	int note = number % 12;
	char line[32];
	sprintf(line,"%s%d",notes[note],oct);

	return std::string(line);
}

class BlockEntryNote : public BlockEntry
{
public:
	BlockEntryNote(unsigned char note) : mNote(note) {}

	void GetFormatted(char *line)
	{
		int oct = mNote / 12;
		int note = mNote % 12;
		sprintf(line,"%s%d",notes[note],oct);
	}

	std::string GetCommandName(void)
	{
		return "";
	}

	int GetByteSize(void)
	{
		return 1;
	}

	virtual int WriteASM(FILE *fp)
	{
		int transpose = mMusic->mCurrentBlockTranspose;
		if (transpose >= 128)
		{
			transpose = -(256 - transpose);
		}
		fprintf(fp,"%d",int(mNote) + transpose);
		return GetByteSize();
	}

	virtual bool IsNote(void)
	{
		return true;
	}

	virtual bool Transpose(const int transpose)
	{
		int note = mNote;
		note += transpose;
		if ((note >= 0) && (note < 96))
		{
			mNote = (unsigned char) note;
			return true;
		}
		return false;
	}

private:
	unsigned char mNote;
};

class BlockEntryFilterGlide : public BlockEntry
{
public:
	BlockEntryFilterGlide(const unsigned char stepSize, const unsigned char speed) : mStepSize(stepSize) , mSpeed(speed) {}

	void GetFormatted(char *line)
	{
		sprintf(line,"FG:%02X,%02X",mStepSize,mSpeed);
	}

	std::string GetCommandName(void)
	{
		return "FilterGlide";
	}

	int GetByteSize(void)
	{
		return 3;
	}

	virtual int WriteASM(FILE *fp)
	{
		fprintf(fp,"<BlockCommandFilterGlide , $%02x , $%02x",mStepSize,mSpeed);
		return GetByteSize();
	}

private:
	unsigned char mStepSize;
	unsigned char mSpeed;
};

class BlockEntryGeneric : public BlockEntry
{
public:
	BlockEntryGeneric(const char *tag,const char *origTag) : mTag(tag) , mOrigTag(origTag) {}

	void GetFormatted(char *line)
	{
		sprintf(line,"%s",mOrigTag);
	}

	std::string GetCommandName(void)
	{
		return mTag;
	}

	int GetByteSize(void)
	{
		return 1;
	}

	virtual int WriteASM(FILE *fp)
	{
		fprintf(fp,"<BlockCommand%s",mTag);
		return GetByteSize();
	}

private:
	const char *mTag;
	const char *mOrigTag;
};

class BlockEntryGeneric1 : public BlockEntry
{
public:
	BlockEntryGeneric1(const char *tag,const char *origTag,const unsigned char param) : mTag(tag) , mOrigTag(origTag) , mParam(param) {}

	void GetFormatted(char *line)
	{
		sprintf(line,"%s:%02X",mOrigTag,mParam);
	}

	std::string GetCommandName(void)
	{
		return mTag;
	}

	int GetByteSize(void)
	{
		return 2;
	}

	virtual int WriteASM(FILE *fp)
	{
		fprintf(fp,"<BlockCommand%s , $%02x",mTag,mParam);
		return GetByteSize();
	}

	virtual unsigned char GetDataByte0(void)
	{
		return mParam;
	}

protected:
	const char *mTag;
	const char *mOrigTag;
	unsigned char mParam;
};

class BlockEntryGeneric2 : public BlockEntry
{
public:
	BlockEntryGeneric2(const char *tag,const char *origTag,const unsigned char param1,const unsigned char param2) : mTag(tag) , mOrigTag(origTag) , mParam1(param1) , mParam2(param2) {}

	void GetFormatted(char *line)
	{
		sprintf(line,"%s:%02X,%02X",mOrigTag,mParam1,mParam2);
	}

	std::string GetCommandName(void)
	{
		return mTag;
	}

	int GetByteSize(void)
	{
		return 3;
	}

	virtual int WriteASM(FILE *fp)
	{
		fprintf(fp,"<BlockCommand%s , $%02x , $%02x",mTag,mParam1,mParam2);
		return GetByteSize();
	}

	virtual unsigned char GetDataByte0(void)
	{
		return mParam1;
	}

	virtual unsigned char GetDataByte1(void)
	{
		return mParam2;
	}

protected:
	const char *mTag;
	const char *mOrigTag;
	unsigned char mParam1,mParam2;
};


class BlockEntrySlurEnable : public BlockEntryGeneric
{
public:
	BlockEntrySlurEnable() : BlockEntryGeneric("SlurEnable","SLE") {}

	void UpdateReferences(void)
	{
		mMusic->mSluringCommandsActive = true;
		mMusic->mInternalSlurCount++;
	}

private:
};

class BlockEntrySlurRelease : public BlockEntryGeneric
{
public:
	BlockEntrySlurRelease() : BlockEntryGeneric("SlurRelease","SLR") {}

	void UpdateReferences(void)
	{
		mMusic->mSluringCommandsActive = true;
		mMusic->mInternalSlurCount--;
		if (mMusic->mInternalSlurCount < 0)
		{
			mMusic->mInternalSlurCount = 0;
		}
	}

private:
};

class BlockEntryHardRestartEnable : public BlockEntryGeneric
{
public:
	BlockEntryHardRestartEnable() : BlockEntryGeneric("HardRestartEnable","HRE") {}

	void UpdateReferences(void)
	{
		mMusic->mHardRestartCommandsActive = true;
	}

private:
};

class BlockEntryHardRestartDisable : public BlockEntryGeneric
{
public:
	BlockEntryHardRestartDisable() : BlockEntryGeneric("HardRestartDisable","HRD") {}

	void UpdateReferences(void)
	{
		mMusic->mHardRestartCommandsActive = true;
	}

private:
};

class BlockEntryHardRestartAD : public BlockEntryGeneric1
{
public:
	BlockEntryHardRestartAD(const unsigned char param) : BlockEntryGeneric1("HardRestartAD","HAD",param) {}

	void UpdateReferences(void)
	{
		mMusic->mVariableHardRestartParams = true;
	}

private:
};

class BlockEntryHardRestartSR : public BlockEntryGeneric1
{
public:
	BlockEntryHardRestartSR(const unsigned char param) : BlockEntryGeneric1("HardRestartSR","HSR",param) {}

	void UpdateReferences(void)
	{
		mMusic->mVariableHardRestartParams = true;
	}

private:
};

class BlockEntryHardRestartWV : public BlockEntryGeneric1
{
public:
	BlockEntryHardRestartWV(const unsigned char param) : BlockEntryGeneric1("HardRestartWV","HWV",param) {}

	void UpdateReferences(void)
	{
		mMusic->mVariableHardRestartParams = true;
	}

private:
};

class BlockEntryHardRestartTI : public BlockEntryGeneric1
{
public:
	BlockEntryHardRestartTI(const unsigned char param) : BlockEntryGeneric1("HardRestartTI","HTI",param) {}

	void UpdateReferences(void)
	{
		mMusic->mVariableHardRestartParams = true;
	}

private:
};



class BlockEntryArpeggioStop : public BlockEntryGeneric
{
public:
	BlockEntryArpeggioStop() : BlockEntryGeneric("ArpeggioStop","ARS") {}

	void UpdateReferences(void)
	{
		mMusic->mArpeggioStopUsed = true;
	}

private:
};

class BlockEntryArpeggio : public BlockEntryGeneric1
{
public:
	BlockEntryArpeggio(const unsigned char param) : BlockEntryGeneric1("Arpeggio","ARP",param) {}

	void UpdateReferences(void)
	{
		mMusic->mArpeggioUsed = true;
	}

private:
};

class BlockEntryEnvelope : public BlockEntry
{
public:
	BlockEntryEnvelope(unsigned char envelope) : mEnvelope(envelope) {}

	void GetFormatted(char *line)
	{
		sprintf(line,"ENV:%02X",mEnvelope);
	}

	std::string GetCommandName(void)
	{
		return "Envelope";
	}

	int GetByteSize(void)
	{
		return 2;
	}

	void UpdateReferences(void)
	{
		mMusic->mEnvelopes[mEnvelope].mIsUsed = true;
	}

	virtual int WriteASM(FILE *fp)
	{
		mMusic->mCurrentBlockTranspose = mMusic->mEnvelopes[mEnvelope].mTranspose;
		fprintf(fp,"<BlockCommandEnvelope , $%02x",mMusic->mEnvelopes[mEnvelope].mRealIndex);
		return GetByteSize();
	}

	virtual unsigned char GetDataByte0(void)
	{
		return mEnvelope;
	}

private:
	unsigned char mEnvelope;
};

class BlockEntryGlide : public BlockEntry
{
public:
	BlockEntryGlide(const unsigned char stepSize, const unsigned char delay) : mStepSize(stepSize) , mDelay(delay) {}

	void GetFormatted(char *line)
	{
		sprintf(line,"GL:%02X,%02X",mStepSize,mDelay);
	}

	std::string GetCommandName(void)
	{
		return "Glide";
	}

	int GetByteSize(void)
	{
		return 3;
	}

	virtual int WriteASM(FILE *fp)
	{
		fprintf(fp,"<BlockCommandGlide , $%02x , $%02x",mStepSize,mDelay);
		return GetByteSize();
	}

private:
	unsigned char mStepSize;
	unsigned char mDelay;
};

class BlockEntryExtendedArpeggio : public BlockEntryGeneric1
{
public:
	BlockEntryExtendedArpeggio(const unsigned char param) : BlockEntryGeneric1("ExtendedArpeggio","ERP",param) {}

	void UpdateReferences(void)
	{
		mMusic->mExtendedArpeggioUsed = true;
		mMusic->mExtendedArpeggios[mParam].mUsed = true;
	}

	int GetByteSize(void)
	{
		return 3;
	}

	virtual int WriteASM(FILE *fp)
	{
		fprintf(fp,"<BlockCommand%s , $%02x ,$%02x",mTag,mMusic->mExtendedArpeggiosReverse[mParam]+1,mMusic->mExtendedArpeggiosReverse[mParam] + mMusic->mExtendedArpeggios[mParam].mLength + 1);
		return GetByteSize();
	}
};

class BlockEntrySmallVibratoStop : public BlockEntryGeneric
{
public:
	BlockEntrySmallVibratoStop() : BlockEntryGeneric("SmallVibratoStop","VBS") {}

	void UpdateReferences(void)
	{
		mMusic->mSmallVibratoUsed = true;
	}

private:
};

class BlockEntrySmallVibratoDelay : public BlockEntryGeneric1
{
public:
	BlockEntrySmallVibratoDelay(const unsigned char param) : BlockEntryGeneric1("SmallVibratoDelay","VBD",param) {}

	void UpdateReferences(void)
	{
		mMusic->mSmallVibratoUsed = true;
	}
};

class BlockEntrySmallVibrato : public BlockEntryGeneric2
{
public:
	BlockEntrySmallVibrato(const unsigned char param1,const unsigned char param2) : BlockEntryGeneric2("SmallVibrato","VIB",param1,param2) {}

	void UpdateReferences(void)
	{
		mMusic->mSmallVibratoUsed = true;
	}
};


class BlockEntryTableWaveSet : public BlockEntryGeneric1
{
public:
	BlockEntryTableWaveSet(const unsigned char param) : BlockEntryGeneric1("TableWaveSet","TWV",param) {}

	void UpdateTableReferences(void)
	{
		mMusic->MarkExecutedTableBytes(mMusic->mTablesControls[0],mMusic->mTablesValues[0],mMusic->mTablesUsedBytes[0],mMusic->mTablesUsedByJump[0],mMusic->mEnvelopes[mParam].mTableWave);
	}

	virtual int WriteASM(FILE *fp)
	{
		fprintf(fp,"<BlockCommand%s , $%02x",mTag,mMusic->mEnvelopes[mParam].mTableWave);
		return GetByteSize();
	}
};

class BlockEntryTableNoteSet : public BlockEntryGeneric1
{
public:
	BlockEntryTableNoteSet(const unsigned char param) : BlockEntryGeneric1("TableNoteSet","TNT",param) {}

	void UpdateTableReferences(void)
	{
		mMusic->MarkExecutedTableBytes(mMusic->mTablesControls[1],mMusic->mTablesValues[1],mMusic->mTablesUsedBytes[1],mMusic->mTablesUsedByJump[1],mMusic->mEnvelopes[mParam].mTableNote);
	}

	virtual int WriteASM(FILE *fp)
	{
		fprintf(fp,"<BlockCommand%s , $%02x",mTag,mMusic->mEnvelopes[mParam].mTableNote);
		return GetByteSize();
	}
};

class BlockEntryTablePulseSet : public BlockEntryGeneric1
{
public:
	BlockEntryTablePulseSet(const unsigned char param) : BlockEntryGeneric1("TablePulseSet","TPL",param) {}

	void UpdateTableReferences(void)
	{
		mMusic->MarkExecutedTableBytes(mMusic->mTablesControls[2],mMusic->mTablesValues[2],mMusic->mTablesUsedBytes[2],mMusic->mTablesUsedByJump[2],mMusic->mEnvelopes[mParam].mTablePulse);
	}

	virtual int WriteASM(FILE *fp)
	{
		fprintf(fp,"<BlockCommand%s , $%02x",mTag,mMusic->mEnvelopes[mParam].mTablePulse);
		return GetByteSize();
	}
};

class BlockEntryTableFilterSet : public BlockEntryGeneric1
{
public:
	BlockEntryTableFilterSet(const unsigned char param) : BlockEntryGeneric1("TableFilterSet","TFL",param) {}

	void UpdateTableReferences(void)
	{
		mMusic->MarkExecutedTableBytes(mMusic->mTablesControls[3],mMusic->mTablesValues[3],mMusic->mTablesUsedBytes[3],mMusic->mTablesUsedByJump[3],mMusic->mEnvelopes[mParam].mTableFilter);
	}

	virtual int WriteASM(FILE *fp)
	{
		fprintf(fp,"<BlockCommand%s , $%02x",mTag,mMusic->mEnvelopes[mParam].mTableFilter);
		return GetByteSize();
	}
};



ExtendedArpeggio::ExtendedArpeggio() : mUsed(false) , mLength(0)
{
	memset(mArpeggios,0,sizeof(mArpeggios));
}

ExtendedArpeggio::~ExtendedArpeggio()
{
}

MusicFile::MusicFile() : mCurrentBlockTranspose(0)
{
	memset(mTracks,0,sizeof(mTracks));
	memset(mDrums,0,sizeof(mDrums));
	memset(mEnvelopes,0,sizeof(mEnvelopes));
	mMaxExtendedArpeggioData = 0;
	memset(mTablesControls,0,sizeof(mTablesControls));
	memset(mTablesValues,0,sizeof(mTablesValues));
}

MusicFile::~MusicFile()
{
	int i;
	for (i=0;i<64;i++)
	{
		ClearBlock(i);
	}
}

bool MusicFile::LoadFromFile(const char *inputFilename)
{
	// Read the input file and unpack it
	DynamicMessageHelper inputFile;
	if (!inputFile.Read(inputFilename,true))
	{
		printf("Couldn't read input file\n");
		exit(-1);
	}

	if (inputFile.GetBufferSize() < 0x1b)
	{
		printf("Input file is too small\n");
		exit(-1);
	}

	char *startOfFile = (char *) inputFile.GetBuffer();
	int sizeToParse = inputFile.GetBufferSize();

	// Check to see if it is C64File format data
	if (strncmp(startOfFile,"C64File",7) == 0 && (startOfFile[0x1a] == 0) && (startOfFile[0x1b] == 0x46))
	{
		printf("C64File header found\n");
		startOfFile += 0x1c;
		sizeToParse -= 0x1c;
	}
	// Check to see if it is a prg file saved with just the expected two byte header
	else if ((startOfFile[0] == 0) && (startOfFile[1] == 0x46))
	{
		printf("Two byte header found\n");
		startOfFile += 0x02;
		sizeToParse -= 0x02;
	}
	else
	{
		printf("Error, unknown header found\n");
		return false;
	}

	printf("Parsing file of %d bytes\n",sizeToParse);

	// Calculate the size to unpack to by parsing the run length encoding where 0xe7 is the
	// run length marker, followed by the byte folowed by the length.
	int i;
	int unpackSize = 0;
	for (i = 0 ; i < sizeToParse; i++)
	{
		if ((unsigned char)startOfFile[i] == 0xe7)
		{
			// A length of zero really means use 256 bytes
			unpackSize += startOfFile[i+2]?(unsigned char)startOfFile[i+2]:256;
			i+=2;
		}
		else
		{
			unpackSize++;
		}
	}

	printf("Calculated unpacked file size of %d bytes\n",unpackSize);

	DynamicMessageHelper realFileData;
	realFileData.SetBufferSize(unpackSize);
	for (i = 0 ; i < sizeToParse; i++)
	{
		if ((unsigned char)startOfFile[i] == 0xe7)
		{
			// A length of zero really means use 256 bytes
			int numBytes = startOfFile[i+2]?(unsigned char)startOfFile[i+2]:256;
			while(numBytes > 0)
			{
				realFileData << startOfFile[i+1];
				numBytes--;
			}
			i+=2;
		}
		else
		{
			realFileData << startOfFile[i];
		}
	}

	if (realFileData.GetSize() != unpackSize)
	{
		printf("An unexpected error occurred while unpacking the data\n");
		return false;
	}

	inputFile.FreeBuffer();
//	std::string tempOutputFilename = std::string(inputFilename)+".raw";
//	realFileData.Write(tempOutputFilename.c_str(),true);

	unsigned char *theRealData = (unsigned char*) realFileData.GetBuffer();
	realFileData.SetSize(0);
	// Now parse the unpacked data into classes for easier parsing
	// The track data
	int j;
	for (i=0;i<kMaxTracks;i++)
	{
		mHighestUsedPosition[i] = 0;
		for (j=0;j<256;j++)
		{
			realFileData >> mTracks[i][j];
		}
	}

	// Envelope data
	for (i=0;i<kOldMaxEnvelopes;i++)
	{
		// Initialise the real index until we can parse block data to figure out the optimised
		// real index.
		mEnvelopes[i].mRealIndex = i;
		realFileData >> mEnvelopes[i].mAttackDecay;
		realFileData >> mEnvelopes[i].mSustainRelease;
		realFileData >> mEnvelopes[i].mWaveControl;
		// Strip out sync bit since it is ignored by the old play routine but can be used
		// by the new routine.
		mEnvelopes[i].mWaveControl = mEnvelopes[i].mWaveControl & 0xfd;
		realFileData >> mEnvelopes[i].mPulseWidthVariance;
		realFileData >> mEnvelopes[i].mVibratoUp;
		realFileData >> mEnvelopes[i].mVibratoDown;
		realFileData >> mEnvelopes[i].mDrumVibratoControl;
		realFileData >> mArpeggios[i];

		// If we are using a drum sound then remove unused effects from the envelope
		if ((mEnvelopes[i].mDrumVibratoControl & 0xf0) == 0x80)
		{
			mArpeggios[i] = 0;
			mEnvelopes[i].mVibratoUp = 0;
			mEnvelopes[i].mVibratoDown = 0;
			mEnvelopes[i].mPulseWidthVariance = mEnvelopes[i].mPulseWidthVariance & 0x0f;
			mEnvelopes[i].mWaveControl = 0x41;
		}
		else
		{
			// Spot unused pulse variance
			if ((mEnvelopes[i].mWaveControl & 0x40) == 0)
			{
				mEnvelopes[i].mPulseWidthVariance = 0;
			}

			// No waveforms used at all, make it a blank envelope.
			if (((mEnvelopes[i].mWaveControl | mEnvelopes[i].mWaveMix) & 0xf0) == 0)
			{
				mArpeggios[i] = 0;
				mEnvelopes[i].mAttackDecay = 0;
				mEnvelopes[i].mSustainRelease = 0;
				mEnvelopes[i].mWaveControl = 0x08;
				mEnvelopes[i].mPulseWidthVariance = 0;
				mEnvelopes[i].mVibratoUp = 0;
				mEnvelopes[i].mVibratoDown = 0;
				mEnvelopes[i].mDrumVibratoControl = 0;
				mEnvelopes[i].mWaveMix = 0;
			}

			// For vibrato the $0f bit mask is used because the old editor never supported other bits
			mEnvelopes[i].mDrumVibratoControl = mEnvelopes[i].mDrumVibratoControl & 0x0f;
			// Spot empty vibratos
			if (((mEnvelopes[i].mVibratoUp | mEnvelopes[i].mVibratoDown) & 0x0f) == 0)
			{
				mEnvelopes[i].mVibratoUp = 0;
				mEnvelopes[i].mVibratoDown = 0;
				mEnvelopes[i].mDrumVibratoControl = 0;
			}
		}
	}

	// Skip the block offset table
	realFileData.SetSize(realFileData.GetSize()+(64*2));

	unsigned char parsedDrumData[76];
	assert(sizeof(parsedDrumData) == sizeof(sDefaultDrumData));

	realFileData.GetData(parsedDrumData,sizeof(parsedDrumData));
	bool drumDataThere = true;
	
	if ((parsedDrumData[0] != sDefaultDrumData[0]) || (parsedDrumData[1] != sDefaultDrumData[1]) || (parsedDrumData[2] != sDefaultDrumData[2]))
	{
		memcpy(parsedDrumData,sDefaultDrumData,sizeof(parsedDrumData));
		drumDataThere = false;
		printf("Drum data not found using default data\n");
	}

	// Now parse either the loaded data or the default data
	MessageHelper drumData;
	drumData.SetBuffer(parsedDrumData+4);

	for (i=0;i<kOldMaxDrums;i++)
	{
		drumData.GetData(mDrums[i].mHiNotes,8);
		drumData.GetData(mDrums[i].mWaveControl,8);
		// Strip out drum gates from imported data.
		// They shouldn't have been there in the first place.
		// Now of course they can be used in new files but their operation has changed.
		for (j=0;j<8;j++)
		{
			mDrums[i].mWaveControl[j] = mDrums[i].mWaveControl[j] & 0xfe;
		}

		drumData >> mDrums[i].mEndLoop;
		drumData >> mDrums[i].mStartLoop;
	}

	if (drumDataThere)
	{
		// New format files with drum data also have wave mix data
		realFileData.SetSize(0x4e0);
		for (i=0;i<32;i++)
		{
			realFileData >> mEnvelopes[i].mWaveMix;
		}
	}

	// Now parse the blocks
	realFileData.SetSize(0x500);

	for (i=0;i<64;i++)
	{
		// Initialise until the blocks can be optimised
		mRealBlockIndex[i] = i;
		unsigned char lastArpeggio = 0;
		bool lastArpeggioNotSet = true;
		unsigned char blockCommand;

		// Special case for spotting ENV:01, DUR:01 and C-0 at the start of a block
		// This handles original versions of files that have problematic fake hard restart tweaks
		unsigned char *specialCase = (unsigned char*) realFileData.GetCurrentPosition();
		if (	(specialCase[0] == kMusicPlayer_Envelope) && (specialCase[1] == 1) &&
				(specialCase[2] == kMusicPlayer_Duration) && (specialCase[3] == 1) &&
				(specialCase[4] == 0) )
		{
			// Convert it to have a one longer duration
			specialCase[3] = 2;
		}


		do
		{
			BlockEntry *entry = 0;

			realFileData >> blockCommand;

			if (blockCommand <= 127)
			{
				entry = new BlockEntryNote(blockCommand);
			}
			else
			{
				switch(blockCommand)
				{
					case kMusicPlayer_FilterGlide:
					{
						unsigned char stepSize , speed;
						realFileData >> stepSize;
						realFileData >> speed;
						entry = new BlockEntryFilterGlide(stepSize,speed);
						break;
					}

					case kMusicPlayer_FilterHi:
					{
						unsigned char param;
						realFileData >> param;
						entry = new BlockEntryGeneric1("FilterHi","FLH",param);
						break;
					}

					case kMusicPlayer_FilterLo:
					{
						unsigned char param;
						realFileData >> param;
						entry = new BlockEntryGeneric1("FilterLo","FLL",param);
						break;
					}

					case kMusicPlayer_FilterControl:
					{
						unsigned char param;
						realFileData >> param;
						entry = new BlockEntryGeneric1("FilterControl","FLC",param);
						break;
					}

					case kMusicPlayer_FilterPass:
					{
						unsigned char param;
						realFileData >> param;
						entry = new BlockEntryGeneric1("FilterPass","FLP",param);
						break;
					}

					case kMusicPlayer_Duration:
					{
						unsigned char param;
						realFileData >> param;
						entry = new BlockEntryGeneric1("Duration","DUR",param);
						break;
					}

					case kMusicPlayer_Envelope:
					{
						unsigned char envelope;
						realFileData >> envelope;
						entry = new BlockEntryEnvelope(envelope);

						// Convert the envelope command to have an extra arpeggio if it
						// changes inside this block.
						if (lastArpeggioNotSet || (mArpeggios[envelope] != lastArpeggio))
						{
							lastArpeggioNotSet = false;
							lastArpeggio = mArpeggios[envelope];
							AddBlockEntry(i,entry);
							if (lastArpeggio)
							{
								entry = new BlockEntryArpeggio(lastArpeggio);
							}
							else
							{
								entry = new BlockEntryArpeggioStop();
							}
						}
						break;
					}

					case kMusicPlayer_Volume:
					{
						unsigned char param;
						realFileData >> param;
						entry = new BlockEntryGeneric1("Volume","VOL",param);
						break;
					}

					case kMusicPlayer_Glide:
					{
						unsigned char stepSize , delay;
						realFileData >> stepSize;
						realFileData >> delay;
						entry = new BlockEntryGlide(stepSize,delay);
						break;
					}

					default:
					case kMusicPlayer_EndBlock:
					{
						// No need to add a block that says "the end" since it is a std::list.
						break;
					}
				}
			}
			if (entry)
			{
				AddBlockEntry(i,entry);
			}			
		} while (blockCommand != kMusicPlayer_EndBlock);
	}


	return true;
}

#define REMOVEENTRY()	\
					std::list<BlockEntry*>::iterator del = st++;	\
					mBlocks[i].erase(del);	\
					continue;


void MusicFile::Optimise(void)
{
	mAutoUsedFeatures.clear();
	mSluringCommandsActive = false;
	mHardRestartCommandsActive = false;
	mExtendedArpeggioUsed = false;
	mArpeggioUsed = false;
	mArpeggioStopUsed = false;
	mSmallVibratoUsed = false;
	mPulseWidthVarianceUsed = false;
	mDrumDynamicStartOffsetUsed = false;
	mDrumGateLengthUsed = false;
	mVariableHardRestartParams = false;

	int realIndex = 0;

	int i,j;

	// Setup the track reverse lookup table
	for (i=0;i<kMaxTracks;i++)
	{
		for (j=0;j<kMaxTrackLength;j++)
		{
			mTrackReverseIndex[i][j] = j;
		}
	}

	// Calculate the highest used position for each track
	for (i=0;i<kMaxTracks;i++)
	{
		mHighestUsedPosition[i] = 0;
		for (j=0;j<256;j++)
		{
			if (mTracks[i][j] >= kMusicPlayer_StopTrack)
			{
				mHighestUsedPosition[i] = j;
			}
		}
	}

	OptimiseTracks();

	// MPi: TODO: Can add further optimisation to remove commands with little effect
	// such as double durations or extra slur enables that are not needed.
	// First pass, flag what we need
	bool firstPassArpeggio = false;
	bool firstPassExtendedArpeggio = false;
	bool firstPassSmallVibrato = false;
	bool firstPassFixedVibrato = false;
	for (i=0;i<kMaxBlocks;i++)
	{
		if (mTrackUsedBlock[i])
		{
			std::list<BlockEntry*>::iterator st = mBlocks[i].begin();
			while (st != mBlocks[i].end())
			{
				char line[256];
				(*st)->GetFormatted(line);
				if (_strnicmp(line,"ARP:",4) == 0)
				{
					firstPassArpeggio = true;
				}
				else if (_strnicmp(line,"ERP:",4) == 0)
				{
					firstPassExtendedArpeggio = true;
				}
				else if (_strnicmp(line,"VIB:",4) == 0)
				{
					firstPassSmallVibrato = true;
				}
				else if (_strnicmp(line,"FVB:",4) == 0)
				{
					firstPassFixedVibrato = true;
				}

				st++;
			}
		}
	}

	// Second pass, use the flags calculated above and remove any redundant commands.
	for (i=0;i<kMaxBlocks;i++)
	{
		if (mTrackUsedBlock[i])
		{
			std::list<BlockEntry*>::iterator st = mBlocks[i].begin();
			while (st != mBlocks[i].end())
			{
				char line[256];
				(*st)->GetFormatted(line);
				if (!firstPassArpeggio && !firstPassExtendedArpeggio && _strnicmp(line,"ARS",3) == 0)
				{
					REMOVEENTRY();
				}
				else if (!firstPassSmallVibrato && _strnicmp(line,"VBD:",4) == 0)
				{
					REMOVEENTRY();
				}
				else if (!firstPassSmallVibrato && _strnicmp(line,"VBS",3) == 0)
				{
					REMOVEENTRY();
				}
				else if (!firstPassFixedVibrato && _strnicmp(line,"FVD:",4) == 0)
				{
					REMOVEENTRY();
				}
				else if (!firstPassFixedVibrato && _strnicmp(line,"FVS",3) == 0)
				{
					REMOVEENTRY();
				}

				st++;
			}
		}
	}

	// And again to make sure any new empty blocks are now removed.
	OptimiseTracks();

	// Update block references
	realIndex = 0;
	for (i=0;i<kMaxBlocks;i++)
	{
		mBlockByteToLine[i].clear();
		if (mTrackUsedBlock[i])
		{
			int resultantSize = 0;
			mRealBlockIndex[i] = realIndex++;

			std::list<BlockEntry*>::iterator st = mBlocks[i].begin();
			while (st != mBlocks[i].end())
			{
				(*st)->UpdateReferences();
				mBlockByteToLine[i].insert(std::pair<int,int>(resultantSize,(*st)->GetTheLine()));
				resultantSize += (*st)->GetByteSize();
				std::string commandName = (*st)->GetCommandName();
				if (!commandName.empty())
				{
					std::string usedFeature = commandName + "AutoCommandsActive";
					mAutoUsedFeatures.insert(usedFeature);
				}
				st++;
			}
		}
	}

	// Renumber track used block numbers
	for (i=0;i<kMaxTracks;i++)
	{
		// Flag used blocks
		for (j=0;j<mHighestUsedPosition[i];j++)
		{
			if (mTracks[i][j] < 64)
			{
				mTracks[i][j] = mRealBlockIndex[mTracks[i][j]];
			}
		}
	}	

	// Optimise envelopes
	realIndex = 0;
	for (i=0;i<kMaxEnvelopes;i++)
	{
		if (mEnvelopes[i].mIsUsed)
		{
			mEnvelopes[i].mRealIndex = realIndex++;
		}
	}

	mMaxExtendedArpeggioData = 0;
	for (i=0;i<kMaxExtendedArpeggios;i++)
	{
		if (mExtendedArpeggios[i].mUsed)
		{
			if (mExtendedArpeggios[i].mLength > MusicStudio1::ExtendedArpeggio::kMaxExtendedArpeggioData)
			{
				mExtendedArpeggios[i].mLength = MusicStudio1::ExtendedArpeggio::kMaxExtendedArpeggioData;
			}
			if (mExtendedArpeggios[i].mLength)
			{
				// Search any already added arpeggio data with the current used one
				int k;
				bool added = false;
				for (k=0;k<=mMaxExtendedArpeggioData-mExtendedArpeggios[i].mLength;k++)
				{
					for (j=0;j<mExtendedArpeggios[i].mLength;j++)
					{
						if (mExtendedArpeggios[i].mArpeggios[j] != mExtendedArpeggioData[k+j])
						{
							break;
						}
					}
					// Test to see if we found a matching existing sequence
					if (j == mExtendedArpeggios[i].mLength)
					{
						mExtendedArpeggiosReverse[i] = k;
						added = true;
						break;
					}
				}
				// If there was no match then insert a new sequence
				if (!added)
				{
					mExtendedArpeggiosReverse[i] = mMaxExtendedArpeggioData;
					// Copy in the new data
					for (j=0;j<mExtendedArpeggios[i].mLength;j++)
					{
						mExtendedArpeggioData[mMaxExtendedArpeggioData++] = mExtendedArpeggios[i].mArpeggios[j];
					}
				}
			}
			else
			{
				mExtendedArpeggiosReverse[i] = mMaxExtendedArpeggioData;
				// MPi: TODO: Warn the user they are using a block with 0 length arpeggio?
				// It should already be a block error so maybe not.
			}
		}
	}

	OptimiseTables();
}


void MusicFile::OptimiseTracks(void)
{
	// Find any empty index blocks in the track and remove it
	int i,j;

	for (i=0;i<kMaxTracks;i++)
	{
		// Zero out unused positions
		for (j=mHighestUsedPosition[i]+1;j<kMaxTrackLength;j++)
		{
			mTracks[i][j] = 0;
		}

		bool anyRemoved;
		do
		{
			anyRemoved = false;
			for (j=0;j<mHighestUsedPosition[i];j++)
			{
				if (mTracks[i][j] < kMaxBlocks)
				{
					if (mBlocks[mTracks[i][j]].empty())
					{
						// Remove the track index at this position
						for (;j<mHighestUsedPosition[i];j++)
						{
							mTracks[i][j] = mTracks[i][j+1];
							mTrackReverseIndex[i][j] = mTrackReverseIndex[i][j+1];
						}
						mHighestUsedPosition[i]--;
						anyRemoved = true;
					}
				}
			}
		} while(anyRemoved);
	}

	memset(mTrackUsedBlock,0,sizeof(mTrackUsedBlock));
	for (i=0;i<kMaxTracks;i++)
	{
		// Flag used blocks
		for (j=0;j<mHighestUsedPosition[i];j++)
		{
			if (mTracks[i][j] < kMaxBlocks)
			{
				mTrackUsedBlock[mTracks[i][j]] = true;
			}
		}
	}
}


BlockEntry *MusicFile::ParseText(const char *line)
{
	if (!line)
	{
		return 0;
	}

	if (strlen(line) < 2)
	{
		return 0;
	}

	int num = -1;
	if (strlen(line) >= 5)
	{
		// Most commands are three chars plus ":" so assume this
		sscanf(line+4,"%02x",&num);
	}

	if ((_strnicmp(line,"ENV:",4) == 0) && (num >= 0) && (num < MusicFile::kMaxEnvelopes))
	{
		return new BlockEntryEnvelope(num);
	}

	if ((_strnicmp(line,"FLL:",4) == 0) && (num >= 0))
	{
		return new BlockEntryGeneric1("FilterLo","FLL",num);
	}

	if ((_strnicmp(line,"FLH:",4) == 0) && (num >= 0))
	{
		return new BlockEntryGeneric1("FilterHi","FLH",num);
	}

	if ((_strnicmp(line,"FLC:",4) == 0) && (num >= 0))
	{
		return new BlockEntryGeneric1("FilterControl","FLC",num);
	}

	if ((_strnicmp(line,"FLP:",4) == 0) && (num >= 0))
	{
		return new BlockEntryGeneric1("FilterPass","FLP",num);
	}

	if ((_strnicmp(line,"DUR:",4) == 0) && (num >= 0) && (num < 0x80))
	{
		mLastDurationParsed = num*2;
		return new BlockEntryGeneric1("Duration","DUR",num);
	}

	if ((_strnicmp(line,"VOL:",4) == 0) && (num >= 0) && (num <= 0x0f))
	{
		return new BlockEntryGeneric1("Volume","VOL",num);
	}

	if ((_strnicmp(line,"ARP:",4) == 0) && (num >= 0))
	{
		if (num)
		{
			return new BlockEntryArpeggio(num);
		}
		else
		{
			return new BlockEntryArpeggioStop();
		}
	}

	if (_strnicmp(line,"GL:",3) == 0)
	{
		int num1 = 0,num2 = 0;
		sscanf(line+3,"%02x,%02x",&num1,&num2);
		return new BlockEntryGlide(num1,num2);
	}

	if (_strnicmp(line,"FG:",3) == 0)
	{
		int num1 = 0,num2 = 0;
		sscanf(line+3,"%02x,%02x",&num1,&num2);
		return new BlockEntryFilterGlide(num1,num2);
	}

	if (_strnicmp(line,"SLE",3) == 0)
	{
		return new BlockEntrySlurEnable();
	}

	if (_strnicmp(line,"SLR",3) == 0)
	{
		return new BlockEntrySlurRelease();
	}

	if (_strnicmp(line,"HRE",3) == 0)
	{
		return new BlockEntryHardRestartEnable();
	}

	if (_strnicmp(line,"HRD",3) == 0)
	{
		return new BlockEntryHardRestartDisable();
	}

	if ((_strnicmp(line,"HAD:",4) == 0))
	{
		return new BlockEntryHardRestartAD(num);
	}

	if ((_strnicmp(line,"HSR:",4) == 0))
	{
		return new BlockEntryHardRestartSR(num);
	}

	if ((_strnicmp(line,"HWV:",4) == 0))
	{
		return new BlockEntryHardRestartWV(num);
	}

	if ((_strnicmp(line,"HTI:",4) == 0) && (num > 0))
	{
		return new BlockEntryHardRestartTI(num);
	}


	if ((_strnicmp(line,"ERP:",4) == 0) && (num >= 0) && (num <= MusicFile::kMaxExtendedArpeggios))
	{
		return new BlockEntryExtendedArpeggio(num);
	}

	if (_strnicmp(line,"ARS",3) == 0)
	{
		return new BlockEntryArpeggioStop();
	}

	if (_strnicmp(line,"VIB:",4) == 0)
	{
		int num1 = 0,num2 = 0;
		sscanf(line+4,"%02x,%02x",&num1,&num2);
		if ((num1 & 0x0f) && (num1 & 0xf0))
		{
			return new BlockEntrySmallVibrato(num1,num2);
		}
	}

	if ((_strnicmp(line,"VBD:",4) == 0))
	{
		return new BlockEntrySmallVibratoDelay(num);
	}

	if (_strnicmp(line,"VBS",3) == 0)
	{
		return new BlockEntrySmallVibratoStop();
	}

	if (_strnicmp(line,"FVB:",4) == 0)
	{
		int num1 = 0,num2 = 0;
		sscanf(line+4,"%02x,%02x",&num1,&num2);
		if (num1 > 0)
		{
			// Obviously cap it since the shift should be more than 16 bits ever
			if (num1 > 15)
			{
				num1 = 15;
			}
			return new BlockEntryGeneric2("FixedVibrato","FVB",num1,num2);
		}
	}

	if (_strnicmp(line,"DTI:",4) == 0)
	{
		int num1 = 0,num2 = 0;
		sscanf(line+4,"%02x,%02x",&num1,&num2);
		if ((num1 > 0) && (num2 <= num1) && (num2 > 0))
		{
			mLastDurationParsed = num1;
			return new BlockEntryGeneric2("DurationTicks","DTI",num1,num1-num2);
		}
		return 0;
	}

	if ((_strnicmp(line,"FVD:",4) == 0))
	{
		return new BlockEntryGeneric1("FixedVibratoDelay","FVD",num);
	}

	if (_strnicmp(line,"FVS",3) == 0)
	{
		return new BlockEntryGeneric("FixedVibratoStop","FBS");
	}

	if (_strnicmp(line,"+++",3) == 0)
	{
		return new BlockEntryGeneric("RestNote","+++");
	}

	if (_strnicmp(line,"===",3) == 0)
	{
		return new BlockEntryGeneric("RestNoteRelease","===");
	}

	if ((_strnicmp(line,"TWV:",4) == 0))
	{
		return new BlockEntryTableWaveSet(num);
	}

	if ((_strnicmp(line,"TNT:",4) == 0))
	{
		return new BlockEntryTableNoteSet(num);
	}

	if ((_strnicmp(line,"TPL:",4) == 0))
	{
		return new BlockEntryTablePulseSet(num);
	}

	if ((_strnicmp(line,"TFL:",4) == 0))
	{
		return new BlockEntryTableFilterSet(num);
	}


	// Otherwise try to parse what we think is a note
	int noteNum = -1;
	int note = tolower(line[0]);
	bool canSharp = false;
	switch(note)
	{
		case 'c':
			noteNum = 0;
			canSharp = true;
			break;

		case 'd':
			noteNum = 2;
			canSharp = true;
			break;

		case 'e':
			noteNum = 4;
			break;

		case 'f':
			noteNum = 5;
			canSharp = true;
			break;

		case 'g':
			noteNum = 7;
			canSharp = true;
			break;

		case 'a':
			noteNum = 9;
			canSharp = true;
			break;

		case 'b':
			noteNum = 11;
			break;
	}

	if (canSharp && strchr(line,'#'))
	{
		noteNum++;
	}

	if (noteNum >= 0)
	{
		size_t octavePos = strcspn(line,"01234567");
		if (octavePos != strlen(line))
		{
			int octave = line[octavePos] - '0';
			return new BlockEntryNote(noteNum + (octave * 12));
		}
	}
	
	return 0;
}


void MusicFile::AddBlockEntryCommon(const int block, BlockEntry *entry, const int theLine, const int blockTime)
{
	entry->SetBlockEntry(this,block,theLine,blockTime);
}

std::list<BlockEntry*>::iterator MusicFile::AddBlockEntry(const int block, BlockEntry *entry, const int theLine, const int blockTime)
{
	AddBlockEntryCommon(block,entry,theLine,blockTime);

	mBlocks[block].push_back(entry);
	std::list<BlockEntry*>::iterator ret = mBlocks[block].end();
	ret--;
	return ret;
}



void MusicFile::ClearBlock(const int block)
{
	if (! ((block >= 0) && (block<= 63)))
	{
		return;
	}

	std::list<BlockEntry*>::iterator st = mBlocks[block].begin();
	while (st != mBlocks[block].end())
	{
		delete *st;
		st++;
	}
	mBlocks[block].clear();
}

#define WriteEnvelopeProperty(prop)	\
	needComma = false;	\
	numUsed = 0;	\
	fprintf(fp,"Envelopes" #prop);	\
	for (i=0;i<kMaxEnvelopes;i++)	\
	{	\
		if (mEnvelopes[i].mIsUsed)	\
		{	\
		numUsed++;	\
		}	\
	}	\
	if (numUsed > 0)	\
	{	\
		fprintf(fp, " !by ");	\
		for (i=0;i<kMaxEnvelopes;i++)	\
		{	\
			if (mEnvelopes[i].mIsUsed)	\
			{	\
				if (needComma)	\
				{	\
					fprintf(fp,",");	\
				}	\
				fprintf(fp,"$%02x",mEnvelopes[i].m##prop);	\
				envelopeSize++;	\
				needComma = true;	\
			}	\
		}	\
	}	\
	fprintf(fp,"\n");


bool MusicFile::OptimiseAndWrite(	const char *acmeCommandLine,
									const char *acmeOptions,
									const char *addr,
									int &codeSize,
									int &trackSize,
									int &blockSize,
									int &envelopeSize,
									int &tableSize,
									const int startSong,
									const bool includeSoundEffectCode,
									const char *titleInfo,
									const char *authorInfo,
									const char *releasedInfo,
									const bool SIDIs6581,
									const bool runningInEditor
									)
{
	codeSize = 0;
	trackSize = 0;
	blockSize = 0;
	envelopeSize = 0;
	tableSize = 0;


	Optimise();

	bool outputSID = false;
	bool outputSelfRunning = false;

	if (_stricmp(addr,"sid") == 0)
	{
		outputSID = true;
	}
	else if (_stricmp(addr,"self") == 0)
	{
		outputSelfRunning = true;
	}

	DeleteFileA("t.lbl");
	DeleteFileA("t.map");
	DeleteFileA("t.sid");
	DeleteFileA("t.prg");

	// Produce ASM output
	FILE *fp;

	fp = fopen("t2.a","w");
	if (!fp)
	{
		printf("Couldn't open title output file for writing.\n");
		exit(-1);
	}
	const char *rtitleInfo = titleInfo;
	const char *rauthorInfo = authorInfo;
	const char *rreleasedInfo = releasedInfo;
	if (!rtitleInfo || (strlen(rtitleInfo) == 0))
	{
		rtitleInfo = "??";
	}
	if (!rauthorInfo || (strlen(rauthorInfo) == 0))
	{
		rauthorInfo = "??";
	}
	if (!rreleasedInfo || (strlen(rreleasedInfo) == 0))
	{
		rreleasedInfo = "20??";
	}
	fprintf(fp,"!tx \"%s\"\n!align 31,0,0\n!tx \"%s\"\n!align 31,0,0\n!tx \"%s\"\n!align 31,0,0\n",rtitleInfo,rauthorInfo,rreleasedInfo);

	if (SIDIs6581)
	{
		fprintf(fp,"!by 0,$10\n");
	}
	else
	{
		fprintf(fp,"!by 0,$20\n");
	}

	fclose(fp);



	fp = fopen("t3.a","w");
	if (!fp)
	{
		printf("Couldn't open title output file for writing.\n");
		exit(-1);
	}
	fprintf(fp,"!by CHR_LightRed\n!tx \"Title: \"\n!by CHR_LightGreen\n!tx \"%s\"\n!by CHR_Return\n!by CHR_LightRed\n!tx \"Author: \"\n!by CHR_LightGreen\n!tx \"%s\"\n!by CHR_Return\n!by CHR_LightRed\n!tx \"Released: \"\n!by CHR_LightGreen\n!tx \"%s\"\n!by CHR_Return\n",rtitleInfo,rauthorInfo,rreleasedInfo);

	fclose(fp);





	fp = fopen("t.a","w");
	if (!fp)
	{
		printf("Couldn't open output file file for writing.\n");
		exit(-1);
	}
	if (outputSID)
	{
		// SID output always uses $900
		fprintf(fp,"!pseudopc $900 {\n");
		if (includeSoundEffectCode)
		{
			// If including sound effects with the SID then create a new header.
			fprintf(fp,"jmp MusicStudioInterface\njmp *\n!align 255,0\n");
		}
	}
	else if (outputSelfRunning)
	{
		// Self running output always uses $900
		fprintf(fp,"*=$900\n");
	}
	else
	{
		fprintf(fp,"*=%s\n",addr);
	}
	int i,j;

	// Enable options in the code depending on the features used.
	if (runningInEditor)
	{
		fprintf(fp,"kRunningInEditor = 1\n");
	}

	if (SIDIs6581)
	{
		fprintf(fp,"kEditorSIDIs6581 = 1\n");
	}

	if (includeSoundEffectCode)
	{
		fprintf(fp,"SoundEffectsActive = 1\n");
	}

	if (mSluringCommandsActive)
	{
		fprintf(fp,"SluringCommandsActive = 1\n");
	}

	if (mHardRestartCommandsActive)
	{
		fprintf(fp,"HardRestartCommandsActive = 1\n");
	}

	if (mExtendedArpeggioUsed)
	{
		fprintf(fp,"ExtendedArpeggioCommandsActive = 1\n");
	}

	if (mArpeggioUsed)
	{
		fprintf(fp,"ArpeggioCommandsActive = 1\n");
	}

	if (mArpeggioStopUsed)
	{
		fprintf(fp,"ArpeggioStopCommandsActive = 1\n");
	}

	if (mSmallVibratoUsed)
	{
		fprintf(fp,"SmallVibratoCommandsActive = 1\n");
	}

	if (mPulseWidthVarianceUsed)
	{
		fprintf(fp,"PulseWidthVarianceActive = 1\n");
	}

	if (mDrumDynamicStartOffsetUsed)
	{
		fprintf(fp,"DrumDynamicStartOffsetUsed = 1\n");
	}

	if (mDrumGateLengthUsed)
	{
		fprintf(fp,"DrumGateLengthUsed = 1\n");
	}

	if (startSong < 0)
	{
		fprintf(fp,"kStartSong = %d\nOverrideWithTrackPointers = 1\n",-startSong);
	}

	if (startSong < 0)
	{
		fprintf(fp,"OverrideWithTrackPointers = 1\n");
	}

	fprintf(fp,"ZeroPageStart = $%x\n",mZeroPageStart);

	// Assembly wave table code is always active, however the following tables may not be active.
	if (TableMaxPos(1) >= 0)
	{
		fprintf(fp,"TableCodeActivateNote = 1\n");
	}
	if (TableMaxPos(2) >= 0)
	{
		fprintf(fp,"TableCodeActivatePulse = 1\n");
	}
	if (TableMaxPos(3) >= 0)
	{
		fprintf(fp,"TableCodeActivateFilter = 1\n");
	}

	for (i=1;i<kMaxTracks;i++)
	{
		if (mHardRestartAttackDecay[0] != mHardRestartAttackDecay[i])
		{
			mVariableHardRestartParams = true;
			break;
		}

		if (mHardRestartSustainRelease[0] != mHardRestartSustainRelease[i])
		{
			mVariableHardRestartParams = true;
			break;
		}

		if (mHardRestartWaveform[0] != mHardRestartWaveform[i])
		{
			mVariableHardRestartParams = true;
			break;
		}

		if (mHardRestartFrames[0] != mHardRestartFrames[i])
		{
			mVariableHardRestartParams = true;
			break;
		}
	}

	if (mVariableHardRestartParams)
	{
		fprintf(fp,"VariableHardRestartParams = 1\n");
	}
	else
	{
		fprintf(fp,"HardRestartSIDParamAD = $%x\n",mHardRestartAttackDecay[0]);
		fprintf(fp,"HardRestartSIDParamSR = $%x\n",mHardRestartSustainRelease[0]);
		fprintf(fp,"HardRestartSIDParamWV = $%x\n",mHardRestartWaveform[0]);
		fprintf(fp,"HardRestartSIDParamTime = $%x\n",mHardRestartFrames[0]);
	}

	std::set<std::string,ThisLess>::iterator st = mAutoUsedFeatures.begin();
	while (st != mAutoUsedFeatures.end())
	{
		fprintf(fp,"%s = 1\n",(*st).c_str());
		st++;
	}


	fprintf(fp,"!source \"MusicPlayer2.a\"\n");

	if (outputSID && includeSoundEffectCode)
	{
		fprintf(fp,"!source \"HeaderSID2.a\"\n");
	}

	for (i=0;i<kMaxTracks;i++)
	{
		fprintf(fp,"Track%d\n",i);
		DumpAsASM(fp,mTracks[i],mHighestUsedPosition[i]+1);
		trackSize += mHighestUsedPosition[i]+1;
	}

	fprintf(fp,"BlockIndexLo\n");
	for (i=0;i<kMaxBlocks;i++)
	{
		if (mTrackUsedBlock[i])
		{
			fprintf(fp,"\t!by <Block%02x\n",mRealBlockIndex[i]);
			blockSize++;
		}
	}
	fprintf(fp,"BlockIndexHi\n");
	for (i=0;i<kMaxBlocks;i++)
	{
		if (mTrackUsedBlock[i])
		{
			fprintf(fp,"\t!by >Block%02x\n",mRealBlockIndex[i]);
			blockSize++;
		}
	}

	for (i=0;i<kMaxBlocks;i++)
	{
		if (mTrackUsedBlock[i])
		{
			mCurrentBlockTranspose = 0;
			fprintf(fp,"Block%02x\n",mRealBlockIndex[i]);
			std::list<BlockEntry*>::iterator st = mBlocks[i].begin();
			while (st != mBlocks[i].end())
			{
				fprintf(fp,"\t!by ");
				blockSize += (*st)->WriteASM(fp);
				fprintf(fp,"\n");
				st++;
			}

			fprintf(fp,"\t!by <BlockCommandEndBlock\n");
			blockSize++;
		}
	}

	tableSize += WriteTable(fp,0,"Wave");
	tableSize += WriteTable(fp,1,"Note");
	tableSize += WriteTable(fp,2,"Pulse");
	tableSize += WriteTable(fp,3,"Filter");

	// Write out the envelope information in a different format to enable 256 envelopes
	bool needComma;
	int numUsed;
	WriteEnvelopeProperty(AttackDecay);
	WriteEnvelopeProperty(SustainRelease);
	WriteEnvelopeProperty(TableWave);
	WriteEnvelopeProperty(TableNote);
	WriteEnvelopeProperty(TablePulse);
	WriteEnvelopeProperty(TableFilter);
	WriteEnvelopeProperty(TableBitMask);
	WriteEnvelopeProperty(MiscBitMask);

	// Pre-calculate track starting positions and store in a table.
	for (i=0;i<kMaxTracks;i++)
	{
		fprintf(fp,"TrackStart%d !by 0\n",i);
		trackSize++;
		// Flag used blocks
		for (j=1;j<mHighestUsedPosition[i];j++)
		{
			if (mTracks[i][j] >= kMusicPlayer_StopTrack)
			{
				fprintf(fp,"\t!by %d\n",j+1);
				trackSize++;
			}
		}
	}

	if (mExtendedArpeggioUsed && mMaxExtendedArpeggioData)
	{
		fprintf(fp,"ExtendedArpeggioData\n");
		DumpAsASM(fp,mExtendedArpeggioData,mMaxExtendedArpeggioData);
		envelopeSize += mMaxExtendedArpeggioData;
	}

	if (mVariableHardRestartParams)
	{
		fprintf(fp,"TableHardRestartAttackDecay\n");
		DumpAsASM(fp,mHardRestartAttackDecay,kMaxTracks);
		fprintf(fp,"TableHardRestartSustainRelease\n");
		DumpAsASM(fp,mHardRestartSustainRelease,kMaxTracks);
		fprintf(fp,"TableHardRestartWaveform\n");
		DumpAsASM(fp,mHardRestartWaveform,kMaxTracks);
		fprintf(fp,"TableHardRestartFrames\n");
		DumpAsASM(fp,mHardRestartFrames,kMaxTracks);
	}

	fprintf(fp,"kNumSongs = TrackStart1 - TrackStart0\n");
	if (startSong < 0)
	{
		fprintf(fp,"kStartSong = %d\n",-startSong);
	}
	else
	{
		fprintf(fp,"kStartSong = %d\n",startSong+1);
	}

	if (outputSID)
	{
		fprintf(fp,"}\n");
	}

	fclose(fp);


	// Now optimise and save the data from the classes and produce an assembler file that
	// includes a new version of the music player.

	// Remember to change the player code so that it can easily play sound effects, with
	// arpeggio and on a certain channel or at least the least recently used channel.
	
	// Whilst saving note which effects are used and produce defines that will enable or
	// disable the various effect code.
	// Can use UpdateReferences() to decide what is really used.

	// Remember that <BlockCommand* must be >= 128

	// Then spawn Acme to assemble the file.


	// Investigate if the save routine can spot the "endless loop problem" and save data
	// that never produces this problem.

	//	; MPi: TODO: There is scope for moving the initialisation of .core2 and .envDataP5_1 etc
	//	; into the note play once only upon a sound change.

	char theCommand[1024];
	if (outputSID)
	{
		sprintf(theCommand,"%sHeaderSID.a",acmeOptions);
	}
	else if (outputSelfRunning)
	{
		sprintf(theCommand,"%sHeaderSelf.a",acmeOptions);
	}
	else
	{
		sprintf(theCommand,"%sHeaderPRG.a",acmeOptions);
	}

	// Starts the command and places the output in the current directory
	OutputDebugStringA(acmeCommandLine);
	OutputDebugStringA(" ");
	OutputDebugStringA(theCommand);
#if 0
	const char *argv[] = {acmeCommandLine,theCommand,0};
	intptr_t ret = _spawnvp(_P_WAIT,acmeCommandLine,argv);
#else
	STARTUPINFOA			si;
	PROCESS_INFORMATION		pi;
	
	memset( &si, 0, sizeof(si) );
	si.cb = sizeof(si);

	std::string path = acmeCommandLine;
	path += " ";
	path += theCommand;

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
#endif

	fp = fopen("t.prg","r");
	if (fp)
	{
		fclose(fp);
	}
	else
	{
		fp = fopen("t.sid","r");
		if (fp)
		{
			fclose(fp);
		}
		else
		{
			theLastError = 0x1234567;
		}
	}

	if (theLastError)
	{
		char error[MAX_PATH + 128];
 		sprintf(error,"Command \"%s\" failed with error code %x. Please send the music file and an explanation of what you were trying to do to martin.piper@gmail.com\n",path.c_str(),theLastError);
		MessageBoxA(0,error,"Error!",MB_OK);
		return false;
	}


	mC64Labels.clear();
	int codeStart = 0 , codeEnd = 0;
	fp = fopen("t.lbl","r");
	if (fp)
	{
		// Look for "al C:xxxx .MusicPlayerCodeStart" and "al C:yyyy .MusicPlayerCodeEnd" to
		// calculate the code size.
		// Also add the labels and values to a std::map
		while (!feof(fp))
		{
			char line[256];
			fgets(line,sizeof(line),fp);
			int theValue = 0;
			sscanf(line,"al C:%04x",&theValue);
			if (strstr(line,"MusicPlayerCodeStart"))
			{
				codeStart = theValue;
			}
			else if (strstr(line,"MusicPlayerCodeEnd"))
			{
				codeEnd = theValue;
			}

			char *delim = " \r\n";
			if (strtok(line,delim))
			{
				if (strtok(0,delim))
				{
					char *label = strtok(0,delim);
					if (label)
					{
						mC64Labels.insert(std::pair<std::string,int>(std::string(label),theValue));
					}
				}
			}
		}
		fclose(fp);
	}

	codeSize = codeEnd - codeStart;
	mC64Tracks[0] = GetAddressFromLabel(".Track0");
	mC64Tracks[1] = GetAddressFromLabel(".Track1");
	mC64Tracks[2] = GetAddressFromLabel(".Track2");
	mC64trcn = GetAddressFromLabel(".trcn");
	mC64blcn = GetAddressFromLabel(".blcn");
	return true;
}

std::string MusicFile::GetBlockText(const int block)
{
	if (! ((block >= 0) && (block<= 63)))
	{
		return std::string("");
	}

	std::string theBlockData;

	std::list<MusicStudio1::BlockEntry*>::iterator st,en;
	st = mBlocks[block].begin();
	en = mBlocks[block].end();
	while (st != en)
	{
		MusicStudio1::BlockEntry *entry = *st++;
		char line[256];
		line[0] = 0;
		entry->GetFormatted(line);
		theBlockData += line;
		theBlockData += "\r\n";
	}

	return theBlockData;
}

void MusicFile::SetBlockFromText(const int block, const char *text,std::list<int> &errorReport,int &resultantSize,int &totalDuration)
{
	errorReport.clear();
	resultantSize = 0;
	totalDuration = 0;
	int currentDuration = 0;
	if (! ((block >= 0) && (block < kMaxBlocks)))
	{
		return;
	}

	ClearBlock(block);
	mLastDurationParsed = -1;
	mLastNoteAdded = mBlocks[block].end();
	mInternalSlurCount = 0;
	bool internalSlurAdded = false;
	std::list<BlockEntry*>::iterator lastTrippleminusCommand;

	char *toTok = _strdup(text);

	int theLine = 0;
	char *tok;
	tok = strtok(toTok,"\n");
	while (tok)
	{
		std::string trimmed(tok);
		size_t found = trimmed.find_first_of(" \t\r");
		while (found != std::string::npos)
		{
			trimmed.erase(found);
			found = trimmed.find_first_of(" \t\r");
		}

		if (!trimmed.empty())
		{
			bool ignoreSlurChecks = false;
			bool isTrippleMinusCommand = false;
			BlockEntry *entry = 0;
			if ((trimmed == "---") && (mLastDurationParsed > 0))
			{
				// Insert extra notes for the "---" operator updating any previously found duration command
				if (mLastNoteAdded != mBlocks[block].end())
				{
					ignoreSlurChecks = true;
					char line[256];
					(*mLastNoteAdded)->GetFormatted(line);
					entry = ParseText(line);
					isTrippleMinusCommand = true;
					if (mInternalSlurCount == 0)
					{
						if (!internalSlurAdded)
						{
							internalSlurAdded = true;
							BlockEntry *theSlur = new BlockEntrySlurEnable();
							AddBlockEntryCommon(block,theSlur,-1,(*mLastNoteAdded)->GetBlockTime());
							mBlocks[block].insert(mLastNoteAdded,theSlur);
							resultantSize += theSlur->GetByteSize();
						}
					}
				}
			}
			else
			{
				entry = ParseText(trimmed.c_str());
			}
			if (entry)
			{
				bool isNote = false;
				if (((trimmed == "===") || (trimmed == "+++") || entry->IsNote()) && (mLastDurationParsed > 0))
				{
					currentDuration = totalDuration;
					totalDuration += mLastDurationParsed;
					isNote = true;
				}
				if (!ignoreSlurChecks && entry->IsNote() && internalSlurAdded)
				{
					internalSlurAdded = false;
					BlockEntry *theSlurRelease = new BlockEntrySlurRelease();
					AddBlockEntryCommon(block,theSlurRelease,-1,(*lastTrippleminusCommand)->GetBlockTime());
					mBlocks[block].insert(lastTrippleminusCommand,theSlurRelease);
					resultantSize += theSlurRelease->GetByteSize();
				}
				std::list<BlockEntry*>::iterator ret;
				if (isNote)
				{
					ret = AddBlockEntry(block,entry,theLine,currentDuration);
				}
				else
				{
					ret = AddBlockEntry(block,entry,theLine,totalDuration);
				}
				if (!ignoreSlurChecks && entry->IsNote())
				{
					mLastNoteAdded = ret;
				}
				if (isTrippleMinusCommand)
				{
					lastTrippleminusCommand = ret;
				}

				resultantSize += entry->GetByteSize();
			}
			else
			{
				if (trimmed[0] != ';')
				{
					errorReport.push_back(theLine);
				}
			}
		}
		tok = strtok(0,"\n");
		theLine++;
	}

	if (internalSlurAdded)
	{
		internalSlurAdded = false;
		BlockEntry *theSlurRelease = new BlockEntrySlurRelease();
		AddBlockEntryCommon(block,theSlurRelease,-1,(*lastTrippleminusCommand)->GetBlockTime());
		mBlocks[block].insert(lastTrippleminusCommand,theSlurRelease);
		resultantSize += theSlurRelease->GetByteSize();
	}

	delete toTok;
}

int MusicFile::GetAddressFromLabel(const char *label)
{
	int address = 0;
	std::map<std::string,int,ThisLess>::iterator found = mC64Labels.find(std::string(label));
	if (found != mC64Labels.end())
	{
		address = (*found).second;
	}
	return address;
}

int MusicFile::MapTrackPosToOptimisedPos(const int track,const int pos)
{
	int mapped = 0;
	int i;
	for (i=0;i<kMaxTrackLength;i++)
	{
		if (mTrackReverseIndex[track][i] <= pos)
		{
			mapped = i;
		}
		else if (mTrackReverseIndex[track][i] > pos)
		{
			break;
		}
	}
	return mapped;
}

void MusicFile::MarkExecutedTableBytes(unsigned char* tableControl,unsigned char* tableValue,bool* tableUsed,bool* tableUsedJump,int start)
{
	if (start > 0)
	{
		tableUsedJump[start-1] = true;
	}
	while (start < (kMaxTableEntries+1) && (start > 0))
	{
		if (tableUsed[start-1])
		{
			// Stop processing, we have already processed this table entry
			break;
		}
		tableUsed[start-1] = true;
		if (tableControl[start-1] == 0xff)
		{
			start = tableValue[start-1];
			if (start > 0)
			{
				tableUsedJump[start-1] = true;
			}
			continue;
		}
		start++;
	}
}

int MusicFile::TableMaxPos(const int table)
{
	int maxPos = -1;
	int i;
	for (i=kMaxTableEntries-1;i>=0;i--)
	{
		if (mTablesUsedBytes[table][i])
		{
			maxPos = i;
			break;
		}
	}

	return maxPos;
}

int MusicFile::WriteTable(FILE *fp,const int table,const char *name)
{
	// Saves out the maximum used table entry
	int maxPos = TableMaxPos(table);
	int i;

	if (table == 1)
	{
		fprintf(fp,"TableControlLN%s",name);
		DumpAsASM(fp,mTablesControls[1],maxPos+1,0,0x0f);
		fprintf(fp,"TableControlCommandJumpLo%s",name);
		DumpAsASM(fp,mTablesControls[1],maxPos+1,4,0x0f,"<NoteTableCommand");

		bool anySplitTablesNeeded = false;
		for (i=0;i<kMaxTableEntries;i++)
		{
			if (((mTablesControls[1][i] & 0xf0) == 0x80) || ((mTablesControls[1][i] & 0xf0) == 0x90))
			{
				anySplitTablesNeeded = true;
				break;
			}
		}

		if (anySplitTablesNeeded)
		{
			// Outputs the lower nybble if it is command 8 or 9 or leaves the value alone otherwise
			unsigned char temp[kMaxTableEntries];
			memcpy(temp,mTablesValues[1],sizeof(mTablesValues[1]));
			for (i=0;i<kMaxTableEntries;i++)
			{
				if (((mTablesControls[1][i] & 0xf0) == 0x80) || ((mTablesControls[1][i] & 0xf0) == 0x90))
				{
					temp[i] = (temp[i] & 0x0f)+1;
				}
			}
			fprintf(fp,"TableValueLN%s\t",name);
			DumpAsASM(fp,temp,maxPos+1);

			memcpy(temp,mTablesValues[1],sizeof(mTablesValues[1]));
			for (i=0;i<kMaxTableEntries;i++)
			{
				if ((mTablesControls[1][i] & 0xf0) == 0x80)
				{
					temp[i] = (temp[i] >> 4)+1;
				}
				else if ((mTablesControls[1][i] & 0xf0) == 0x90)
				{
					temp[i] = ~(temp[i] >> 4);
				}
			}
			fprintf(fp,"TableValueHN%s\t",name);
			DumpAsASM(fp,temp,maxPos+1);
		}
		else
		{
			fprintf(fp,"TableValueLN%s\n",name);
			fprintf(fp,"TableValueHN%s\n",name);
			DumpAsASM(fp,mTablesValues[table],maxPos+1);
		}
	}
	else
	{
		fprintf(fp,"TableControl%s",name);
		DumpAsASM(fp,mTablesControls[table],maxPos+1);
		fprintf(fp,"TableValue%s\t",name);
		DumpAsASM(fp,mTablesValues[table],maxPos+1);
	}

	return (maxPos+1)*2;
}

bool MusicFile::OptimiseTables(const bool assumeAllEnvelopesUsed)
{
	bool changed = false;
	int i,j;
	// First sanity check the data for jumps to the same offset and zero them instead.
	// This saves raster time by terminating a wave table instead of letting it loop around all the time.
	for (j=0;j<kMaxTables;j++)
	{
		for (i=0;i<kMaxTableEntries;i++)
		{
			if ((mTablesControls[j][i] == 0xff) && (mTablesValues[j][i] == (i+1)))
			{
				mTablesValues[j][i] = 0;
				changed = true;
			}
		}
	}
	
	// More sanity checking. This time avoid tables with one entry from looping if the used
	// commands have no further effect.
	for (i=0;i<kMaxTableEntries-1;i++)
	{
		// Stop a one entry wave table from looping back to itself.
		if ((mTablesControls[0][i] != 0xff) && (mTablesControls[0][i+1] == 0xff) && (mTablesValues[0][i+1] == (i+1)))
		{
			mTablesValues[0][i+1] = 0;
			changed = true;
		}

		// Stop a one entry note table from looping back to itself if the command has little effect.
		if (((mTablesControls[1][i] & 0xf0) == 0x10) && (mTablesControls[1][i+1] == 0xff) && (mTablesValues[1][i+1] == (i+1)))
		{
			mTablesValues[1][i+1] = 0;
			changed = true;
		}
		if (((mTablesControls[1][i] & 0xf0) == 0x00) && (mTablesControls[1][i+1] == 0xff) && (mTablesValues[1][i+1] == (i+1)))
		{
			mTablesValues[1][i+1] = 0;
			changed = true;
		}

		// Stop a one entry pulse table from looping back to itself if the command has little effect.
		if (((mTablesControls[2][i] & 0xf0) == 0x01) && (mTablesControls[2][i+1] == 0xff) && (mTablesValues[2][i+1] == (i+1)))
		{
			mTablesValues[2][i+1] = 0;
			changed = true;
		}
	}

	// First find duplicate wave tables (taking into account their jumps) and renumber
	// the shorter (or later) table to fit within the other table.
	for (i=0;i<kMaxEnvelopes;i++)
	{
		if (mEnvelopes[i].mIsUsed || assumeAllEnvelopesUsed)
		{
			unsigned char *usedOffset[kMaxTables];
			memset(usedOffset,0,sizeof(usedOffset));
			if (mEnvelopes[i].mActiveTableWave)
			{
				usedOffset[0] = &mEnvelopes[i].mTableWave;
			}
			if (mEnvelopes[i].mActiveTableNote)
			{
				usedOffset[1] = &mEnvelopes[i].mTableNote;
			}
			if (mEnvelopes[i].mActiveTablePulse)
			{
				usedOffset[2] = &mEnvelopes[i].mTablePulse;
			}
			if (mEnvelopes[i].mActiveTableFilter)
			{
				usedOffset[3] = &mEnvelopes[i].mTableFilter;
			}
			for (j=0;j<kMaxTables;j++)
			{
				if (!usedOffset[j])
				{
					continue;
				}
				if (!usedOffset[j][0])
				{
					continue;
				}
				int a;
				bool found = false;
				// Look for the same sequence earlier in the table
				for (a=0;a<usedOffset[j][0]-1 && !found;a++)
				{
					int b;
					for (b=0;b<kMaxTableEntries && !found;b++)
					{
						if (mTablesControls[j][usedOffset[j][0]-1+b] == 0xff)
						{
							if (mTablesControls[j][a+b] != 0xff)
							{
								break;
							}
							if (mTablesValues[j][usedOffset[j][0]-1+b] == 0 &&
								mTablesValues[j][a+b] == 0)
							{
								found = true;
								usedOffset[j][0] = a+1;
								break;
							}
							// If the relative value for the jump is the same then we have a match
							if (((usedOffset[j][0]+b) - mTablesValues[j][usedOffset[j][0]-1+b]) == ((a+b+1) - mTablesValues[j][a+b]))
							{
								found = true;
								usedOffset[j][0] = a+1;
								break;
							}
						}
						else if (mTablesControls[j][usedOffset[j][0]-1+b] != mTablesControls[j][a+b])
						{
							break;
						}
						else if (mTablesValues[j][usedOffset[j][0]-1+b] != mTablesValues[j][a+b])
						{
							break;
						}
					}
				}
			}
		}
	}

	// Calculate from the used envelopes what table entries are really used.
	memset(mTablesUsedBytes,0,sizeof(mTablesUsedBytes));
	memset(mTablesUsedByJump,0,sizeof(mTablesUsedByJump));
	for (i=0;i<kMaxEnvelopes;i++)
	{
		if (mEnvelopes[i].mIsUsed || assumeAllEnvelopesUsed)
		{
			mEnvelopes[i].mTableBitMask = 0;
			if (mEnvelopes[i].mActiveTableWave)
			{
				mEnvelopes[i].mTableBitMask |= 1<<0;
			}
			if (mEnvelopes[i].mActiveTableNote)
			{
				mEnvelopes[i].mTableBitMask |= 1<<1;
			}
			if (mEnvelopes[i].mActiveTablePulse)
			{
				mEnvelopes[i].mTableBitMask |= 1<<2;
			}
			if (mEnvelopes[i].mActiveTableFilter)
			{
				mEnvelopes[i].mTableBitMask |= 1<<3;
			}

			if (mEnvelopes[i].mActiveTableWave && mEnvelopes[i].mTableWave)
			{
				MarkExecutedTableBytes(mTablesControls[0],mTablesValues[0],mTablesUsedBytes[0],mTablesUsedByJump[0],mEnvelopes[i].mTableWave);
			}
			if (mEnvelopes[i].mActiveTableNote && mEnvelopes[i].mTableNote)
			{
				MarkExecutedTableBytes(mTablesControls[1],mTablesValues[1],mTablesUsedBytes[1],mTablesUsedByJump[1],mEnvelopes[i].mTableNote);
			}
			if (mEnvelopes[i].mActiveTablePulse && mEnvelopes[i].mTablePulse)
			{
				MarkExecutedTableBytes(mTablesControls[2],mTablesValues[2],mTablesUsedBytes[2],mTablesUsedByJump[2],mEnvelopes[i].mTablePulse);
			}
			if (mEnvelopes[i].mActiveTableFilter && mEnvelopes[i].mTableFilter)
			{
				MarkExecutedTableBytes(mTablesControls[3],mTablesValues[3],mTablesUsedBytes[3],mTablesUsedByJump[3],mEnvelopes[i].mTableFilter);
			}

			// Calculate any other bitmasks
			mEnvelopes[i].mMiscBitMask = 0;
			if (mEnvelopes[i].mAllowVoiceEffects)
			{
				mEnvelopes[i].mMiscBitMask |= 1<<7;
			}
		}
	}

	for (i=0;i<kMaxBlocks;i++)
	{
		if (assumeAllEnvelopesUsed || mTrackUsedBlock[i])
		{
			std::list<BlockEntry*>::iterator st = mBlocks[i].begin();
			while (st != mBlocks[i].end())
			{
				(*st)->UpdateTableReferences();
				st++;
			}
		}
	}


	// Make sure unused bytes are zeroed out for the document view optimisation.
	for (j=0;j<kMaxTables;j++)
	{
		for (i=0;i<kMaxTableEntries;i++)
		{
			if (!mTablesUsedBytes[j][i])
			{
				mTablesControls[j][i] = 0;
				mTablesValues[j][i] = 0;
			}
		}
	}

	for (i=0;i<kMaxTables;i++)
	{
		// Keep on looping until nothing has been optimised this pass
		bool optimised = false;
		do
		{
			optimised = false;
			int j;

			// Remove any unused bytes
			for (j=0;j<kMaxTableEntries-1;j++)
			{
				if (!mTablesUsedBytes[i][j])
				{
					int j2;
					bool valid = false;
					for (j2 = j+1; j2 < kMaxTableEntries; j2++)
					{
						if (mTablesUsedBytes[i][j2])
						{
							valid = true;
							break;
						}
					}
					if (!valid)
					{
						continue;
					}
					optimised = true;
					RemoveTableByteAt(i,j);
					changed = true;
				}
			}
		} while(optimised);
	}

	// Spot adjacent duplicate commands in some tables (command sensitive) and if they are
	// not directly jumped into then pack them together.
	for (i=0;i<kMaxTableEntries-1;i++)
	{
		// Wave commands testing
		if (mTablesUsedBytes[0][i] && !mTablesUsedByJump[0][i+1])
		{
			if (mTablesControls[0][i] == mTablesControls[0][i+1])
			{
				if ( (int(mTablesValues[0][i]) + int(mTablesValues[0][i+1]+1)) <= 255 )
				{
					mTablesValues[0][i] += mTablesValues[0][i+1]+1;
					RemoveTableByteAt(0,i+1);
					i--;
					changed = true;
					continue;
				}
			}
		}
	}

	// For tables entries that jump within their own sequence spot two or more sequences and merge them.
	for (i=0;i<kMaxTables;i++)
	{
		// Keep on looping until nothing has been optimised this pass
		bool optimised = false;
		do
		{
			optimised = false;
			int j;

			for (j=0;j<kMaxTableEntries-1;j++)
			{
				// Hop out if there was an optimisation done
				if (optimised)
				{
					break;
				}
				if (!mTablesUsedBytes[i][j])
				{
					break;
				}
				// Find the ending jump command
				int k;
				for (k=j+1;k<kMaxTableEntries-1;k++)
				{
					// Hop out if there was an optimisation done
					if (optimised)
					{
						break;
					}
					if (!mTablesUsedBytes[i][k])
					{
						// Next sequence please
						j = k;
						break;
					}
					if (mTablesControls[i][k] == 0xff)
					{
						// Not a repeating jump so ignore it
						if (!mTablesValues[i][k])
						{
							// Next sequence please
							j = k;
							break;
						}
						// Is the jump self contained in this sequence?
						if (((mTablesValues[i][k]-1) >= j) && ((mTablesValues[i][k]-1) < k))
						{
							// Yes, so consider it for optimisation
							int maxLoop = (k-j)/2;
							int theLoop = maxLoop;
							// Remove longer loops first
							while (theLoop > 1)
							{
								// Hop out if there was an optimisation done
								if (optimised)
								{
									break;
								}
								bool exact = true;
								int l;
								for (l=0;l<theLoop;l++)
								{
									// Paranoia, stop unused sections being considered
									if (!mTablesUsedBytes[i][j+theLoop])
									{
										exact = false;
										break;
									}
									// Paranoia, stop jump commands being considered
									if (mTablesControls[i][j+l+theLoop] == 0xff)
									{
										exact = false;
										break;
									}
									if ((mTablesControls[i][j+l] != mTablesControls[i][j+l+theLoop]) || (mTablesValues[i][j] != mTablesValues[i][j+l+theLoop]))
									{
										exact = false;
										break;
									}
								}
								if (exact)
								{
									// There was a duplicate exact sequence match so find which one to optimise
									// by not using the one with jumps marked in it
									bool usedJump = false;
									for (l=0;l<theLoop;l++)
									{
										// If the two jump to positions are equal then they can be packed as well
										if (mTablesUsedByJump[i][j+l] == mTablesUsedByJump[i][j+l+theLoop])
										{
											continue;
										}
										// If there is a lone jump to position don't consider it
										if (mTablesUsedByJump[i][j+l])
										{
											usedJump = true;
											break;
										}
									}
									if (!usedJump)
									{
										while(theLoop > 0)
										{
											RemoveTableByteAt(i,j);
											theLoop--;
										}
										optimised = true;
										break;
									}

									usedJump = false;
									for (l=0;l<theLoop;l++)
									{
										if (mTablesUsedByJump[i][j+l+theLoop])
										{
											usedJump = true;
											break;
										}
									}
									if (!usedJump)
									{
										while(theLoop > 0)
										{
											RemoveTableByteAt(i,j+theLoop);
											theLoop--;
											changed = true;
										}
										optimised = true;
										break;
									}
								}
								theLoop--;
							}
						}

						// Next sequence please
						j = k;
						break;
					}
				}
			}
		} while(optimised);
	}

	return changed;
}

void MusicFile::RemoveTableByteAt(const int table, const int position)
{
	int i = table;
	int j = position;
	bool deletedJump = (mTablesControls[i][j] == 0xff);
	memmove(mTablesControls[i]+j,mTablesControls[i]+j+1,(kMaxTableEntries-j)-1);
	memmove(mTablesValues[i]+j,mTablesValues[i]+j+1,(kMaxTableEntries-j)-1);
	memmove(mTablesUsedBytes[i]+j,mTablesUsedBytes[i]+j+1,(kMaxTableEntries-j)-1);
	memmove(mTablesUsedByJump[i]+j,mTablesUsedByJump[i]+j+1,(kMaxTableEntries-j)-1);
	mTablesControls[i][kMaxTableEntries-1]= 0;
	mTablesValues[i][kMaxTableEntries-1]= 0;
	mTablesUsedBytes[i][kMaxTableEntries-1]= false;
	mTablesUsedByJump[i][kMaxTableEntries-1]= false;
	int k;
	// Renumber jumps
	for (k=0;k<kMaxTableEntries;k++)
	{
		if (mTablesControls[i][k] == 0xff)
		{
			if (mTablesValues[i][k] >= j+1)
			{
				mTablesValues[i][k]--;
			}
		}
	}
	// Renumber envelope offsets
	for (k=0;k<kMaxEnvelopes;k++)
	{
		switch(i)
		{
			default:
			{
				assert(!"Should not ever trigger");
				break;
			}
			case 0:
			{
				if ((mEnvelopes[k].mTableWave == j+1) && deletedJump)
				{
					mEnvelopes[k].mTableWave = 0;
				}
				else if (mEnvelopes[k].mTableWave > j+1)
				{
					mEnvelopes[k].mTableWave--;
				}
				break;
			}
			case 1:
			{
				if ((mEnvelopes[k].mTableNote == j+1) && deletedJump)
				{
					mEnvelopes[k].mTableNote = 0;
				}
				else if (mEnvelopes[k].mTableNote > j+1)
				{
					mEnvelopes[k].mTableNote--;
				}
				break;
			}
			case 2:
			{
				if ((mEnvelopes[k].mTablePulse == j+1) && deletedJump)
				{
					mEnvelopes[k].mTablePulse = 0;
				}
				else if (mEnvelopes[k].mTablePulse > j+1)
				{
					mEnvelopes[k].mTablePulse--;
				}
				break;
			}
			case 3:
			{
				if ((mEnvelopes[k].mTableFilter == j+1) && deletedJump)
				{
					mEnvelopes[k].mTableFilter = 0;
				}
				else if (mEnvelopes[k].mTableFilter > j+1)
				{
					mEnvelopes[k].mTableFilter--;
				}
				break;
			}
		}
	}
}

void MusicFile::AddTableByteAt(const int table, const int position)
{
	int i = table;
	int j = position;
	memmove(mTablesControls[i]+j+1,mTablesControls[i]+j,(kMaxTableEntries-j)-1);
	memmove(mTablesValues[i]+j+1,mTablesValues[i]+j,(kMaxTableEntries-j)-1);
	memmove(mTablesUsedBytes[i]+j+1,mTablesUsedBytes[i]+j,(kMaxTableEntries-j)-1);
	memmove(mTablesUsedByJump[i]+j+1,mTablesUsedByJump[i]+j,(kMaxTableEntries-j)-1);
	mTablesControls[i][j]= 0;
	mTablesValues[i][j]= 0;
	mTablesUsedBytes[i][j]= false;
	mTablesUsedByJump[i][j]= false;
	int k;
	// Renumber jumps
	for (k=0;k<kMaxTableEntries;k++)
	{
		if (mTablesControls[i][k] == 0xff)
		{
			if (mTablesValues[i][k] > j)
			{
				mTablesValues[i][k]++;
			}
		}
	}
	// Renumber envelope offsets
	for (k=0;k<kMaxEnvelopes;k++)
	{
		switch(i)
		{
			default:
			{
				assert(!"Should not ever trigger");
				break;
			}
			case 0:
			{
				if (mEnvelopes[k].mTableWave > j+1)
				{
					mEnvelopes[k].mTableWave++;
				}
				break;
			}
			case 1:
			{
				if (mEnvelopes[k].mTableNote > j+1)
				{
					mEnvelopes[k].mTableNote++;
				}
				break;
			}
			case 2:
			{
				if (mEnvelopes[k].mTablePulse > j+1)
				{
					mEnvelopes[k].mTablePulse++;
				}
				break;
			}
			case 3:
			{
				if (mEnvelopes[k].mTableFilter > j+1)
				{
					mEnvelopes[k].mTableFilter++;
				}
				break;
			}
		}
	}
}
