// MusicStudioDocGoatTracker.cpp : implementation of the CMusicStudioDoc class for loading Goat tracker songs
//

#include "stdafx.h"
#include "MusicStudio.h"

#include "MusicStudioDoc.h"
#include "MusicStudioView2.h"
#include "ChildFrm.h"
#include "ImportMIDI.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static int sLenLeft = 0;

static bool sDataPushed = false;
unsigned char sThePushedData = 0;
static unsigned char ReadUChar(CArchive &ar)
{
	if ( sDataPushed )
	{
		sDataPushed = false;
		return sThePushedData;
	}
	unsigned char ret = 0;

	ar.Read( &ret , 1 );

	sLenLeft--;

	return ret;
}

static int ReadMSBInt(CArchive &ar)
{
	int ret = 0;

	ret = ((int)ReadUChar( ar )) << 24;
	ret |= ((int)ReadUChar( ar )) << 16;
	ret |= ((int)ReadUChar( ar )) << 8;
	ret |= ((int)ReadUChar( ar ));

	sLenLeft -= 4;

	return ret;
}

static short ReadMSBShort(CArchive &ar)
{
	short ret = 0;

	ret = ((int)ReadUChar( ar )) << 8;
	ret |= ((int)ReadUChar( ar ));

	sLenLeft -= 2;

	return ret;
}

static int ReadVTime(CArchive &ar)
{
	int ret = 0;
	unsigned char theByte;

	do
	{
		theByte = ReadUChar( ar );
		ret = (ret << 7) | ( theByte & 0x7f );

	} while ( theByte & 0x80 );

	return ret;
}

static bool ReportParseError( void )
{
	MessageBoxA( 0 , "This MIDI file has an expected format, please send it with a bug report to martin.piper@gmail.com" , "Error!" , MB_OK );
	return false;
}

bool CMusicStudioDoc::LoadMIDIFile(CArchive &ar)
{
	if (!ar.IsLoading())
	{
		return false;
	}

	OnNewDocument();

	mGenericInfo = CString(_T("Originaly converted from the MIDI file: ")) + ar.GetFile()->GetFileName() + _T("\x0d\x0a");

	// Track data
	ZeroMemory(mTracks,sizeof(mTracks));
	int i;

	mEnvelopeNames[1] = _T("A note");
	mEnvelopes[1].mAttackDecay = 0x22;
	mEnvelopes[1].mSustainRelease = 0xf8;
	mEnvelopes[1].mAllowVoiceEffects = true;
	mEnvelopes[1].mTableWave = 0x01;
	mEnvelopes[1].mTableNote = 0x00;
	mEnvelopes[1].mTablePulse = 0x00;
	mEnvelopes[1].mTableFilter = 0x00;
	mEnvelopes[1].mActiveTableWave = true;
	mEnvelopes[1].mActiveTableNote = false;
	mEnvelopes[1].mActiveTablePulse = false;
	mEnvelopes[1].mActiveTableFilter = false;
	mTablesControls[0][1] = 0x11;
	mTablesControls[0][2] = 0xff;

	CImportMIDI dlg;
	// Some sensible default values
	dlg.mTimeMultiplier = 10;
	dlg.mBlockLength = 256;
	dlg.mReleaseNoteAfterHalfDuration = FALSE;
	INT_PTR ret = dlg.DoModal();

	if (ret != IDOK)
	{
		return false;
	}

	int timeMultiplier = dlg.mTimeMultiplier;
	int blockLength = dlg.mBlockLength;
	bool releaseNoteAfterHalfDuration = dlg.mReleaseNoteAfterHalfDuration?true:false;

	// MPi: TODO: channelToTrackMap should map channels to the tracks, but at the moment it just flags if a channel should be converted to track 1 ( not track 2 or 3 )
	int channelToTrackMap[16];
	channelToTrackMap[0] = dlg.mChannelMap0;
	channelToTrackMap[1] = dlg.mChannelMap1;
	channelToTrackMap[2] = dlg.mChannelMap2;
	channelToTrackMap[3] = dlg.mChannelMap3;
	channelToTrackMap[4] = dlg.mChannelMap4;
	channelToTrackMap[5] = dlg.mChannelMap5;
	channelToTrackMap[6] = dlg.mChannelMap6;
	channelToTrackMap[7] = dlg.mChannelMap7;
	channelToTrackMap[8] = dlg.mChannelMap8;
	channelToTrackMap[9] = dlg.mChannelMap9;
	channelToTrackMap[10] = dlg.mChannelMap10;
	channelToTrackMap[11] = dlg.mChannelMap11;
	channelToTrackMap[12] = dlg.mChannelMap12;
	channelToTrackMap[13] = dlg.mChannelMap13;
	channelToTrackMap[14] = dlg.mChannelMap14;
	channelToTrackMap[15] = dlg.mChannelMap15;

	CString temp;
	temp.Format( _T("Conversion options:\x0d\x0aTime multiplier %d Block Length %d\x0d\x0a") , timeMultiplier , blockLength );
	mGenericInfo += temp;
	if ( releaseNoteAfterHalfDuration )
	{
		mGenericInfo += _T("With release note after half duration.\x0d\x0a");
	}
	for ( i = 0 ; i < 15 ; i++ )
	{
		if ( channelToTrackMap[i] )
		{
			temp.Format( _T("MIDI Channel %d to track %d\x0d\x0a") , i , channelToTrackMap[i] );
			mGenericInfo += temp;
		}
	}

	for (i=0;i<3;i++)
	{
		// Setup something by default
		mTracks[i][0] = MusicStudio1::kMusicPlayer_StopTrack;
	}

	int len = ReadMSBInt( ar );
	short format = ReadMSBShort( ar );
	short numTrackBlocks = ReadMSBShort( ar );
	short timeDivision = ReadMSBShort( ar );

	int channel;
	for ( channel = 0 ; channel < numTrackBlocks ; channel++ )
	{
		LONGLONG theTime = 0;
		LONGLONG lastNoteTime = 0;

		int header = ReadMSBInt( ar );
		len = ReadMSBInt( ar );

		if ( header != 0x4d54726b )
		{
			return ReportParseError();
		}

		sLenLeft = len;

		// Skip channels we don't want to map to a track
		if ( !channelToTrackMap[ channel ] )
		{
			while ( sLenLeft > 0 )
			{
				unsigned char temp = ReadUChar( ar );
			}
			continue;
		}

		unsigned char lastEvent = 0;
		while ( sLenLeft > 0 )
		{
			int deltaTime = ReadVTime( ar );
			theTime += deltaTime;
			unsigned char theEvent = ReadUChar( ar );

			// Running mode check
			if ( theEvent <= 127 )
			{
				sDataPushed = true;
				sThePushedData = theEvent;
				theEvent = lastEvent;
			}
			else
			{
				lastEvent = theEvent;
			}

			if ( theEvent == 0xff )
			{
				// Meta event
				unsigned char theCommand = ReadUChar( ar );
				unsigned char commandLength = ReadUChar( ar );
				while ( commandLength != 0 )
				{
					// Just skip the data
					unsigned char temp = ReadUChar( ar );
					commandLength--;
				}
			}
			else //<! if ( theEvent == 0xff )
			{
				LONGLONG realTime = ( theTime * timeMultiplier ) / timeDivision; // 50 frames per second

				int blockPos = (int) (realTime % blockLength);
				int theBlock = (int) (realTime / blockLength);
				if ( theBlock >= MusicStudio1::MusicFile::kMaxBlocks )
				{
					int i = 0;
//					return ReportParseError();
				}

				if ( ( theBlock >= 0 ) && ( theBlock < MusicStudio1::MusicFile::kMaxBlocks ) )
				{
					mBlockLastEditedAsTracker[ theBlock ] = true;
					if ( blockLength > mBlockTrackerLengths[ theBlock ] )
					{
						mBlockTrackerLengths[ theBlock ] = blockLength;
						mBlockEditTrackerMaxCalcedRow[ theBlock ] = blockLength;
					}
					mTracks[0][ theBlock ] = theBlock;
					mTracks[0][ theBlock + 1] = MusicStudio1::kMusicPlayer_StopTrack;
				}

				switch( theEvent & 0xf0 )
				{
					case 0x80:
					{
						// Note off
						unsigned char noteNumber = ReadUChar( ar );
						unsigned char velocity = ReadUChar( ar );

						if ( ( theBlock >= 0 ) && ( theBlock < MusicStudio1::MusicFile::kMaxBlocks ) )
						{
							mBlockTrackerRows[ theBlock ][ blockPos ][0] = "===";
							lastNoteTime = 0;
						}
						break;
					}

					case 0x90:
					{
						// Note on
						unsigned char noteNumber = ReadUChar( ar );
						unsigned char velocity = ReadUChar( ar );

						// Any previous note to release?
						if ( releaseNoteAfterHalfDuration && ( lastNoteTime > 0 ) )
						{
							LONGLONG theDiff = theTime - lastNoteTime;

							if ( theDiff > 0 )
							{
								LONGLONG realTime = ( ( lastNoteTime + ( theDiff / 2 ) ) * timeMultiplier ) / timeDivision; // 50 frames per second

								int blockPos = (int) (realTime % blockLength);
								int theBlock = (int) (realTime / blockLength);
								if ( ( theBlock >= 0 ) && ( theBlock < MusicStudio1::MusicFile::kMaxBlocks ) )
								{
									// Don't put a release if the position already has something
									if ( mBlockTrackerRows[ theBlock ][ blockPos ][0].IsEmpty() )
									{
										mBlockTrackerRows[ theBlock ][ blockPos ][0] = "===";
									}
								}
							}

							lastNoteTime = 0;
						}

						if ( ( theBlock >= 0 ) && ( theBlock < MusicStudio1::MusicFile::kMaxBlocks ) )
						{
							mBlockTrackerRows[ theBlock ][ blockPos ][0] = MusicStudio1::BlockEntry::GetNoteFromNumber(noteNumber).c_str();
							mBlockTrackerRows[ theBlock ][ blockPos ][1] = "1";	// Always envelope 1

							lastNoteTime = theTime;
						}

						break;
					}

					case 0xa0:
					{
						// After touch
						unsigned char noteNumber = ReadUChar( ar );
						unsigned char velocity = ReadUChar( ar );
						break;
					}

					case 0xb0:
					{
						// Controller change
						unsigned char controller = ReadUChar( ar );
						unsigned char newValue = ReadUChar( ar );
						break;
					}

					case 0xc0:
					{
						// Patch change
						unsigned char programNumber = ReadUChar( ar );
						break;
					}

					case 0xd0:
					{
						// Channel after touch
						unsigned char channelNumber = ReadUChar( ar );
						break;
					}

					case 0xe0:
					{
						// Pitch wheel change
						short wheelChange = ReadMSBShort( ar );
						break;
					}

					default:
					{
						return ReportParseError();
						break;
					}
				}
			} //< if ( theEvent == 0xff )
		} //< while ( sLenLeft > 0 )
	}

#if 0
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
#endif

	return true;
}
