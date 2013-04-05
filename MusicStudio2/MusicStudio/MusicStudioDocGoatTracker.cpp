// MusicStudioDocGoatTracker.cpp : implementation of the CMusicStudioDoc class for loading Goat tracker songs
//

#include "stdafx.h"
#include "MusicStudio.h"

#include "MusicStudioDoc.h"
#include "MusicStudioView2.h"
#include "ChildFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


bool CMusicStudioDoc::LoadGoatTrackerSong(CArchive &ar,const int version)
{
	if (!ar.IsLoading())
	{
		return false;
	}

	OnNewDocument();

	// Get song title, author and release info
	char tempString[33];
	ar.Read(tempString,32);
	tempString[32] = 0;
	mTitleInfo = tempString;
	ar.Read(tempString,32);
	tempString[32] = 0;
	mAuthorInfo = tempString;
	ar.Read(tempString,32);
	tempString[32] = 0;
	mReleasedInfo = tempString;

	mGenericInfo = CString(_T("Originaly converted from the Goat Tracker file: ")) + ar.GetFile()->GetFileName();
	
	unsigned char tempUChar;
	short tempShort = 0;
	int tempInt = 0;

	unsigned char numSubTunes;

	// Number of sub-tunes
	ar >> numSubTunes;

	// Track data
	ZeroMemory(mTracks,sizeof(mTracks));
	int i,j;

	for (i=0;i<3;i++)
	{
		// Setup something by default
		mTracks[i][0] = MusicStudio1::kMusicPlayer_StopTrack;

		j = 0;
		int realNumSubTunes = numSubTunes;
		while (realNumSubTunes-- > 0)
		{
			unsigned char trackLen;
			ar >> trackLen;

			for (;trackLen>0;trackLen--)
			{
				ar >> tempUChar;
				// Hope that the file doesn't have a lot of blocks
				if (tempUChar < MusicStudio1::kMusicPlayer_RepeatBlock)
				{
					mTracks[i][j++] = tempUChar;
				}
				else if (tempUChar == 0xff)
				{
					mTracks[i][j++] = MusicStudio1::kMusicPlayer_StopTrack;
					// Skip the next
					ar >> tempUChar;
				}
				else
				{
					OutputDebugStringA("Unknown\n");
				}
			}

			// Cap off the end of the track
			if (j > 0)
			{
				if (mTracks[i][j-1] < MusicStudio1::kMusicPlayer_RepeatBlock)
				{
					mTracks[i][j] = MusicStudio1::kMusicPlayer_StopTrack;
				}
			}
		}
	}

	int numInstr = 32;
	
	if (version == kGoatTrackerV2)
	{
		ar >> tempUChar;
		numInstr = tempUChar;
		numInstr++;
	}

	// Then FX data
	for (i=1;i<numInstr;i++)
	{
		mEnvelopes[i].mDrumVibratoControl = 0;
		mEnvelopes[i].mPulseWidthVariance = 0;
		if (version == kGoatTrackerV1)
		{
			ar >> mEnvelopes[i].mAttackDecay;
			ar >> mEnvelopes[i].mSustainRelease;
			ar >> mEnvelopes[i].mPulseWidthVariance;
			mEnvelopes[i].mPulseWidthVariance &= 0xf;
			// MPi: TODO: Pulse and filter control
			ar >> tempUChar;
			ar >> tempUChar;
			ar >> tempUChar;
			ar >> tempUChar;

			// MPi: TODO: Find out what this is.
			ar >> tempUChar;
		}
		else if (version == kGoatTrackerV2)
		{
			ar >> mEnvelopes[i].mAttackDecay;
			ar >> mEnvelopes[i].mSustainRelease;
			// MPi: TODO: Table pointers
			ar >> mEnvelopes[i].mWaveControl;
			ar >> mEnvelopes[i].mPulseWidthVariance;
			ar >> tempUChar;
			ar >> tempUChar;

			ar >> tempUChar;
			ar >> tempUChar;
			ar >> tempUChar;
		}

		ar.Read(tempString,16);
		tempString[16] = 0;
		mEnvelopeNames[i] = tempString;

		if (version == kGoatTrackerV1)
		{
			// Parse the wave table
			unsigned char nextWave = 0;
			unsigned char lastWave = 0;
			
			ar >> nextWave;
			lastWave = 0;
			while (nextWave != 0xff)
			{
				lastWave = mEnvelopes[i].mWaveControl;
				mEnvelopes[i].mWaveControl = nextWave | 1;
				if ((nextWave & 0x40) && !mEnvelopes[i].mPulseWidthVariance)
				{
					mEnvelopes[i].mPulseWidthVariance = 8;
				}
				// MPi: TODO: Find out what this is.
				ar >> tempUChar;

				ar >> nextWave;
			}

			// MPi: TODO: Need to do more in-depth conversion of instruments and the wave table
#if 0
			if ((lastWave & 0xf0) != (mEnvelopes[i].mWaveControl & 0xf0))
			{
				mEnvelopes[i].mWaveMix = (lastWave | mEnvelopes[i].mWaveControl) & 0xf0;
			}
#endif

			// Trim silly values
			if (!((mEnvelopes[i].mWaveControl | mEnvelopes[i].mWaveMix) & 0x40))
			{
				mEnvelopes[i].mPulseWidthVariance = 0;
			}

			// Spare byte at the end of the wave table
			ar >> tempUChar;
		}//< if (version == kGoatTrackerV1)
	}

	std::pair<unsigned char,unsigned char> tables[4][256];
	// Parse any table information
	if (version == kGoatTrackerV2)
	{
		for (i=0;i<4;i++)
		{
			ar >> tempUChar;
			int rows = tempUChar;
			for (j=0;j<rows;j++)
			{
				ar >> tempUChar;
				tables[i][j].first = tempUChar;
			}

			for (j=0;j<rows;j++)
			{
				ar >> tempUChar;
				tables[i][j].second = tempUChar;
			}
		}

		// Update the wave and pulse information from the table
		for (i=1;i<numInstr;i++)
		{
			size_t thePointer = mEnvelopes[i].mWaveControl - 1;
			unsigned char firstWave = 0;
			unsigned char nextWave = 0;
			while ((thePointer < 256) && tables[0][thePointer].first != 0xff)
			{
				if ((tables[0][thePointer].first & 0xf0) <= 0x80)
				{
					nextWave = firstWave;
					firstWave = tables[0][thePointer].first;
				}
				thePointer++;
			}
			if (firstWave & 0xf0)
			{
				mEnvelopes[i].mWaveControl = firstWave | 1;
			}
			else
			{
				// Set a default when all else fails
				mEnvelopes[i].mWaveControl = 0x11;
			}
			// No wave mixing detected
			if ((firstWave & 0xf0) == (nextWave & 0xf0) || !(nextWave & 0xf0))
			{
				mEnvelopes[i].mWaveMix = 0;
			}
			else
			{
				mEnvelopes[i].mWaveMix = (nextWave | firstWave) & 0xf0;
			}
			// Any pulse?
			if ((firstWave | nextWave) & 0x40)
			{
				if (mEnvelopes[i].mPulseWidthVariance)
				{
					mEnvelopes[i].mPulseWidthVariance = tables[1][mEnvelopes[i].mPulseWidthVariance-1].first;
					mEnvelopes[i].mPulseWidthVariance |= mEnvelopes[i].mPulseWidthVariance >> 4;
				}
				else
				{
					mEnvelopes[i].mPulseWidthVariance = 8;
				}
			}
			else
			{
				mEnvelopes[i].mPulseWidthVariance = 0;
			}
		}

	}


	unsigned char numBlocks;
	ar >> numBlocks;

	if (numInstr > MusicStudio1::MusicFile::kMaxBlocks)
	{
		MessageBox(0,_T("This Goat Track file has too many blocks to load"),0,MB_OK);
		return false;
	}


	for (i=0;i<numBlocks;i++)
	{
		unsigned char blockLen;
		ar >> blockLen;

		bool anyNoteOutput = false;
		unsigned char currentNote = 0;
		int lastBlockTime = 0;
		unsigned char currentBlockTime = 0;
		unsigned char lastBlockEnv = 0;
		unsigned char lastBlockEnvOutput = 255;
		int lastBlockDurOutput = -1;

		mBlocks[i].Empty();

		while (blockLen)
		{
			if (version == kGoatTrackerV1)
			{
				blockLen -= 3;
			}
			else if (version == kGoatTrackerV2)
			{
				blockLen--;
			}

			unsigned char blockNoteRead;
			ar >> blockNoteRead;
			unsigned char blockSoundRead;
			ar >> blockSoundRead;
			unsigned char blockControlRead;
			ar >> blockControlRead;

			if (version == kGoatTrackerV2)
			{
				ar >> tempUChar;
				if ((blockNoteRead >= 0x60) && (blockNoteRead <= 0xbc))
				{
					blockNoteRead -= 0x60;
				}
			}

			CStringA command;
			
			if (((blockNoteRead == 0xff) || (blockNoteRead < 0x5e)) && (currentBlockTime > 0))
			{
				int nextDur = currentBlockTime - lastBlockTime;
				int durSetThisChunk;
				while (nextDur > 0)
				{
					durSetThisChunk = nextDur;
					if (durSetThisChunk > 0x7f)
					{
						durSetThisChunk = 0x7f;
					}
					if (lastBlockDurOutput != durSetThisChunk)
					{
						command.Format("DUR:%02x\r\n",(unsigned char)durSetThisChunk);
						mBlocks[i].Append(CString(command));
						lastBlockDurOutput = durSetThisChunk;
					}

					if (lastBlockEnvOutput != lastBlockEnv)
					{
						if (lastBlockEnv && (blockNoteRead < 0x5e))
						{
							command.Format("ENV:%02x\r\n",lastBlockEnv);
							mBlocks[i].Append(CString(command));
							lastBlockEnvOutput = lastBlockEnv;
						}
					}

					if (lastBlockEnv)
					{
						command = MusicStudio1::BlockEntry::GetNoteFromNumber(currentNote).c_str();
						anyNoteOutput = true;
					}
					else
					{
						command = "+++";
					}

					mBlocks[i].Append(CString(command) + _T("\r\n"));

					nextDur -= durSetThisChunk;
				}
			}

			if (blockNoteRead < 0x5e)
			{
				currentNote = blockNoteRead;
				lastBlockTime = currentBlockTime;
				if (version == kGoatTrackerV1)
				{
					lastBlockEnv = blockSoundRead / 8;
				}
				else if (version == kGoatTrackerV2)
				{
					lastBlockEnv = blockSoundRead;
				}

			}
			else if (blockNoteRead == 0x5e)
			{
			}
			
			currentBlockTime+=3;
		}
	}

	return true;
}
