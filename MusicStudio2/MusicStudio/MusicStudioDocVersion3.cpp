#include "stdafx.h"
#include "MusicStudio.h"
#include "MusicStudioDoc.h"

// Version 2.0
// Limited release to NecroPolo and Richard Bayliss
void CMusicStudioDoc::LoadVersion3(CArchive& ar)
{
	ar >> mUsing6581;

	int i,j;
	for (i=0;i<MusicStudio1::MusicFile::kMaxTracks;i++)
	{
		for (j=0;j<MusicStudio1::MusicFile::kMaxTrackLength;j++)
		{
			ar >> mTracks[i][j];
		}
	}

	for (i=0;i<MusicStudio1::MusicFile::kMaxBlocks;i++)
	{		
		ar >> mBlocks[i];
	}

	for (i=0;i<MusicStudio1::MusicFile::kOldMaxEnvelopes;i++)
	{		
		ar >> mEnvelopeNames[i];
		ar >> mEnvelopes[i].mAttackDecay;
		ar >> mEnvelopes[i].mSustainRelease;
		ar >> mEnvelopes[i].mWaveControl;
		ar >> mEnvelopes[i].mPulseWidthVariance;
		ar >> mEnvelopes[i].mVibratoUp;
		ar >> mEnvelopes[i].mVibratoDown;
		ar >> mEnvelopes[i].mDrumVibratoControl;
		ar >> mEnvelopes[i].mWaveMix;
		// Save this so later on it can be used for flagging sound effects not used in the music
		ar >> mEnvelopes[i].mIsUsed;
		ar >> mEnvelopes[i].mTranspose;
	}

	for (i=0;i<MusicStudio1::MusicFile::kOldMaxDrums;i++)
	{		
		ar >> mDrumNames[i];
		for (j=0;j<8;j++)
		{
			ar >> mDrums[i].mHiNotes[j];
			ar >> mDrums[i].mWaveControl[j];
		}
		ar >> mDrums[i].mStartLoop;
		ar >> mDrums[i].mEndLoop;
	}

	for (i=0;i<MusicStudio1::MusicFile::kMaxExtendedArpeggios;i++)
	{
		ar >> mExtendedArpeggiosNames[i];
		ar >> mExtendedArpeggios[i].mLength;
		for (j=0;j<MusicStudio1::ExtendedArpeggio::kMaxExtendedArpeggioData;j++)
		{
			ar >> mExtendedArpeggios[i].mArpeggios[j];
		}
	}

	ar >> mGenericInfo;

	ar >> mTitleInfo;
	ar >> mAuthorInfo;
	ar >> mReleasedInfo;
}
