
// MusicStudioDoc.h : interface of the CMusicStudioDoc class
//


#pragma once
#include "../MusicStudioConvert/MusicFile.h"
#include "../RESID-FP/trunk/sidplay-2.0.9/src/SimpleInterface.h"

class SIDWriteEvent
{
public:
	explicit SIDWriteEvent(const double clock,const unsigned char reg,const unsigned char data) :
		mClock(clock) , mReg(reg) , mData(data)
	{
	}
	virtual ~SIDWriteEvent()
	{
	}
	const double mClock;
	const unsigned char mReg;
	const unsigned char mData;
};

class CMusicStudioDoc : public CDocument , public MemoryHook
{
public:

	static const int kMIDIHeader = 0x6468544d;
	static const int kGoatTrackerV1 = 0x21535447;
	static const int kGoatTrackerV2 = 0x35535447;

protected: // create from serialization only
	CMusicStudioDoc();
	DECLARE_DYNCREATE(CMusicStudioDoc)

// Attributes
public:

	MusicStudio1::MusicFile mMusicFile;

	bool mUsing6581;

	CString mTitleInfo;
	CString mAuthorInfo;
	CString mReleasedInfo;
	CString mGenericInfo;

	// The loaded data for this document
	unsigned char mTracks[MusicStudio1::MusicFile::kMaxTracks][MusicStudio1::MusicFile::kMaxTrackLength];

	CString mBlocks[MusicStudio1::MusicFile::kMaxBlocks];
	std::list<int> mBlockErrorReport[MusicStudio1::MusicFile::kMaxBlocks];
	static const int kMaxInternalTrackerRows = 1024;
	CString mBlockTrackerRows[MusicStudio1::MusicFile::kMaxBlocks][kMaxInternalTrackerRows][3];
	bool mBlockLastEditedAsTracker[MusicStudio1::MusicFile::kMaxBlocks];
	unsigned char mBlockTrackerRowsCachedAsTempo[MusicStudio1::MusicFile::kMaxBlocks];
	bool mBlockTrackerErrorRows[MusicStudio1::MusicFile::kMaxBlocks][kMaxInternalTrackerRows];
	unsigned char mBlockTrackerTempos[MusicStudio1::MusicFile::kMaxBlocks];
	int mBlockTrackerLengths[MusicStudio1::MusicFile::kMaxBlocks];
	int mBlockEditTrackerMaxCalcedRow[MusicStudio1::MusicFile::kMaxBlocks];


	MusicStudio1::Envelope mEnvelopes[MusicStudio1::MusicFile::kMaxEnvelopes];
	CString mEnvelopeNames[MusicStudio1::MusicFile::kMaxEnvelopes];
	MusicStudio1::DrumData mDrums[MusicStudio1::MusicFile::kOldMaxDrums];
	CString mDrumNames[MusicStudio1::MusicFile::kOldMaxDrums];

	MusicStudio1::ExtendedArpeggio mExtendedArpeggios[MusicStudio1::MusicFile::kMaxExtendedArpeggios];
	CString mExtendedArpeggiosNames[MusicStudio1::MusicFile::kMaxExtendedArpeggios];

	// Remember that position 0 is not displayed for editing and is not used by the player code
	// Position 0 actually means "stop the generator".
	unsigned char mTablesControls[MusicStudio1::MusicFile::kMaxTables][MusicStudio1::MusicFile::kMaxTableEntries+1];
	unsigned char mTablesValues[MusicStudio1::MusicFile::kMaxTables][MusicStudio1::MusicFile::kMaxTableEntries+1];

	unsigned char mZeroPageStart;

	unsigned char mHardRestartAttackDecay[MusicStudio1::MusicFile::kMaxTracks];
	unsigned char mHardRestartSustainRelease[MusicStudio1::MusicFile::kMaxTracks];
	unsigned char mHardRestartWaveform[MusicStudio1::MusicFile::kMaxTracks];
	unsigned char mHardRestartFrames[MusicStudio1::MusicFile::kMaxTracks];

	unsigned char mTempo[MusicStudio1::MusicFile::kMaxTracks];	// Currently not used.

	bool mTrackerBlockEditState;

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

	void LoadVersion0(CArchive& ar);
	void LoadVersion1(CArchive& ar);
	void LoadVersion2(CArchive& ar);
	void LoadVersion3(CArchive& ar);
	void LoadVersion4(CArchive& ar);
	void LoadVersion5(CArchive& ar);
	void LoadVersion6(CArchive& ar);
	void LoadVersion7(CArchive& ar);

// Implementation
public:
	virtual ~CMusicStudioDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void UpdateDocumentDataToInternalFile(void);

	void UpgradeOldV4InstrumentData(void);

	void OptimiseTables(const bool assumeAllEnvelopesUsed = false);

	int getNextFreeTableControl(const int tableIndex);

protected:
	CMultiDocTemplate* mDocTemplateExtended;

	std::list<SIDWriteEvent> mSIDEvents;

	virtual void SIDRegisterWrite(const double clock,const unsigned char reg,const unsigned char data);

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	CDocTemplate *GetTemplateExtended(void);
	bool LoadGoatTrackerSong(CArchive &ar,const int version);

	bool LoadMIDIFile(CArchive &ar);

	bool LoadMWMusicFile(CArchive &ar);

	void RemoveTableByteAt(const int table, const int position);

	void AddTableByteAt(const int table, const int position);

	void ImportEnvelopesAndTablesFromFile(void);

	void UpdateTrackerBlockToInternal(const int currentBlock);

	void ClearCapturedSIDData(void);

	void ProcessSIDCaptureData(void);

	int GetInstrument(const unsigned char *SIDBytes);
};
