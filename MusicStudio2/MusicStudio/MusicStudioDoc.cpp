
// MusicStudioDoc.cpp : implementation of the CMusicStudioDoc class
//

#include "stdafx.h"
#include <math.h>
#include <assert.h>
#include "MusicStudio.h"

#include "MusicStudioDoc.h"
#include "MusicStudioView2.h"
#include "ChildFrm.h"
#include "RNPlatform/Inc/StringUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMusicStudioDoc

IMPLEMENT_DYNCREATE(CMusicStudioDoc, CDocument)

BEGIN_MESSAGE_MAP(CMusicStudioDoc, CDocument)
END_MESSAGE_MAP()


// CMusicStudioDoc construction/destruction

using namespace MusicStudio1;

CMusicStudioDoc::CMusicStudioDoc() : mUsing6581(false) , mTrackerBlockEditState(false)
{
	mDocTemplateExtended = new CMultiDocTemplate(IDR_MusicStudioTYPE,
		RUNTIME_CLASS(CMusicStudioDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CMusicStudioView2));
		
	ZeroMemory(mTracks,sizeof(mTracks));
	ZeroMemory(mTablesControls,sizeof(mTablesControls));
	ZeroMemory(mTablesValues,sizeof(mTablesValues));
	ZeroMemory(mBlockLastEditedAsTracker,sizeof(mBlockLastEditedAsTracker));
	ZeroMemory(mBlockTrackerRowsCachedAsTempo,sizeof(mBlockTrackerRowsCachedAsTempo));
	ZeroMemory(mBlockTrackerErrorRows,sizeof(mBlockTrackerErrorRows));

	mZeroPageStart = 0xfb;

	int i;

	for (i=0;i<MusicStudio1::MusicFile::kMaxBlocks;i++)
	{
		mBlockTrackerTempos[i] = 2;
		mBlockTrackerLengths[i] = 64;
	}

	for (i=0;i<MusicStudio1::MusicFile::kMaxTracks;i++)
	{
		mHardRestartAttackDecay[i] = 0;
		mHardRestartSustainRelease[i] = 0xf0;
		mHardRestartWaveform[i] = 0x09;
		mHardRestartFrames[i] = 1;

		mTempo[i] = 2;
	}

	for (i=0;i<MusicStudio1::MusicFile::kMaxTracks;i++)
	{
		mTracks[i][0] = MusicStudio1::kMusicPlayer_StopTrack;
	}
}

CMusicStudioDoc::~CMusicStudioDoc()
{
}

BOOL CMusicStudioDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// Initialise some blank data like the old music editor did
#ifdef _DEBUG
//#if 0
	mTracks[0][0] = 0;
	mTracks[0][1] = MusicStudio1::kMusicPlayer_LoopTrack;
	mTracks[1][0] = MusicStudio1::kMusicPlayer_StopTrack;
	mTracks[2][0] = MusicStudio1::kMusicPlayer_StopTrack;

	mBlocks[0] = _T("ENV:01\r\nDUR:10\r\nC-4\r\nENV:02\r\nC-4\r\nENV:03\r\nC-4\r\nENV:04\r\nC-4\r\nENV:05\r\nC-4\r\n");
//	mBlocks[0] = _T("ENV:01\r\nDUR:10\r\nC-4\r\n");

	CStringA ansiBlock(mBlocks[0]);
	int byteSize = 0;
	int totalDuration = 0;
	mMusicFile.SetBlockFromText(0,ansiBlock,mBlockErrorReport[0],byteSize,totalDuration);
	mBlockTrackerLengths[0] = totalDuration;

	int i;
	for (i=0;i<=5;i++)
	{
		mEnvelopes[i].mActiveTableWave = true;
		mEnvelopes[i].mActiveTableNote = true;
		mEnvelopes[i].mActiveTablePulse = true;
		mEnvelopes[i].mAllowVoiceEffects = false;
	}

	mEnvelopeNames[0] = _T("Blank");

	mEnvelopeNames[1] = _T("Pulse note");
	mEnvelopes[1].mAttackDecay = 0x22;
	mEnvelopes[1].mSustainRelease = 0xfd;
	mEnvelopes[1].mAllowVoiceEffects = true;
	mEnvelopes[1].mTableWave = 0x01;
	mEnvelopes[1].mTableNote = 0x00;
	mEnvelopes[1].mTablePulse = 0x01;

	mEnvelopeNames[2] = _T("Snare drum");
	mEnvelopes[2].mAttackDecay = 0x00;
	mEnvelopes[2].mSustainRelease = 0xfa;
	mEnvelopes[2].mTableWave = 0x03;
	mEnvelopes[2].mTableNote = 0x01;
	mEnvelopes[2].mTablePulse = 0x03;

	mEnvelopeNames[3] = _T("Bass drum");
	mEnvelopes[3].mAttackDecay = 0x00;
	mEnvelopes[3].mSustainRelease = 0xf7;
	mEnvelopes[3].mTableWave = 0x09;
	mEnvelopes[3].mTableNote = 0x08;
	mEnvelopes[3].mTablePulse = 0x05;

	mEnvelopeNames[4] = _T("Tom tom1");
	mEnvelopes[4].mAttackDecay = 0x00;
	mEnvelopes[4].mSustainRelease = 0xf8;
	mEnvelopes[4].mTableWave = 0x09;
	mEnvelopes[4].mTableNote = 0x0f;
	mEnvelopes[4].mTablePulse = 0x07;

	mEnvelopeNames[5] = _T("Tom tom2");
	mEnvelopes[5].mAttackDecay = 0x00;
	mEnvelopes[5].mSustainRelease = 0xfc;
	mEnvelopes[5].mTableWave = 0x0c;
	mEnvelopes[5].mTableNote = 0x12;
	mEnvelopes[5].mTablePulse = 0x07;

	unsigned char tableControlWave[]	= {0x41,0xff,0x81,0x40,0x40,0x80,0x40,0xff,0x81,0x40,0xff,0x81,0x40,0x80,0xff};
	unsigned char tableValueWave[]		= {0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x06,0x00,0x00,0x00,0x00,0x04,0x00,0x0d};
	unsigned char tableControlNote[]	= {0xb0,0xb0,0xb0,0xb0,0xb0,0xb0,0xff,0xb0,0xb0,0xb0,0xb0,0xb0,0xbf,0xff,0xb0,0xaf,0xff,0xb0,0xa4,0xb0,0xff};
	unsigned char tableValueNote[]		= {0x2b,0x08,0x0c,0x8f,0x82,0x0d,0x04,0x28,0x08,0x07,0x05,0x03,0x01,0x00,0x08,0xfe,0x10,0x08,0xfe,0x09,0x13};
	unsigned char tableControlPulse[]	= {0x0b,0xff,0x09,0xff,0x04,0xff,0x07,0xff};
	unsigned char tableValuePulse[]		= {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	memcpy(mTablesControls[0]+1,tableControlWave,sizeof(tableControlWave));
	memcpy(mTablesValues[0]+1,tableValueWave,sizeof(tableValueWave));
	memcpy(mTablesControls[1]+1,tableControlNote,sizeof(tableControlNote));
	memcpy(mTablesValues[1]+1,tableValueNote,sizeof(tableValueNote));
	memcpy(mTablesControls[2]+1,tableControlPulse,sizeof(tableControlPulse));
	memcpy(mTablesValues[2]+1,tableValuePulse,sizeof(tableValuePulse));


	mExtendedArpeggiosNames[0] = _T("Major third");
	mExtendedArpeggios[0].mArpeggios[0] = 4;
	mExtendedArpeggios[0].mArpeggios[1] = 3;
	mExtendedArpeggios[0].mArpeggios[2] = 256 - 4 - 3;
	mExtendedArpeggios[0].mLength = 3;

	mExtendedArpeggiosNames[1] = _T("Minor third");
	mExtendedArpeggios[1].mArpeggios[0] = 3;
	mExtendedArpeggios[1].mArpeggios[1] = 4;
	mExtendedArpeggios[1].mArpeggios[2] = 256 - 3 - 4;
	mExtendedArpeggios[1].mLength = 3;

	mExtendedArpeggiosNames[2] = _T("Minor seventh");
	mExtendedArpeggios[2].mArpeggios[0] = 3;
	mExtendedArpeggios[2].mArpeggios[1] = 3;
	mExtendedArpeggios[2].mArpeggios[2] = 4;
	mExtendedArpeggios[2].mArpeggios[3] = 256 - 3 - 3 - 4;
	mExtendedArpeggios[2].mLength = 4;
#endif // <ifdef _DEBUG

	UpdateDocumentDataToInternalFile();
	return TRUE;
}




// CMusicStudioDoc serialization

void CMusicStudioDoc::Serialize(CArchive& ar)
{
	// A constant value at the start of every file
	const int kMagicHeader = 0x7623abfe;
	// Version 0 - 4	= 2.0
	// Version 5 -		= 2.1
	// A version of the file that we save
	const int kCurrentVersion = 7;
	if (ar.IsStoring())
	{
		ar << kMagicHeader;
		ar << kCurrentVersion;
		
		ar << mUsing6581;

		int i,j;
		for (i=0;i<MusicStudio1::MusicFile::kMaxTracks;i++)
		{
			for (j=0;j<MusicStudio1::MusicFile::kMaxTrackLength;j++)
			{
				ar << mTracks[i][j];
			}
		}

		for (i=0;i<MusicStudio1::MusicFile::kMaxBlocks;i++)
		{		
			ar << mBlockTrackerTempos[i];
			ar << mBlockTrackerLengths[i];
			ar << mBlockEditTrackerMaxCalcedRow[i];
			ar << mBlockLastEditedAsTracker[i];
			ar << mBlocks[i];
			if (mBlockLastEditedAsTracker[i])
			{
				for (j=0;j<kMaxInternalTrackerRows;j++)
				{
					ar << mBlockTrackerRows[i][j][0];
					ar << mBlockTrackerRows[i][j][1];
					ar << mBlockTrackerRows[i][j][2];
				}
			}
		}

		for (i=0;i<MusicStudio1::MusicFile::kMaxEnvelopes;i++)
		{		
			ar << mEnvelopeNames[i];
			ar << mEnvelopes[i].mAttackDecay;
			ar << mEnvelopes[i].mSustainRelease;
			ar << mEnvelopes[i].mWaveControl;
			ar << mEnvelopes[i].mPulseWidthVariance;
			ar << mEnvelopes[i].mVibratoUp;
			ar << mEnvelopes[i].mVibratoDown;
			ar << mEnvelopes[i].mDrumVibratoControl;
			ar << mEnvelopes[i].mWaveMix;
			// Save this so later on it can be used for flagging sound effects not used in the music
			ar << mEnvelopes[i].mIsUsed;
			ar << mEnvelopes[i].mTranspose;

			ar << mEnvelopes[i].mTableWave;
			ar << mEnvelopes[i].mTableNote;
			ar << mEnvelopes[i].mTablePulse;
			ar << mEnvelopes[i].mTableFilter;
			ar << mEnvelopes[i].mActiveTableWave;
			ar << mEnvelopes[i].mActiveTableNote;
			ar << mEnvelopes[i].mActiveTablePulse;
			ar << mEnvelopes[i].mActiveTableFilter;

			ar << mEnvelopes[i].mHardRestartTime;
			ar << mEnvelopes[i].mHardRestartAttackDecay;
			ar << mEnvelopes[i].mHardRestartSustainRelease;
			ar << mEnvelopes[i].mHardRestartWave;

			ar << mEnvelopes[i].mAllowVoiceEffects;
		}

		for (i=0;i<MusicStudio1::MusicFile::kOldMaxDrums;i++)
		{		
			ar << mDrumNames[i];
			for (j=0;j<8;j++)
			{
				ar << mDrums[i].mHiNotes[j];
				ar << mDrums[i].mWaveControl[j];
			}
			ar << mDrums[i].mStartLoop;
			ar << mDrums[i].mEndLoop;
			ar << mDrums[i].mGateLength;
		}

		for (i=0;i<MusicStudio1::MusicFile::kMaxExtendedArpeggios;i++)
		{
			ar << mExtendedArpeggiosNames[i];
			ar << mExtendedArpeggios[i].mLength;
			for (j=0;j<MusicStudio1::ExtendedArpeggio::kMaxExtendedArpeggioData;j++)
			{
				ar << mExtendedArpeggios[i].mArpeggios[j];
			}
		}

		for (i=0;i<MusicStudio1::MusicFile::kMaxTables;i++)
		{
			for (j=0;j<MusicStudio1::MusicFile::kMaxTableEntries;j++)
			{
				ar << mTablesControls[i][j+1];
				ar << mTablesValues[i][j+1];
			}
		}

		ar << mGenericInfo;

		ar << mTitleInfo;
		ar << mAuthorInfo;
		ar << mReleasedInfo;



		ar << mZeroPageStart;

		for (i=0;i<MusicStudio1::MusicFile::kMaxTracks;i++)
		{
			ar << mHardRestartAttackDecay[i];
			ar << mHardRestartSustainRelease[i];
			ar << mHardRestartWaveform[i];
			ar << mHardRestartFrames[i];

			ar << mTempo[i];
		}

		ar << mTrackerBlockEditState;
	}
	else
	{
		int magicTest , version = -1;
		ar >> magicTest;

		if (magicTest != kMagicHeader)
		{
			if (magicTest == kMIDIHeader)
			{
				LoadMIDIFile(ar);
				UpdateDocumentDataToInternalFile();
				return;
			}

			// Detect goat tracker header "GTS!" or "GTS5" and parse it
			if ((magicTest == kGoatTrackerV1) || (magicTest == kGoatTrackerV2))
			{
				LoadGoatTrackerSong(ar,magicTest);
				UpdateDocumentDataToInternalFile();
				return;
			}
			// It isn't a regular file, try to load the C64 format data instead.
			CStringA ansiName(ar.GetFile()->GetFilePath());
			mMusicFile.LoadFromFile(ansiName);

			int i,j;
			for (i=0;i<MusicStudio1::MusicFile::kMaxBlocks;i++)
			{
				mBlocks[i] = CString(mMusicFile.GetBlockText(i).c_str());
			}

			for (j=0;j<MusicStudio1::MusicFile::kMaxTracks;j++)
			{
				for (i=0;i<MusicStudio1::MusicFile::kMaxTrackLength;i++)
				{
					mTracks[j][i] = mMusicFile.mTracks[j][i];
				}
			}

			for (i=0;i<MusicStudio1::MusicFile::kOldMaxEnvelopes;i++)
			{
				mEnvelopes[i] = mMusicFile.mEnvelopes[i];
			}

			for (i=0;i<MusicStudio1::MusicFile::kOldMaxDrums;i++)
			{
				mDrums[i] = mMusicFile.mDrums[i];
			}
		}
		else
		{
			ar >> version;

			// Parse the various released file versions
			if (version == 0)
			{
				LoadVersion0(ar);
			}
			else if (version == 1)
			{
				LoadVersion1(ar);
			}
			else if (version == 2)
			{
				LoadVersion2(ar);
			}
			else if (version == 3)
			{
				LoadVersion3(ar);
			}
			else if (version == 4)
			{
				LoadVersion4(ar);
			}
			else if (version == 5)
			{
				LoadVersion5(ar);
			}
			else if (version == 6)
			{
				LoadVersion6(ar);
			}
			else if (version == 7)
			{
				LoadVersion7(ar);
			}
			else
			{
				MessageBox(0,_T("The loaded version is unknown"),_T("Error"),MB_ICONERROR | MB_OK);
				return;
			}
		}

		// Old versions have the negative transpose one lower so they need adjusting.
		if (version <= 3)
		{
			int i,j;
			for (j=0;j<MusicStudio1::MusicFile::kMaxTracks;j++)
			{
				for (i=0;i<MusicStudio1::MusicFile::kMaxTrackLength;i++)
				{
					if ( (mTracks[j][i] >= MusicStudio1::kMusicPlayer_TransposeBlockNeg) && (mTracks[j][i] < MusicStudio1::kMusicPlayer_StopTrack))
					{
						mTracks[j][i] = mTracks[j][i] + 1;
					}
				}
			}
		}

		if (version <= 4)
		{
			UpgradeOldV4InstrumentData();
		}

		if (version <= 6)
		{
			// Find out the block lengths from older files and calculate the smallest duration to use
			int i;
			for (i=0;i<MusicStudio1::MusicFile::kMaxBlocks;i++)
			{
				CStringA ansiBlock(mBlocks[i]);
				int byteSize = 0;
				int totalDuration = 0;
				mMusicFile.SetBlockFromText(i,ansiBlock,mBlockErrorReport[i],byteSize,totalDuration);
				
				if ((byteSize > 0) && (totalDuration > 0))
				{
					mBlockTrackerLengths[i] = totalDuration;

					int smallestDuration = 16;
					std::list<MusicStudio1::BlockEntry*>::iterator st = mMusicFile.mBlocks[i].begin();
					while (st != mMusicFile.mBlocks[i].end())
					{
						MusicStudio1::BlockEntry *block = *st++;

						int lastDurationReleaseTick = 0;
						if (block->GetCommandName() == "Duration")
						{
							lastDurationReleaseTick = block->GetDataByte0();
						}
						else if (block->GetCommandName() == "DurationTicks")
						{
							lastDurationReleaseTick = block->GetDataByte0() - block->GetDataByte1();
						}
						if (lastDurationReleaseTick && (lastDurationReleaseTick < smallestDuration))
						{
							smallestDuration = lastDurationReleaseTick;
						}
					}

					mBlockTrackerTempos[i] = smallestDuration;
				}
			}
		}

		// Lastly, make sure the internal data is initialised
		UpdateDocumentDataToInternalFile();
	}
}

// CMusicStudioDoc diagnostics

#ifdef _DEBUG
void CMusicStudioDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMusicStudioDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMusicStudioDoc commands

void CMusicStudioDoc::UpdateDocumentDataToInternalFile(void)
{
	ZeroMemory(mBlockTrackerErrorRows,sizeof(mBlockTrackerErrorRows));
	// Update the data in the music file with the data from our document
	int i,j;
	for (i=0;i<MusicStudio1::MusicFile::kMaxEnvelopes;i++)
	{
		mMusicFile.mEnvelopes[i] = mEnvelopes[i];
	}

	for (i=0;i<MusicStudio1::MusicFile::kOldMaxDrums;i++)
	{
		mMusicFile.mDrums[i] = mDrums[i];
	}

	for (i=0;i<MusicStudio1::MusicFile::kMaxTracks;i++)
	{
		mMusicFile.mHardRestartAttackDecay[i] = mHardRestartAttackDecay[i];
		mMusicFile.mHardRestartSustainRelease[i] = mHardRestartSustainRelease[i];
		mMusicFile.mHardRestartWaveform[i] = mHardRestartWaveform[i];
		mMusicFile.mHardRestartFrames[i] = mHardRestartFrames[i];
		mMusicFile.mTempo[i] = mTempo[i];

		if (mMusicFile.mHardRestartFrames[i] < 1)
		{
			mMusicFile.mHardRestartFrames[i] = 1;
		}

		int tokPos = 0;
		for (j=0;j<MusicStudio1::MusicFile::kMaxTrackLength;j++)
		{
			mMusicFile.mTracks[i][j] = mTracks[i][j];
		}
	}
	
	for (i=0;i<MusicStudio1::MusicFile::kMaxBlocks;i++)
	{
		if (mBlockLastEditedAsTracker[i])
		{
			UpdateTrackerBlockToInternal(i);
		}
		else
		{
			CStringA ansiBlock(mBlocks[i]);
			int byteSize = 0;
			int totalDuration = 0;
			mMusicFile.SetBlockFromText(i,ansiBlock,mBlockErrorReport[i],byteSize,totalDuration);
		}
	}

	for (i=0;i<MusicStudio1::MusicFile::kMaxExtendedArpeggios;i++)
	{
		mMusicFile.mExtendedArpeggios[i] = mExtendedArpeggios[i];
	}
	mMusicFile.mMaxExtendedArpeggioData = 0;

	for (i=0;i<MusicStudio1::MusicFile::kMaxTables;i++)
	{
		for (j=0;j<MusicStudio1::MusicFile::kMaxTableEntries;j++)
		{
			// Remembering that the document version has an unused 0 entry
			mMusicFile.mTablesControls[i][j] = mTablesControls[i][j+1];
			mMusicFile.mTablesValues[i][j] = mTablesValues[i][j+1];
		}
	}

	mMusicFile.mZeroPageStart = mZeroPageStart;
}

CDocTemplate *CMusicStudioDoc::GetTemplateExtended(void)
{
	return mDocTemplateExtended;
}

void CMusicStudioDoc::UpgradeOldV4InstrumentData(void)
{
	int waveTablePos = 3;
	int noteTablePos = 1;
	int pulseTablePos = 1;

	memset(mTablesControls,0,sizeof(mTablesControls));
	memset(mTablesValues,0,sizeof(mTablesValues));

	mTablesControls[0][1] = 0x09;	// Default case
	if (mEnvelopes[0].mWaveControl == 0x08)
	{
		// Special case for some of the earlier Tusari pieces
		mTablesControls[0][1] = 0x08;
	}
	mTablesValues[0][1] = 0x00;
	mTablesControls[0][2] = 0xff;
	mTablesValues[0][2] = 0x00;

	int i;
	for (i=0;i<MusicStudio1::MusicFile::kOldMaxEnvelopes;i++)
	{
		int oldWaveTablePos = waveTablePos;
		int oldNoteTablePos = noteTablePos;
		int oldPulseTablePos = pulseTablePos;

		mEnvelopes[i].mTableWave = 0;
		mEnvelopes[i].mActiveTableWave = true;
		mEnvelopes[i].mTableNote = 0;
		mEnvelopes[i].mActiveTableNote = true;
		mEnvelopes[i].mTablePulse = 0;
		mEnvelopes[i].mActiveTablePulse = true;
		mEnvelopes[i].mAllowVoiceEffects = true;

		// Spot wave effects not defined
		if (!(mEnvelopes[i].mWaveControl & 0xf0) && (mEnvelopes[i].mDrumVibratoControl & 0xf0) != 0x80)
		{
			mEnvelopes[i].mTableWave = 1;
			mEnvelopes[i].mActiveTableWave = true;
			mEnvelopes[i].mTableNote = 0;
			mEnvelopes[i].mActiveTableNote = true;
			mEnvelopes[i].mTablePulse = 0;
			mEnvelopes[i].mActiveTablePulse = true;
			mEnvelopes[i].mAllowVoiceEffects = false;
			continue;
		}

		if ((mEnvelopes[i].mDrumVibratoControl & 0xf0) == 0x80)
		{
			mEnvelopes[i].mTableWave = waveTablePos;
			mEnvelopes[i].mTableNote = noteTablePos;
			mEnvelopes[i].mTablePulse = pulseTablePos;

			// Parse the drum data
			mEnvelopes[i].mAllowVoiceEffects = false;

			mTablesControls[2][pulseTablePos] = mEnvelopes[i].mPulseWidthVariance & 0x0f;
			mTablesValues[2][pulseTablePos] = 0;
			pulseTablePos++;

			mTablesControls[2][pulseTablePos] = 0xff;
			mTablesValues[2][pulseTablePos] = 0;
			pulseTablePos++;

			int theDrum = mEnvelopes[i].mDrumVibratoControl & 0x03;
			int offset = (mEnvelopes[i].mDrumVibratoControl >> 2) & 0x03;
			int theGateTime = mDrums[theDrum].mGateLength;
			if (mDrums[theDrum].mStartLoop >= (theGateTime + offset))
			{
				// Can loop within the data since the gate has been used by the time the loop starts
				int i;
				for (i=offset;i<mDrums[theDrum].mEndLoop;i++)
				{
					if (i == mDrums[theDrum].mStartLoop)
					{
						oldWaveTablePos = waveTablePos;
						oldNoteTablePos = noteTablePos;
					}

					mTablesControls[0][waveTablePos] = mDrums[theDrum].mWaveControl[i] & 0xfe;
					if (theGateTime >= 0)
					{
						mTablesControls[0][waveTablePos] |= 1;
					}
					waveTablePos++;
					theGateTime--;

					if ((theDrum < 2) || (mDrums[theDrum].mWaveControl[i] & 0x80))
					{
						mTablesControls[1][noteTablePos] = 0xb0;
						mTablesValues[1][noteTablePos] = mDrums[theDrum].mHiNotes[i];
					}
					else
					{
						mTablesControls[1][noteTablePos] = 0xa0;
						mTablesValues[1][noteTablePos] = mDrums[theDrum].mHiNotes[i];
					}
					noteTablePos++;
				}
			}
			else
			{
				// Need to unroll the drum sound somewhat
				int i;
				for (i=offset;i<mDrums[theDrum].mEndLoop;i++)
				{
					mTablesControls[0][waveTablePos] = mDrums[theDrum].mWaveControl[i] & 0xfe;
					if (theGateTime >= 0)
					{
						mTablesControls[0][waveTablePos] |= 1;
					}
					waveTablePos++;
					theGateTime--;

					if ((theDrum < 2) || (mDrums[theDrum].mWaveControl[i] & 0x80))
					{
						mTablesControls[1][noteTablePos] = 0xb0;
						mTablesValues[1][noteTablePos] = mDrums[theDrum].mHiNotes[i];
					}
					else
					{
						mTablesControls[1][noteTablePos] = 0xa0;
						mTablesValues[1][noteTablePos] = mDrums[theDrum].mHiNotes[i];
					}
					noteTablePos++;
				}

				oldWaveTablePos = waveTablePos;
				oldNoteTablePos = noteTablePos;

				for (i=mDrums[theDrum].mStartLoop;i<mDrums[theDrum].mEndLoop;i++)
				{
					mTablesControls[0][waveTablePos] = mDrums[theDrum].mWaveControl[i] & 0xfe;
					waveTablePos++;

					if ((theDrum < 2) || (mDrums[theDrum].mWaveControl[i] & 0x80))
					{
						mTablesControls[1][noteTablePos] = 0xb0;
						mTablesValues[1][noteTablePos] = mDrums[theDrum].mHiNotes[i];
					}
					else
					{
						mTablesControls[1][noteTablePos] = 0xa0;
						mTablesValues[1][noteTablePos] = mDrums[theDrum].mHiNotes[i];
					}
					noteTablePos++;
				}
			}

			mTablesControls[0][waveTablePos] = 0xff;
			mTablesValues[0][waveTablePos] = oldWaveTablePos;
			waveTablePos++;

			mTablesControls[1][noteTablePos] = 0xff;
			mTablesValues[1][noteTablePos] = oldNoteTablePos;
			noteTablePos++;

		}
		else
		{
			mEnvelopes[i].mTableWave = waveTablePos;
			// Parse the non-drum instrument data
			mTablesControls[0][waveTablePos] = mEnvelopes[i].mWaveControl;
			if (mTablesControls[0][waveTablePos] & 0x40)
			{
				mTablesControls[0][waveTablePos] &= 0xc7;	// Remove the pulse variance information with pulse waveforms
			}
			mTablesValues[0][waveTablePos] = 0;
			waveTablePos++;
			if (mEnvelopes[i].mWaveMix)
			{
				mTablesControls[0][waveTablePos] = (mEnvelopes[i].mWaveControl ^ mEnvelopes[i].mWaveMix) | 1;
				mTablesValues[0][waveTablePos] = 0;
				waveTablePos++;
			}
			mTablesControls[0][waveTablePos] = 0xff;
			if (waveTablePos - oldWaveTablePos > 1)
			{
				mTablesValues[0][waveTablePos] = oldWaveTablePos;
			}
			else
			{
				mTablesValues[0][waveTablePos] = 0;
			}
			waveTablePos++;
			// We have pulse bits set so add these to the table
			if ((mEnvelopes[i].mWaveControl & 0x40) || (mEnvelopes[i].mWaveMix & 0x40))
			{
				mEnvelopes[i].mTablePulse = pulseTablePos;
				mTablesControls[2][pulseTablePos] = mEnvelopes[i].mPulseWidthVariance & 0x0f;
				mTablesValues[2][pulseTablePos] = 0;
				pulseTablePos++;

				if (mEnvelopes[i].mPulseWidthVariance & 0x30)
				{
					// Got some pulse width variance
					oldPulseTablePos = pulseTablePos;

					// Convert the speed into time and use the step size
					if (((mEnvelopes[i].mWaveControl >> 3) & 0x7)==0)
					{
						// This recreates a bug in the original player with "0 0 0	slow pulse change"
						mTablesControls[2][pulseTablePos] = 0x17;
						mTablesValues[2][pulseTablePos] = ((mEnvelopes[i].mPulseWidthVariance & 0xc0)+1) >> 2;
						pulseTablePos++;
					}
					else
					{
						if (mEnvelopes[i].mPulseWidthVariance & 0x10)
						{
							mTablesControls[2][pulseTablePos] = 0x10 | (10-((mEnvelopes[i].mWaveControl >> 3) & 0x7));
							mTablesValues[2][pulseTablePos] = ((mEnvelopes[i].mPulseWidthVariance & 0xc0)+1) >> 2;
							pulseTablePos++;
						}

						if (mEnvelopes[i].mPulseWidthVariance & 0x20)
						{
							mTablesControls[2][pulseTablePos] = 0x20 | (10-((mEnvelopes[i].mWaveControl >> 3) & 0x7));
							mTablesValues[2][pulseTablePos] = ((mEnvelopes[i].mPulseWidthVariance & 0xc0)+1) >> 2;
							pulseTablePos++;
						}
					}

					mTablesControls[2][pulseTablePos] = 0xff;
					mTablesValues[2][pulseTablePos] = oldPulseTablePos;
					pulseTablePos++;
				}
				else
				{
					mTablesControls[2][pulseTablePos] = 0xff;
					mTablesValues[2][pulseTablePos] = 0;
					pulseTablePos++;
				}
			}

			// We have vibrato bits set then add these to the table
			if (((mEnvelopes[i].mVibratoUp & 0x0f) || (mEnvelopes[i].mVibratoDown & 0x0f)) && (mEnvelopes[i].mDrumVibratoControl & 0x0f))
			{
				mEnvelopes[i].mTableNote = noteTablePos;

				if ((mEnvelopes[i].mDrumVibratoControl & 0x0f) == 1)
				{
					if ((mEnvelopes[i].mVibratoDown & 0x0f) && !(mEnvelopes[i].mDrumVibratoControl & 0x10))
					{
						mTablesControls[1][noteTablePos] = 0xa0 | ((mEnvelopes[i].mVibratoDown & 0x0f)-1);
						mTablesValues[1][noteTablePos] = (unsigned char) -(mEnvelopes[i].mVibratoDown >> 4);
						noteTablePos++;
					}

					if (mEnvelopes[i].mVibratoUp & 0x0f)
					{
						mTablesControls[1][noteTablePos] = 0xa0 | ((mEnvelopes[i].mVibratoUp & 0x0f)-1);
						mTablesValues[1][noteTablePos] = (unsigned char) mEnvelopes[i].mVibratoUp >> 4;
						noteTablePos++;
					}

					if ((mEnvelopes[i].mVibratoDown & 0x0f) && (mEnvelopes[i].mDrumVibratoControl & 0x10))
					{
						mTablesControls[1][noteTablePos] = 0xa0 | ((mEnvelopes[i].mVibratoDown & 0x0f)-1);
						mTablesValues[1][noteTablePos] = (unsigned char) -(mEnvelopes[i].mVibratoDown >> 4);
						noteTablePos++;
					}
				}
				else
				{
					if ((mEnvelopes[i].mVibratoDown & 0x0f) && !(mEnvelopes[i].mDrumVibratoControl & 0x10))
					{
						mTablesControls[1][noteTablePos] = 0x90 | ((mEnvelopes[i].mVibratoDown & 0x0f)-1);
						mTablesValues[1][noteTablePos] = (((mEnvelopes[i].mVibratoDown >> 4)-1)<<4) | ((mEnvelopes[i].mDrumVibratoControl & 0x0f)-1);
						noteTablePos++;
					}

					if (mEnvelopes[i].mVibratoUp & 0x0f)
					{
						mTablesControls[1][noteTablePos] = 0x80 | ((mEnvelopes[i].mVibratoUp & 0x0f)-1);
						mTablesValues[1][noteTablePos] = (((mEnvelopes[i].mVibratoUp >> 4)-1)<<4) | ((mEnvelopes[i].mDrumVibratoControl & 0x0f)-1);
						noteTablePos++;
					}

					if ((mEnvelopes[i].mVibratoDown & 0x0f) && (mEnvelopes[i].mDrumVibratoControl & 0x10))
					{
						mTablesControls[1][noteTablePos] = 0x90 | ((mEnvelopes[i].mVibratoDown & 0x0f)-1);
						mTablesValues[1][noteTablePos] = (((mEnvelopes[i].mVibratoDown >> 4)-1)<<4) | ((mEnvelopes[i].mDrumVibratoControl & 0x0f)-1);
						noteTablePos++;
					}
				}
				mTablesControls[1][noteTablePos] = 0xff;
				mTablesValues[1][noteTablePos] = oldNoteTablePos;
				noteTablePos++;
			}
		}
	}

	OptimiseTables(true);
}

void CMusicStudioDoc::OptimiseTables(const bool assumeAllEnvelopesUsed)
{
	UpdateDocumentDataToInternalFile();
	if ( mMusicFile.OptimiseTables(assumeAllEnvelopesUsed) )
	{
		SetModifiedFlag();
	}
	ImportEnvelopesAndTablesFromFile();
}

void CMusicStudioDoc::RemoveTableByteAt(const int table, const int position)
{
	UpdateDocumentDataToInternalFile();
	mMusicFile.RemoveTableByteAt(table,position);
	ImportEnvelopesAndTablesFromFile();
}

void CMusicStudioDoc::AddTableByteAt(const int table, const int position)
{
	UpdateDocumentDataToInternalFile();
	mMusicFile.AddTableByteAt(table,position);
	ImportEnvelopesAndTablesFromFile();
}

void CMusicStudioDoc::ImportEnvelopesAndTablesFromFile(void)
{
	int i,j;
	for (i=0;i<MusicStudio1::MusicFile::kMaxEnvelopes;i++)
	{
		mEnvelopes[i] = mMusicFile.mEnvelopes[i];
	}

	for (i=0;i<MusicStudio1::MusicFile::kMaxTables;i++)
	{
		for (j=0;j<MusicStudio1::MusicFile::kMaxTableEntries;j++)
		{
			// Remembering that the document version has an unused 0 entry
			mTablesControls[i][j+1] = mMusicFile.mTablesControls[i][j];
			mTablesValues[i][j+1] = mMusicFile.mTablesValues[i][j];
		}
	}
}

void CMusicStudioDoc::UpdateTrackerBlockToInternal(const int currentBlock)
{
	std::map<int,int> blockTrackerErrorMap;	// A map indexed by text block line number back to the row

	ZeroMemory(mBlockTrackerErrorRows[currentBlock],sizeof(mBlockTrackerErrorRows[currentBlock]));

	CString replacementBlockText(_T(";Tracker block\r\n"));
	int replacementBlockTextLine = 1;
	int i,j;
	int lastDuration = 0;
	int lastDurationRelease = 0;
	CString lastEnvelope;
	CString lastNote;
	bool noteOutput = false;
	blockTrackerErrorMap.clear();
	bool isSluring = false;
	bool wantEffectSlur = false;
	bool usedEffectSlur = false;
	bool processedAloneEffect = false;
	bool isHRDing = false;
	bool wantEffectHRD = false;
	bool usedEffectHRD = false;
	CString lastDurationCommandOut;
	bool afterRelease = false;
	bool justSLR = false;
	bool encounteredNextNoteOrPatternEnd = false;
	bool releaseRestOutput = false;
	bool usedEffectTrackerSLE = false;

	for (i=0;i<mBlockEditTrackerMaxCalcedRow[currentBlock];i++)
	{
		encounteredNextNoteOrPatternEnd = false;
		// By default assume the duration is for the remainder of this block
		int thisDuration = (mBlockEditTrackerMaxCalcedRow[currentBlock] - i) * mBlockTrackerTempos[currentBlock];
		int thisDurationRelease = thisDuration;
		// We have a note and envelope so scan forward looking for a release or another note
		if ( processedAloneEffect || ((i==0) && mBlockTrackerRows[currentBlock][i][0].IsEmpty()) || mBlockTrackerRows[currentBlock][i][0] == "+++" || (!mBlockTrackerRows[currentBlock][i][0].IsEmpty() && !mBlockTrackerRows[currentBlock][i][1].IsEmpty()))
		{
			justSLR = false;
			processedAloneEffect = false;
			for (j=i+1;j<mBlockEditTrackerMaxCalcedRow[currentBlock];j++)
			{
				if (mBlockTrackerRows[currentBlock][j][0].IsEmpty() && mBlockTrackerRows[currentBlock][j][1].IsEmpty() && mBlockTrackerRows[currentBlock][j][2].IsEmpty())
				{
					continue;
				}

				if (mBlockTrackerRows[currentBlock][j][0] == "===")
				{
					thisDurationRelease = (j-i) * mBlockTrackerTempos[currentBlock];
					wantEffectSlur = false;
					afterRelease = true;
				}
				
				if ( ( (mBlockTrackerRows[currentBlock][j][0] == "===") || mBlockTrackerRows[currentBlock][j][0].IsEmpty()) && mBlockTrackerRows[currentBlock][j][1].IsEmpty() && !mBlockTrackerRows[currentBlock][j][2].IsEmpty())
				{
					// Have some effect related command at this position
					processedAloneEffect = true;
					if (afterRelease)
					{
						wantEffectHRD = true;
					}
					else
					{
						if (!isSluring && !usedEffectTrackerSLE)
						{
							wantEffectSlur = true;
							usedEffectTrackerSLE = true;
						}
					}
					thisDuration = (j-i) * mBlockTrackerTempos[currentBlock];
					if (thisDurationRelease > thisDuration)
					{
						thisDurationRelease = thisDuration;
					}
					if (!thisDurationRelease)
					{
						thisDurationRelease = thisDuration;
					}
					break;
				}
				else if (mBlockTrackerRows[currentBlock][j][0] == "===")
				{
					continue;
				}
				else if (mBlockTrackerRows[currentBlock][j][0] == "+++" || (!mBlockTrackerRows[currentBlock][j][0].IsEmpty() && !mBlockTrackerRows[currentBlock][j][1].IsEmpty()))
				{
					encounteredNextNoteOrPatternEnd = true;
					wantEffectHRD = false;
					wantEffectSlur = false;

					// Will be +++ or a note with envelope
					thisDuration = (j-i) * mBlockTrackerTempos[currentBlock];
					if (thisDurationRelease > thisDuration)
					{
						thisDurationRelease = thisDuration;
					}
					if (!thisDurationRelease)
					{
						thisDurationRelease = thisDuration;
					}
					break;
				}
				else if (!mBlockTrackerRows[currentBlock][j][0].IsEmpty() && mBlockTrackerRows[currentBlock][j][1].IsEmpty())
				{
					mBlockTrackerErrorRows[currentBlock][j] = true;
				}
				else if (mBlockTrackerRows[currentBlock][j][0].IsEmpty() && !mBlockTrackerRows[currentBlock][j][1].IsEmpty())
				{
					mBlockTrackerErrorRows[currentBlock][j] = true;
				}
			}

			if (j == mBlockEditTrackerMaxCalcedRow[currentBlock])
			{
				// It finished with the last row being reached so don't flag it as such
				encounteredNextNoteOrPatternEnd = true;
				wantEffectHRD = false;
				wantEffectSlur = false;
			}

			bool anyEffectOutputThisRow = false;
			CString effect = mBlockTrackerRows[currentBlock][i][2];
			int start = 0;
			do
			{
				CString tok = effect.Tokenize(_T(" "),start);
				if (!tok.IsEmpty())
				{
					CStringA line(tok);
					if (_strnicmp(line,"SLE",3) == 0)
					{
						isSluring = true;
						if (usedEffectSlur)
						{
							continue;
						}
					}

					if (_strnicmp(line,"SLR",3) == 0)
					{
						isSluring = false;
						if (usedEffectSlur && !wantEffectSlur)
						{
							continue;
						}
					}

					if (_strnicmp(line,"HRD",3) == 0)
					{
						isHRDing = true;
						if (usedEffectHRD)
						{
							continue;
						}
					}

					if (_strnicmp(line,"HRE",3) == 0)
					{
						isHRDing = false;
						if (usedEffectHRD && !wantEffectHRD)
						{
							continue;
						}
					}
					anyEffectOutputThisRow= true;
					replacementBlockText += tok;
					replacementBlockText += _T("\r\n");
					blockTrackerErrorMap.insert(std::pair<int,int>(replacementBlockTextLine,i));
					replacementBlockTextLine++;
				}
			}while(start > 0);

			if (usedEffectSlur && !wantEffectSlur && !isSluring)
			{
				replacementBlockText += _T("SLR\r\n");
				blockTrackerErrorMap.insert(std::pair<int,int>(replacementBlockTextLine,i));
				replacementBlockTextLine++;
				usedEffectSlur = false;
				afterRelease = false;
				justSLR = true;
			}
			if (!isSluring && wantEffectSlur)
			{
				replacementBlockText += _T("SLE\r\n");
				blockTrackerErrorMap.insert(std::pair<int,int>(replacementBlockTextLine,i));
				replacementBlockTextLine++;
				usedEffectSlur = true;
			}


			if (usedEffectHRD && !wantEffectHRD && !isHRDing)
			{
				replacementBlockText += _T("HRE\r\n");
				blockTrackerErrorMap.insert(std::pair<int,int>(replacementBlockTextLine,i));
				replacementBlockTextLine++;
				anyEffectOutputThisRow = true;
			}
			if (!isHRDing && wantEffectHRD)
			{
				usedEffectTrackerSLE = true;
				replacementBlockText += _T("HRD\r\n");
				blockTrackerErrorMap.insert(std::pair<int,int>(replacementBlockTextLine,i));
				replacementBlockTextLine++;
				usedEffectHRD = true;
			}
			
			if (lastEnvelope != mBlockTrackerRows[currentBlock][i][1])
			{
				lastEnvelope = mBlockTrackerRows[currentBlock][i][1];
				if (!lastEnvelope.IsEmpty())
				{
					replacementBlockText += _T("ENV:");
					replacementBlockText += lastEnvelope;
					replacementBlockText += _T("\r\n");
					blockTrackerErrorMap.insert(std::pair<int,int>(replacementBlockTextLine,i));
					replacementBlockTextLine++;
				}
			}

			if ((lastDuration != thisDuration) || (lastDurationRelease != thisDurationRelease))
			{
				lastDuration = thisDuration;
				lastDurationRelease = thisDurationRelease;
				CString durationCommand;
				// While sluring we can ignore the release duration
				if (!(lastDuration&1) && (isSluring || usedEffectSlur || (lastDurationRelease*2) == lastDuration) || ((i==0) && mBlockTrackerRows[currentBlock][i][0].IsEmpty()))
				{
					durationCommand.Format(_T("DUR:%x\r\n"),lastDuration/2);
				}
				else
				{
					durationCommand.Format(_T("DTI:%x,%x\r\n"),lastDuration,lastDurationRelease);
				}

				if (lastDurationCommandOut != durationCommand)
				{
					lastDurationCommandOut = durationCommand;
					replacementBlockText += durationCommand;
					blockTrackerErrorMap.insert(std::pair<int,int>(replacementBlockTextLine,i));
					replacementBlockTextLine++;
				}
			}

			if ((i==0) && mBlockTrackerRows[currentBlock][i][0].IsEmpty())
			{
				lastNote = "+++";
			}
			else if (!mBlockTrackerRows[currentBlock][i][0].IsEmpty() && (mBlockTrackerRows[currentBlock][i][0] != "==="))
			{
				lastNote = mBlockTrackerRows[currentBlock][i][0];
			}

			if (!justSLR && noteOutput && (isHRDing || usedEffectHRD))
			{
				if (mBlockTrackerRows[currentBlock][i][0] == "===" && anyEffectOutputThisRow && !releaseRestOutput)
				{
					replacementBlockText += "===";
					releaseRestOutput = true;
				}
				else
				{
					replacementBlockText += "+++";
				}
			}
			else
			{
				replacementBlockText += lastNote;
			}
			replacementBlockText += _T("\r\n");
			blockTrackerErrorMap.insert(std::pair<int,int>(replacementBlockTextLine,i));
			replacementBlockTextLine++;

			if (lastNote != "+++" && lastNote != "===")
			{
				noteOutput = true;
			}

			if (encounteredNextNoteOrPatternEnd)
			{
				afterRelease = false;
				wantEffectSlur = false;
				usedEffectSlur = false;
				processedAloneEffect = false;
				wantEffectHRD = false;
				usedEffectHRD = false;
				noteOutput = false;
				releaseRestOutput = false;
				usedEffectTrackerSLE = false;
			}

			i = j-1;
		}
	}
	mBlocks[currentBlock] = replacementBlockText;

	CStringA ansiBlock(mBlocks[currentBlock]);
	int byteSize = 0;
	int totalDuration = 0;
	mMusicFile.SetBlockFromText(currentBlock,ansiBlock,mBlockErrorReport[currentBlock],byteSize,totalDuration);

	std::list<int>::iterator st = mBlockErrorReport[currentBlock].begin();
	while (st != mBlockErrorReport[currentBlock].end())
	{
		int line = *st++;
		std::map<int,int>::iterator found = blockTrackerErrorMap.find(line);
		if (found != blockTrackerErrorMap.end())
		{
			int row = (*found).second;
			mBlockTrackerErrorRows[currentBlock][row] = true;
		}
	}
}

void CMusicStudioDoc::ClearCapturedSIDData(void)
{
	mSIDEvents.clear();
}

void CMusicStudioDoc::SIDRegisterWrite(const double clock,const unsigned char reg,const unsigned char data)
{
//	char temp[256];
//	sprintf(temp,"%f SID write $%x data $%x\n",clock,reg,data);
//	OutputDebugStringA(temp);

	mSIDEvents.push_back(SIDWriteEvent(clock,reg,data));
}

static bool ProcessEvents(unsigned char SIDBytes[0x20],std::list<SIDWriteEvent>::iterator &st,std::list<SIDWriteEvent>::iterator &en,const double untilTime)
{
	while (st != en)
	{
		SIDWriteEvent &tick = *st;
		if (tick.mClock >= untilTime)
		{
			return true;
		}
		assert(tick.mReg < 0x20);
		SIDBytes[tick.mReg] = tick.mData;
		st++;
	}
	return false;
}

static const double kFPS = 50.0f;
static const double kOneFrame = 1.0f / kFPS;
// Most music routines will complete SID writes within this fraction of a frame
static const double kFrameFraction = kOneFrame / 4.0f;

static CString PitchToNote(double pitch)
{
	// Log base 2 and adjust for the note index
	double octave = (log(pitch) / log(2.0f)) - 8.0f;
	octave *= 12.0f;

	CString note(MusicStudio1::BlockEntry::GetNoteFromNumber(int(octave)).c_str());

	return note;
}

static int CleanControl(int control , const bool addGate = false)
{
	control &= kSIDVoiceControl_Mask_Gate | kSIDVoiceControl_Mask_Ring | kSIDVoiceControl_Mask_Triangle | kSIDVoiceControl_Mask_Sawtooth | kSIDVoiceControl_Mask_Pulse | kSIDVoiceControl_Mask_Noise;
	if (addGate)
	{
		control |= kSIDVoiceControl_Mask_Gate;
	}
	return control;
}

void CMusicStudioDoc::ProcessSIDCaptureData(void)
{
	// Process the ripped music events into the document

	// Find the first non-blank waveform and note the time
	double firstUsefulTime = 0.0f;
	std::list<SIDWriteEvent>::iterator st,en;
	st = mSIDEvents.begin();
	while (st != mSIDEvents.end())
	{
		SIDWriteEvent &tick = *st++;
		if ((tick.mReg == 0x04) || (tick.mReg == 0x0b) || (tick.mReg == 0x12))
		{
			// Waveform, with gate and no test bit set
			if ( (tick.mData & 0xf0) && !(tick.mData & 0x08) && (tick.mData&0x01) )
			{
				firstUsefulTime = tick.mClock;
				break;
			}
		}
	}

	const double kMaxSecondsPerBlock = 4;

	// MPi: TODO: Calculate sensible block sizes based on analysis for the ripped SID
	// Some default values
	int i;
	for (i=0;i < kSIDMaxVoice;i++)
	{
		mTracks[i][0] = i;
		mTracks[i][1] = MusicStudio1::kMusicPlayer_StopTrack;
	}
	for (i=0;i < MusicStudio1::MusicFile::kMaxBlocks;i++)
	{
		mBlockLastEditedAsTracker[i] = true;
		mBlockEditTrackerMaxCalcedRow[i] = (int)(kFPS * kMaxSecondsPerBlock);//kMaxInternalTrackerRows;
		mBlockTrackerLengths[i] = mBlockEditTrackerMaxCalcedRow[i];
		mBlockTrackerTempos[i] = 1;
		int j;
		for (j=0;j<kMaxInternalTrackerRows;j++)
		{
			mBlockTrackerRows[i][j][0].Empty();
			mBlockTrackerRows[i][j][1].Empty();
			mBlockTrackerRows[i][j][2].Empty();
		}
	}
	// Parse events and maintain SID state
	unsigned char SIDBytes[0x20];
	ZeroMemory(SIDBytes,sizeof(SIDBytes));
	unsigned char previousSIDBytes[0x20];
	ZeroMemory(previousSIDBytes,sizeof(previousSIDBytes));


	int chan;

	st = mSIDEvents.begin();
	en = mSIDEvents.end();
	// Until we parse all the events
	double nowTime = firstUsefulTime;
	double startBlockTime = nowTime;
	int blockOffset = 0;
	int trackOffset = 0;
	CString lastNote[kSIDMaxVoice];
	CString lastEnvelope[kSIDMaxVoice];
	while (ProcessEvents(SIDBytes,st,en,nowTime + kFrameFraction))
	{
		// Progress onto new blocks when they reach their end times
		if ( (nowTime - startBlockTime) >= kMaxSecondsPerBlock )
		{
			startBlockTime += kMaxSecondsPerBlock;
			blockOffset += kSIDMaxVoice;
			trackOffset++;
			if (blockOffset < MusicStudio1::MusicFile::kMaxBlocks)
			{
				for (chan=0;chan<kSIDMaxVoice;chan++)
				{
					mTracks[chan][trackOffset] = blockOffset+chan;
					mTracks[chan][trackOffset+1] = MusicStudio1::kMusicPlayer_StopTrack;
				}
			}
		}
		if (blockOffset >= MusicStudio1::MusicFile::kMaxBlocks)
		{
			break;
		}
		int currentRow = int(((nowTime - startBlockTime)/*+(kOneFrame/2.0f)*/) * kFPS);
		if (currentRow < 0)
		{
			currentRow = 0;
		}
		else if (currentRow >= mBlockEditTrackerMaxCalcedRow[0])
		{
			currentRow = mBlockEditTrackerMaxCalcedRow[0]-1;
		}
		for (chan=0;chan<kSIDMaxVoice;chan++)
		{
			int realBlockIndex = blockOffset + chan;
			// Ignore hard resets for notes
			if (SIDBytes[(chan*kSIDVoice_Size)+kSIDVoiceControl] & kSIDVoiceControl_Mask_Test)
			{
				continue;
			}

			if ((SIDBytes[(chan*kSIDVoice_Size)+kSIDVoiceControl] & kSIDVoiceControl_Mask_Gate) && !(previousSIDBytes[(chan*kSIDVoice_Size)+kSIDVoiceControl] & kSIDVoiceControl_Mask_Gate))
			{
				// Gate on this frame
				lastNote[chan] = PitchToNote(double(int(SIDBytes[(chan*kSIDVoice_Size)+kSIDVoiceFreqLo]) | (int(SIDBytes[(chan*kSIDVoice_Size)+kSIDVoiceFreqHi]) << 8)));
				mBlockTrackerRows[realBlockIndex][currentRow][0] = lastNote[chan];
				int instrument = GetInstrument(SIDBytes + (chan*kSIDVoice_Size));
				lastEnvelope[chan] = CString(RNReplicaNet::ToString(instrument).c_str());
				mBlockTrackerRows[realBlockIndex][currentRow][1] = lastEnvelope[chan];
			}
			else if (!(SIDBytes[(chan*kSIDVoice_Size)+kSIDVoiceControl] & kSIDVoiceControl_Mask_Gate) && (previousSIDBytes[(chan*kSIDVoice_Size)+kSIDVoiceControl] & kSIDVoiceControl_Mask_Gate))
			{
				// Release on this frame
				mBlockTrackerRows[realBlockIndex][currentRow][0] = "===";
			}
			else if ( (SIDBytes[(chan*kSIDVoice_Size)+kSIDVoiceFreqLo] != previousSIDBytes[(chan*kSIDVoice_Size)+kSIDVoiceFreqLo]) || (SIDBytes[(chan*kSIDVoice_Size)+kSIDVoiceFreqHi] != previousSIDBytes[(chan*kSIDVoice_Size)+kSIDVoiceFreqHi]) )
			{
				// Note change
				CString newNote = PitchToNote(double(int(SIDBytes[(chan*kSIDVoice_Size)+kSIDVoiceFreqLo]) | (int(SIDBytes[(chan*kSIDVoice_Size)+kSIDVoiceFreqHi]) << 8)));
				if (newNote != lastNote[chan])
				{
					lastNote[chan] = newNote;
					mBlockTrackerRows[realBlockIndex][currentRow][0] = lastNote[chan];
					if (lastEnvelope[chan].IsEmpty())
					{
						int instrument = GetInstrument(SIDBytes + (chan*kSIDVoice_Size));
						lastEnvelope[chan] = CString(RNReplicaNet::ToString(instrument).c_str());
					}
					mBlockTrackerRows[realBlockIndex][currentRow][1] = lastEnvelope[chan];
				}
			}
		}
		
		memcpy(previousSIDBytes,SIDBytes,sizeof(previousSIDBytes));
		nowTime += kOneFrame;
	}

	for (i=0;i<MusicStudio1::MusicFile::kMaxBlocks;i++)
	{
		UpdateTrackerBlockToInternal(i);
	}

	UpdateDocumentDataToInternalFile();
}

int CMusicStudioDoc::GetInstrument(const unsigned char *SIDBytes)
{
	int i;
	int maxInstrument = 0;
	int maxTableWave = 0;
	int maxTablePulse = 0;

	for (i = 1 ; i < MusicStudio1::MusicFile::kMaxEnvelopes ; i++)
	{
		if (mEnvelopes[i].mTableWave > 0 && i > maxInstrument)
		{
			maxInstrument = i;
		}
		if (mEnvelopes[i].mAttackDecay == SIDBytes[kSIDVoiceAttackDecay] &&
			mEnvelopes[i].mSustainRelease == SIDBytes[kSIDVoiceSustainRelease] &&
			mEnvelopes[i].mActiveTableWave &&
			mTablesControls[kTableIndex_Wave][mEnvelopes[i].mTableWave] == CleanControl(SIDBytes[kSIDVoiceControl] , true)
			)
		{
			if (SIDBytes[kSIDVoiceControl] & kSIDVoiceControl_Mask_Pulse)
			{
				if (mEnvelopes[i].mActiveTablePulse &&
					mTablesControls[kTableIndex_Pulse][mEnvelopes[i].mTablePulse] == (SIDBytes[kSIDVoicePulseWidthHi] & 0x0f) &&
					mTablesValues[kTableIndex_Pulse][mEnvelopes[i].mTablePulse] == SIDBytes[kSIDVoicePulseWidthLo])
				{
					return i;
				}
			}
			else
			{
				return i;
			}
		}
	}

	for (i = 1 ; i < MusicStudio1::MusicFile::kMaxTableEntries ; i++)
	{
		if ( (mTablesControls[kTableIndex_Wave][i] == 0xff))
		{
			maxTableWave = i;
		}
		if ( (mTablesControls[kTableIndex_Pulse][i] == 0xff))
		{
			maxTablePulse = i;
		}
	}

	// Create a new instrument
	maxInstrument++;
	maxTableWave++;
	maxTablePulse++;
	mEnvelopeNames[maxInstrument] = "Created";
	mEnvelopes[maxInstrument].mAttackDecay = SIDBytes[kSIDVoiceAttackDecay];
	mEnvelopes[maxInstrument].mSustainRelease = SIDBytes[kSIDVoiceSustainRelease];
	mEnvelopes[maxInstrument].mActiveTableWave = true;
	for (i = 1 ; i < MusicStudio1::MusicFile::kMaxTableEntries ; i++)
	{
		if ( mTablesControls[kTableIndex_Wave][i] == CleanControl(SIDBytes[kSIDVoiceControl] , true) )
		{
			mEnvelopes[maxInstrument].mTableWave = i;
			break;
		}
	}
	if (mEnvelopes[maxInstrument].mTableWave == 0)
	{
		mEnvelopes[maxInstrument].mTableWave = maxTableWave;
		mTablesControls[kTableIndex_Wave][maxTableWave] = CleanControl(SIDBytes[kSIDVoiceControl] , true);
		mTablesValues[kTableIndex_Wave][maxTableWave] = 0;
		mTablesControls[kTableIndex_Wave][maxTableWave+1] = 0xff;
		mTablesValues[kTableIndex_Wave][maxTableWave+1] = 0;
	}
	if (SIDBytes[kSIDVoiceControl] & kSIDVoiceControl_Mask_Pulse)
	{
		mEnvelopes[maxInstrument].mActiveTablePulse = true;

		for (i = 1 ; i < MusicStudio1::MusicFile::kMaxTableEntries ; i++)
		{
			if ( mTablesControls[kTableIndex_Pulse][i] == (SIDBytes[kSIDVoicePulseWidthHi] & 0x0f) &&
				mTablesControls[kTableIndex_Pulse][i] == SIDBytes[kSIDVoicePulseWidthLo])
			{
				mEnvelopes[maxInstrument].mTablePulse = i;
				break;
			}
		}

		if (mEnvelopes[maxInstrument].mTablePulse == 0)
		{
			mEnvelopes[maxInstrument].mTablePulse = maxTablePulse;
			mTablesControls[kTableIndex_Pulse][maxTablePulse] = SIDBytes[kSIDVoicePulseWidthHi] & 0x0f;
			mTablesValues[kTableIndex_Pulse][maxTablePulse] = SIDBytes[kSIDVoicePulseWidthLo];
			mTablesControls[kTableIndex_Pulse][maxTablePulse+1] = 0xff;
			mTablesValues[kTableIndex_Pulse][maxTablePulse+1] = 0;
		}
	}

	OptimiseTables(true);

	return maxInstrument;
}
