#ifndef _MUSICFILE_H_
#define _MUSICFILE_H_

#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>

// Constants and data format from Music Studio 1 (MusicEditor/MusicPlayer.a)
namespace MusicStudio1
{

// The order and values of these are important because bcs tests are used when comparing with
// kMusicPlayer_StopTrack
// Track control defines
const unsigned char kMusicPlayer_StopTrack		= 0xfd;
const unsigned char kMusicPlayer_StopAllTracks	= 0xfe;
const unsigned char kMusicPlayer_LoopTrack		= 0xff;
const unsigned char kMusicPlayer_RepeatBlock	= 0x40;
const unsigned char kMusicPlayer_TransposeBlock	= 0x80;
const unsigned char kMusicPlayer_TransposeBlockNeg	= 0xb0;

// Block control defines
const unsigned char kMusicPlayer_Arpeggio		= 0xf4;	// New command in Music Studio 2
const unsigned char kMusicPlayer_FilterGlide	= 0xf5;
const unsigned char kMusicPlayer_FilterHi		= 0xf6;
const unsigned char kMusicPlayer_FilterLo		= 0xf7;
const unsigned char kMusicPlayer_FilterControl	= 0xf8;
const unsigned char kMusicPlayer_FilterPass		= 0xf9;
const unsigned char kMusicPlayer_Duration		= 0xfa;
const unsigned char kMusicPlayer_Envelope		= 0xfb;
const unsigned char kMusicPlayer_Volume			= 0xfc;
const unsigned char kMusicPlayer_EndBlock		= 0xfd;
const unsigned char kMusicPlayer_Glide			= 0xfe;
// $ff seems to be unused and checked for in the block editor as a blank line
const unsigned char kMusicPlayer_BlankCommand	= 0xff;

const unsigned char kMusicFileVersion = 1;

class MusicFile;

class BlockEntry
{
public:
	BlockEntry();
	virtual ~BlockEntry();

	virtual void UpdateReferences(void)
	{
	}

	virtual void UpdateTableReferences(void)
	{
	}

	virtual void GetFormatted(char *line) = 0;

	virtual std::string GetCommandName(void) = 0;

	virtual int GetByteSize(void) = 0;

	virtual int WriteASM(FILE *fp) = 0;

	void SetBlockEntry(MusicFile *music,const int blockNum,const int theLine = -1, const int blockTime = -1)
	{
		mMusic = music;
		mBlockNum = blockNum;
		mTheLine = theLine;
		mBlockTime = blockTime;
	}

	int GetTheLine(void)
	{
		return mTheLine;
	}
	
	int GetBlockTime(void)
	{
		return mBlockTime;
	}

	virtual bool IsNote(void)
	{
		return false;
	}

	virtual bool Transpose(const int transpose)
	{
		return false;
	}

	virtual unsigned char GetDataByte0(void)
	{
		return 0;
	}

	virtual unsigned char GetDataByte1(void)
	{
		return 0;
	}

	static std::string GetNoteFromNumber(const unsigned char number);

protected:
	MusicFile *mMusic;
	int mBlockNum;
	int mTheLine;
	int mBlockTime;
};

class Envelope
{
public:
	Envelope();
	virtual ~Envelope();

	// Legacy entries from the original file format. Loaded if converting older files.
	unsigned char mAttackDecay;
	unsigned char mSustainRelease;
	unsigned char mWaveControl;
	unsigned char mPulseWidthVariance;
	unsigned char mVibratoUp;
	unsigned char mVibratoDown;
	unsigned char mDrumVibratoControl;
	unsigned char mWaveMix;

	// New entries
	bool mIsUsed;
	int mRealIndex;
	unsigned char mTranspose;

	// The new tables, these can be active or not. If they are active then the value for the
	// table is used.
	unsigned char mTableWave;
	unsigned char mTableNote;
	unsigned char mTablePulse;
	unsigned char mTableFilter;
	bool mActiveTableWave;
	bool mActiveTableNote;
	bool mActiveTablePulse;
	bool mActiveTableFilter;
	unsigned char mTableBitMask;	// Only calculated for used envelopes during the C64 export

	unsigned char mMiscBitMask;		// Only calculated for used envelopes during the C64 export


	unsigned char mHardRestartTime;
	unsigned char mHardRestartAttackDecay;
	unsigned char mHardRestartSustainRelease;
	unsigned char mHardRestartWave;

	bool mAllowVoiceEffects;
};

class DrumData
{
public:
	DrumData();
	virtual ~DrumData();

	unsigned char mHiNotes[8];
	unsigned char mWaveControl[8];

	unsigned char mStartLoop;
	unsigned char mEndLoop;
	unsigned char mHighestStartPos;
	unsigned char mGateLength;
	
	bool mUsed;
};

class ExtendedArpeggio
{
public:
	ExtendedArpeggio();
	virtual ~ExtendedArpeggio();

	static const int kMaxExtendedArpeggioData = 8;

	bool mUsed;
	unsigned char mArpeggios[kMaxExtendedArpeggioData];
	int mLength;
};

// Very simple representation of the parsed music data
class MusicFile
{
public:
	MusicFile();
	virtual ~MusicFile();

	static const int kMaxBlocks = 64;
	static const int kMaxTracks = 3;
	static const int kMaxTrackLength = 256;
	static const int kMaxEnvelopes = 256;
	static const int kMaxExtendedArpeggios = 32;
	static const int kMaxTableEntries = 255;
	static const int kMaxTables = 4;

	// Constants for the old MusicStudio 1 files
	static const int kOldMaxEnvelopes = 32;
	// Constants from older versions of MusicStudio in this new editor
	static const int kOldMaxDrums = 4;

	bool LoadFromFile(const char *inputFilename);

	void Optimise(void);

	BlockEntry *ParseText(const char *line);

	void AddBlockEntryCommon(const int block, BlockEntry *entry, const int theLine = -1, const int blockTime = -1);
	std::list<BlockEntry*>::iterator AddBlockEntry(const int block, BlockEntry *entry, const int theLine = -1, const int blockTime = -1);

	void ClearBlock(const int block);

	std::string GetBlockText(const int block);

	void SetBlockFromText(const int block, const char *text,std::list<int> &errorReport,int &resultantSize,int &totalDuration);

	bool OptimiseAndWrite(	const char *acmeCommandLine,
							const char *acmeOptions,
							const char *addr,
							int &codeSize,
							int &trackSize,
							int &blockSize,
							int &envelopeSize,
							int &tableSize,
							const int startSong = 0,
							const bool includeSoundEffectCode = false,
							const char *titleInfo = "",
							const char *authorInfo = "",
							const char *releasedInfo = "20??",
							const bool SIDIs6581 = true,
							const bool runningInEditor = false
							);

	int MapTrackPosToOptimisedPos(const int track,const int pos);

	struct ThisLess
	{
		bool operator()(const std::string &s1, const std::string &s2) const
		{
			return strcmp(s1.c_str(), s2.c_str()) < 0;
		}
	};

	// Always three tracks of 256 entries and 64 blocks of various commands and sizes
	unsigned char mTracks[kMaxTracks][kMaxTrackLength];
	unsigned char mTrackReverseIndex[kMaxTracks][kMaxTrackLength];
	int mHighestUsedPosition[kMaxTracks];
	std::list<BlockEntry*> mBlocks[kMaxBlocks];
	// A map of byte offset to line number in the text block
	std::map<int,int> mBlockByteToLine[kMaxBlocks];
	unsigned char mCurrentBlockTranspose;
	bool mTrackUsedBlock[kMaxBlocks];
	int mRealBlockIndex[kMaxBlocks];
	std::set<std::string,ThisLess> mAutoUsedFeatures;

	unsigned char mTablesControls[kMaxTables][kMaxTableEntries];
	unsigned char mTablesValues[kMaxTables][kMaxTableEntries];
	bool mTablesUsedBytes[kMaxTables][kMaxTableEntries];
	bool mTablesUsedByJump[kMaxTables][kMaxTableEntries];

	DrumData mDrums[kOldMaxDrums];

	Envelope mEnvelopes[kMaxEnvelopes];
	unsigned char mArpeggios[kMaxEnvelopes];	// Loaded but not saved in this position. Instead wave mix is saved. This is used to create arpeggio commands during the packed file load.

	bool mSluringCommandsActive;
	bool mHardRestartCommandsActive;

	int mLastDurationParsed;
	int mLastDurationSetInternally;
	int mInternalSlurCount;
	std::list<BlockEntry*>::iterator mLastNoteAdded;

	// Addresses inside the C64's memory indexed by the label
	std::map<std::string,int,ThisLess> mC64Labels;
	int GetAddressFromLabel(const char *label);
	// Some frequently used cached values from mC64Labels
	int mC64Tracks[kMaxTracks];
	int mC64trcn;
	int mC64blcn;

	// Maximum of one C64 memory page for this data
	unsigned char mExtendedArpeggioData[256];
	int mMaxExtendedArpeggioData;
	unsigned char mExtendedArpeggiosReverse[kMaxExtendedArpeggios];
	ExtendedArpeggio mExtendedArpeggios[MusicStudio1::MusicFile::kMaxExtendedArpeggios];
	bool mExtendedArpeggioUsed;
	bool mArpeggioUsed;
	bool mArpeggioStopUsed;
	bool mSmallVibratoUsed;
	bool mLargeVibratoUsed;
	bool mPulseWidthVarianceUsed;
	bool mDrumDynamicStartOffsetUsed;
	bool mDrumGateLengthUsed;

	/// Returns true if something was changed
	bool OptimiseTables(const bool assumeAllEnvelopesUsed = false);

	void RemoveTableByteAt(const int table, const int position);
	void AddTableByteAt(const int table, const int position);

	unsigned char mZeroPageStart;

	unsigned char mHardRestartAttackDecay[MusicStudio1::MusicFile::kMaxTracks];
	unsigned char mHardRestartSustainRelease[MusicStudio1::MusicFile::kMaxTracks];
	unsigned char mHardRestartWaveform[MusicStudio1::MusicFile::kMaxTracks];
	unsigned char mHardRestartFrames[MusicStudio1::MusicFile::kMaxTracks];

	unsigned char mTempo[MusicStudio1::MusicFile::kMaxTracks];

	bool mVariableHardRestartParams;

	void OptimiseTracks(void);
	void MarkExecutedTableBytes(unsigned char* tableControl,unsigned char* tableValue,bool* tableUsed,bool* tableUsedJump,int start);
	int TableMaxPos(const int table);
	int WriteTable(FILE *fp,const int table,const char *name);

private:
};

}; //< namespace MusicStudio1

#endif
