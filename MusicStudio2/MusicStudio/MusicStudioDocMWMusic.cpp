// MusicStudioDocMIDI.cpp : implementation of the CMusicStudioDoc class for loading MIDI files
//

#include "stdafx.h"
#include "MusicStudio.h"

#include "MusicStudioDoc.h"
#include "MusicStudioView2.h"
#include "ChildFrm.h"
#include "ImportMIDI.h"
#include "RNPlatform/Inc/MessageHelper.h"

#include "MWMusic/CompactorTool/Music.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool CMusicStudioDoc::LoadMWMusicFile(CArchive &ar)
{
	if (!ar.IsLoading())
	{
		return false;
	}

	OnNewDocument();

	mGenericInfo = CString(_T("Originaly converted from the MW music file: ")) + ar.GetFile()->GetFileName() + _T("\x0d\x0a");

	Music music;
	CStringA filenameA(ar.GetFile()->GetFilePath());
	const char *filename = filenameA.GetBuffer();
	music.LoadMusic(filename);
	music.Compact();

	ZeroMemory(mTracks,sizeof(mTracks));
	ZeroMemory(mTablesControls,sizeof(mTablesControls));
	ZeroMemory(mTablesValues,sizeof(mTablesValues));

	for (int i = 0 ; i < MusicStudio1::MusicFile::kMaxEnvelopes ; i++)
	{
		mEnvelopeNames[i] = "";
		mEnvelopes[i] = MusicStudio1::Envelope();
	}

	OptimiseTables(true);

	const std::vector<Block> &tracks = music.getTracks();

	size_t channelPos[3] = {0,0,0};
	size_t blockIndex = 0;
	int channel = 0;

	for (size_t blockIndex = 0 ; blockIndex < tracks.size() ; blockIndex++)
	{
		channel = blockIndex % MusicStudio1::MusicFile::kMaxTracks;

		if (channelPos[channel] >= MusicStudio1::MusicFile::kMaxTrackLength)
		{
			break;
		}

		const Block &trackBlock = tracks[blockIndex];

		for (size_t i = 0 ; i < trackBlock.size() ; i++)
		{
			if (trackBlock[i] == 0xff )
			{
				mTracks[channel][channelPos[channel]] = MusicStudio1::kMusicPlayer_LoopTrack;
				channelPos[channel]++;
				break;
			}
			else if (trackBlock[i] == 0xfe )
			{
				mTracks[channel][channelPos[channel]] = MusicStudio1::kMusicPlayer_StopAllTracks;
				channelPos[channel]++;
				break;
			}
			else if (trackBlock[i] == 0xfd )
			{
				// TODO: fade down?
				mTracks[channel][channelPos[channel]] = MusicStudio1::kMusicPlayer_StopAllTracks;
				channelPos[channel]++;
				break;
			}
			if ((trackBlock[i] & 0x40) == 0x40)
			{
				// TODO: Loop start/end across a range of track entries
				// Ignore for now
				i++;
			}
			unsigned char numTimes = trackBlock[i];
			i++;
			unsigned char sequence = trackBlock[i];
			// TODO: Output a warning if the sequence/block index is out of range of MusicStudio1::MusicFile::kMaxBlocks ?
			// Finally the sequence number
			if (sequence >= MusicStudio1::MusicFile::kMaxBlocks)
			{
				continue;
			}

			if (numTimes > 2)
			{
				mTracks[channel][channelPos[channel]] = MusicStudio1::kMusicPlayer_RepeatBlock | numTimes;
				channelPos[channel]++;
				if (channelPos[channel] >= MusicStudio1::MusicFile::kMaxTrackLength)
				{
					break;
				}
			}

			// Finally the sequence number
			// The sequence range is less in this format
			mTracks[channel][channelPos[channel]] = sequence & (MusicStudio1::MusicFile::kMaxBlocks - 1);
			channelPos[channel]++;
			if (channelPos[channel] >= MusicStudio1::MusicFile::kMaxTrackLength)
			{
				break;
			}
			// Small optimisation to avoid a repeat block instruction being output
			if (numTimes == 2)
			{
				mTracks[channel][channelPos[channel]] = mTracks[channel][channelPos[channel]-1];
				channelPos[channel]++;
			}
		}
	}

	const std::vector<Block> &sequences = music.getSequences();
	for (size_t sequence = 0 ; sequence < sequences.size() ; sequence++)
	{
		if (sequence >= MusicStudio1::MusicFile::kMaxBlocks)
		{
			break;
		}

		const Block &sequenceBlock = sequences[sequence];

		CString blockText;
		unsigned char lastEnv = 0xff;
		unsigned char lastDur = 0xff;

		for (size_t i = 0 ; i < sequenceBlock.size() ; i++)
		{
			unsigned char lengthByte = sequenceBlock[i];
			i++;
			if (lengthByte == 0xff)
			{
				break;
			}

			if (lengthByte & 0x80)
			{
				// Change envelope
				if (i >= sequenceBlock.size())
				{
					break;
				}
				unsigned char env = sequenceBlock[i];
				i++;

				if (env & 0x80)
				{
					// TODO: Bend instead of voice change?
				}
				else
				{
					if (env != lastEnv)
					{
						lastEnv = env;
						blockText += L"ENV:" + GetNum(lastEnv) + L"\r\n";
					}
				}
			}

			lengthByte = lengthByte & 0x3f;
			if (lengthByte != lastDur)
			{
				lastDur = lengthByte;
				blockText += L"DUR:" + GetNum(lastDur) + L"\r\n";
			}
			if (i >= sequenceBlock.size())
			{
				break;
			}
			unsigned char noteByte = sequenceBlock[i];
			noteByte &= 0x7f;

			blockText += CString(MusicStudio1::BlockEntry::GetNoteFromNumber(noteByte).c_str()) + L"\r\n";
		}

		mBlocks[sequence] = blockText;
	}

	const std::vector<Block> &effect1 = music.getEffect1();
	const std::vector<Block> &effect2 = music.getEffect2();

	for (size_t effect = 0 ; effect < effect1.size() ; effect++)
	{
		if (effect >= MusicStudio1::MusicFile::kMaxEnvelopes)
		{
			break;
		}

		mEnvelopeNames[effect] = _T("A note");
		mEnvelopes[effect].mAttackDecay = effect1[effect][0];
		mEnvelopes[effect].mSustainRelease = effect1[effect][1];
		mEnvelopes[effect].mAllowVoiceEffects = true;
		int nextTableWave = getNextFreeTableControl(MusicStudio1::kTableIndex_Wave);
		int nextTableNote = getNextFreeTableControl(MusicStudio1::kTableIndex_Note);
		int nextTablePulse = getNextFreeTableControl(MusicStudio1::kTableIndex_Pulse);
		if (nextTableWave > 0)
		{
			mEnvelopes[effect].mActiveTableWave = true;
			mEnvelopes[effect].mTableWave = nextTableWave;
			mTablesControls[MusicStudio1::kTableIndex_Wave][nextTableWave] = effect1[effect][2];
			mTablesControls[MusicStudio1::kTableIndex_Wave][nextTableWave+1] = 0xff;

			if ( (effect1[effect][2] & MusicStudio1::kSIDVoiceControl_Mask_Pulse) && nextTablePulse > 0)
			{
				mEnvelopes[effect].mActiveTablePulse = true;
				mEnvelopes[effect].mTablePulse = nextTablePulse;
				mTablesControls[MusicStudio1::kTableIndex_Pulse][nextTablePulse] = effect1[effect][4] & 0x0f;
				mTablesValues[MusicStudio1::kTableIndex_Pulse][nextTablePulse] = effect1[effect][4];
				mTablesControls[MusicStudio1::kTableIndex_Pulse][nextTablePulse+1] = 0xff;
				// TODO: effect1[effect][5] is used for "PULSE WIDTH MODULATION CODE"
			}

			// Handle: ATK NOTE SPECIAL
			if ( (effect1[effect][3] & 0x20)  && nextTableNote > 0 )
			{
				mEnvelopes[effect].mActiveTableNote = true;
				mEnvelopes[effect].mTableNote = nextTableNote;
				if (effect2[effect][1] == 0)
				{
					mTablesControls[MusicStudio1::kTableIndex_Note][nextTableNote] = 0xb0 | (effect1[effect][7] & 0x0f);
					mTablesValues[MusicStudio1::kTableIndex_Note][nextTableNote] = 0;
					mTablesControls[MusicStudio1::kTableIndex_Note][nextTableNote+1] = 0xff;
				}
				else
				{
					mTablesControls[MusicStudio1::kTableIndex_Note][nextTableNote] = 0x10 | (effect1[effect][7] & 0x0f);
					mTablesValues[MusicStudio1::kTableIndex_Note][nextTableNote] = effect2[effect][1];
					mTablesControls[MusicStudio1::kTableIndex_Note][nextTableNote+1] = 0x20;
					mTablesControls[MusicStudio1::kTableIndex_Note][nextTableNote+2] = 0xff;
				}
			}
		}
		

//		OptimiseTables(true);
	}

	return true;
}
