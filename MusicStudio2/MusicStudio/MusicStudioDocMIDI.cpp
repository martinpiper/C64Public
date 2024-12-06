// MusicStudioDocMIDI.cpp : implementation of the CMusicStudioDoc class for loading MIDI files
//

#include "stdafx.h"
#include "MusicStudio.h"

#include "MusicStudioDoc.h"
#include "MusicStudioView2.h"
#include "ChildFrm.h"
#include "ImportMIDI.h"
#include "RNPlatform/Inc/MessageHelper.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static int sLenLeft = 0;

static bool sDataPushed = false;
unsigned char sThePushedData = 0;
static unsigned char ReadUChar(RNReplicaNet::DynamicMessageHelper &theFile)
{
	if ( sDataPushed )
	{
		sDataPushed = false;
		return sThePushedData;
	}
	unsigned char ret = 0;

	theFile >> ret;

	sLenLeft--;

	return ret;
}

static int ReadMSBInt(RNReplicaNet::DynamicMessageHelper &theFile)
{
	int ret = 0;

	ret = ((int)ReadUChar( theFile )) << 24;
	ret |= ((int)ReadUChar( theFile )) << 16;
	ret |= ((int)ReadUChar( theFile )) << 8;
	ret |= ((int)ReadUChar( theFile ));

	sLenLeft -= 4;

	return ret;
}

static short ReadMSBShort(RNReplicaNet::DynamicMessageHelper &theFile)
{
	short ret = 0;

	ret = ((int)ReadUChar( theFile )) << 8;
	ret |= ((int)ReadUChar( theFile ));

	sLenLeft -= 2;

	return ret;
}

static int ReadVTime(RNReplicaNet::DynamicMessageHelper &theFile)
{
	int ret = 0;
	unsigned char theByte;

	do
	{
		theByte = ReadUChar( theFile );
		ret = (ret << 7) | ( theByte & 0x7f );

	} while ( theByte & 0x80 );

	return ret;
}

static bool ReportParseError( void )
{
	MessageBoxA( 0 , "This MIDI file has an unexpected format, please send it with a bug report to martin.piper@gmail.com" , "Error!" , MB_OK );
	return false;
}

bool CMusicStudioDoc::LoadMIDIFile(CArchive &ar)
{
	if (!ar.IsLoading())
	{
		return false;
	}

	OnNewDocument();

	mTrackerBlockEditState = true;

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

	char buffer[1024];
	RNReplicaNet::DynamicMessageHelper theFile;
	UINT gotBytes = 0;
	do 
	{
		gotBytes = ar.Read( buffer , sizeof( buffer ) );
		if ( gotBytes )
		{
			theFile.AddData( buffer , gotBytes );
		}
	} while ( gotBytes );

	theFile.SetSize( 0 );

	int len = ReadMSBInt( theFile );
	short format = ReadMSBShort( theFile );
	short numTrackBlocks = ReadMSBShort( theFile );
	short timeDivision = ReadMSBShort( theFile );
	int channel;
	int channelSizes[16];
	memset( channelSizes , 0 , sizeof( channelSizes ) );

	for ( channel = 0 ; channel < numTrackBlocks ; channel++ )
	{
		LONGLONG theTime = 0;
		LONGLONG lastNoteTime = 0;

		int header = ReadMSBInt( theFile );
		len = ReadMSBInt( theFile );

		if ( header != 0x4d54726b )
		{
			return ReportParseError();
		}

		sLenLeft = len;

		// Skip channels we don't want to map to a track
		while ( sLenLeft > 0 )
		{
			unsigned char temp = ReadUChar( theFile );
			channelSizes[ channel ]++;
		}
	}

	// Some sensible default values
	CImportMIDI dlg;

	dlg.mBytesUsed0 = channelSizes[0];
	dlg.mBytesUsed1 = channelSizes[1];
	dlg.mBytesUsed2 = channelSizes[2];
	dlg.mBytesUsed3 = channelSizes[3];
	dlg.mBytesUsed4 = channelSizes[4];
	dlg.mBytesUsed5 = channelSizes[5];
	dlg.mBytesUsed6 = channelSizes[6];
	dlg.mBytesUsed7 = channelSizes[7];
	dlg.mBytesUsed8 = channelSizes[8];
	dlg.mBytesUsed9 = channelSizes[9];
	dlg.mBytesUsed10 = channelSizes[10];
	dlg.mBytesUsed11 = channelSizes[11];
	dlg.mBytesUsed12 = channelSizes[12];
	dlg.mBytesUsed13 = channelSizes[13];
	dlg.mBytesUsed14 = channelSizes[14];
	dlg.mBytesUsed15 = channelSizes[15];

	dlg.mTimeMultiplier = 10;
	dlg.mBlockLength = 256;
	dlg.mReleaseNoteAfterHalfDuration = FALSE;
	dlg.mIgnoreNoteOffEvents = FALSE;
	dlg.mAddHRDCommand = FALSE;
	INT_PTR ret = dlg.DoModal();

	if (ret != IDOK)
	{
		return false;
	}

	int timeMultiplier = dlg.mTimeMultiplier;
	int blockLength = dlg.mBlockLength;
	bool releaseNoteAfterHalfDuration = dlg.mReleaseNoteAfterHalfDuration?true:false;
	bool ignoreNoteOffEvents = dlg.mIgnoreNoteOffEvents?true:false;
	bool addHRDCommand = dlg.mAddHRDCommand?true:false;

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

	int blockOffset = 0;
	int maxBlockSoFar = 0;

	for (int realTrack = 1 ; realTrack <= 3 ; realTrack++)
	{
		theFile.SetSize( 0 );

		len = ReadMSBInt( theFile );
		format = ReadMSBShort( theFile );
		numTrackBlocks = ReadMSBShort( theFile );
		timeDivision = ReadMSBShort( theFile );

		for ( channel = 0 ; channel < numTrackBlocks ; channel++ )
		{
			LONGLONG theTime = 0;
			LONGLONG lastNoteTime = 0;

			int header = ReadMSBInt( theFile );
			len = ReadMSBInt( theFile );

			if ( header != 0x4d54726b )
			{
				return ReportParseError();
			}

			sLenLeft = len;

			// Skip channels we don't want to map to a track
			if ( channelToTrackMap[ channel ] != realTrack )
			{
				while ( sLenLeft > 0 )
				{
					unsigned char temp = ReadUChar( theFile );
				}
				continue;
			}

			unsigned char lastEvent = 0;
			while ( sLenLeft > 0 )
			{
				int deltaTime = ReadVTime( theFile );
				theTime += deltaTime;
				unsigned char theEvent = ReadUChar( theFile );

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
					unsigned char theCommand = ReadUChar( theFile );
					unsigned char commandLength = ReadUChar( theFile );
					while ( commandLength != 0 )
					{
						// Just skip the data
						unsigned char temp = ReadUChar( theFile );
						commandLength--;
					}
				}
				else //<! if ( theEvent == 0xff )
				{
					LONGLONG realTime = ( theTime * timeMultiplier ) / timeDivision; // 50 frames per second

					int blockPos = (int) (realTime % blockLength);
					int theBlockTime = (int) (realTime / blockLength);
					int theBlock = blockOffset + theBlockTime;
					maxBlockSoFar = max(maxBlockSoFar , theBlock);

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
						mTracks[realTrack-1][ theBlockTime ] = theBlock;
						mTracks[realTrack-1][ theBlockTime + 1] = MusicStudio1::kMusicPlayer_StopTrack;
					}

					switch( theEvent & 0xf0 )
					{
						case 0x80:
						{
							// Note off
							unsigned char noteNumber = ReadUChar( theFile );
							unsigned char velocity = ReadUChar( theFile );

							if ( !ignoreNoteOffEvents && ( theBlock >= 0 ) && ( theBlock < MusicStudio1::MusicFile::kMaxBlocks ) )
							{
								mBlockTrackerRows[ theBlock ][ blockPos ][0] = "===";
								lastNoteTime = 0;
							}
							break;
						}

						case 0x90:
						{
							// Note on
							unsigned char noteNumber = ReadUChar( theFile );
							unsigned char velocity = ReadUChar( theFile );

							// Any previous note to release?
							if ( releaseNoteAfterHalfDuration && ( lastNoteTime > 0 ) )
							{
								LONGLONG theDiff = theTime - lastNoteTime;

								if ( theDiff > 0 )
								{
									LONGLONG realTime = ( ( lastNoteTime + ( theDiff / 2 ) ) * timeMultiplier ) / timeDivision; // 50 frames per second

									int blockPos = (int) (realTime % blockLength);
									int theBlock = blockOffset + (int) (realTime / blockLength);
									maxBlockSoFar = max(maxBlockSoFar , theBlock);

									if (addHRDCommand)
									{
										mBlockTrackerRows[ theBlock ][ 0 ][2] = "HRD";
									}

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
							unsigned char noteNumber = ReadUChar( theFile );
							unsigned char velocity = ReadUChar( theFile );
							break;
						}

						case 0xb0:
						{
							// Controller change
							unsigned char controller = ReadUChar( theFile );
							unsigned char newValue = ReadUChar( theFile );
							break;
						}

						case 0xc0:
						{
							// Patch change
							unsigned char programNumber = ReadUChar( theFile );
							break;
						}

						case 0xd0:
						{
							// Channel after touch
							unsigned char channelNumber = ReadUChar( theFile );
							break;
						}

						case 0xe0:
						{
							// Pitch wheel change
							short wheelChange = ReadMSBShort( theFile );
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

		blockOffset = maxBlockSoFar + 1;
	} //< for (int realTrack = 1 ; realTrack <= 3 ; realTrack++)

	return true;
}
