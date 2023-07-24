// MusicStudioDocReaper.cpp : implementation of the CMusicStudioDoc class for loading Reaper files
//

#include "stdafx.h"
#include "MusicStudio.h"

#include "MusicStudioDoc.h"
#include "MusicStudioView2.h"
#include "ChildFrm.h"
#include "ImportReaper.h"
#include "RNPlatform/Inc/MessageHelper.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static bool ReportParseError( const char *extraMessage = 0 )
{
	CString message = L"This Reaper file has an unexpected format, please send it with a bug report to martin.piper@gmail.com\n";
	if (extraMessage)
	{
		message += extraMessage;
	}
	MessageBox( 0 , message , L"Error!" , MB_OK );
	return false;
}

#define MCHECKLIMITS() \
if (block >= (MusicStudio1::MusicFile::kMaxBlocks - 1))							\
{																				\
	return ReportParseError("Too many blocks in this music.");					\
}																				\
if (trackPos >= (MusicStudio1::MusicFile::kMaxTrackLength - 3))					\
{																				\
	return ReportParseError("Too many used track positions in this music.");	\
}																				\


void CMusicStudioDoc::BlockInit(int &block, int outBlockTempo, int track, int &trackPos, LONGLONG &theBlockLengthProcessed, int &numberOfEventsInBlock, bool checkForDuplicates)
{
	bool blockSame = false;
	// Check for any duplicates
	if (block > 0 && checkForDuplicates)
	{
		for (int blk = 0 ; !blockSame && blk < block ; blk++)
		{
			if (mBlockTrackerLengths[blk] != mBlockTrackerLengths[block])
			{
				continue;
			}
			bool same = true;
			for (int i = 0 ; same && i < mBlockTrackerLengths[blk] ; i++)
			{
				if (mBlockTrackerRows[blk][i][0] != mBlockTrackerRows[block][i][0])
				{
					same = false;
					break;
				}
				if (mBlockTrackerRows[blk][i][1] != mBlockTrackerRows[block][i][1])
				{
					same = false;
					break;
				}
				if (mBlockTrackerRows[blk][i][2] != mBlockTrackerRows[block][i][2])
				{
					same = false;
					break;
				}
			}
			if (same)
			{
				blockSame = true;
				break;
			}
		}
	}
	if (!blockSame)
	{
		block++;
	}
	mBlockLastEditedAsTracker[block] = true;
	mBlockTrackerTempos[block] = outBlockTempo;
	mTracks[track][trackPos] = block;
	mTracks[track][trackPos+1] = MusicStudio1::kMusicPlayer_StopTrack;
	mBlockTrackerRows[block][0][2] = "HRD";
	theBlockLengthProcessed = 0;
	numberOfEventsInBlock = 0;
	if (!blockSame)
	{
		trackPos++;
	}
}


bool CMusicStudioDoc::LoadReaperFile(CArchive &ar)
{
	if (!ar.IsLoading())
	{
		return false;
	}

	OnNewDocument();

	mGenericInfo = CString(_T("Originaly converted from the Reaper file: ")) + ar.GetFile()->GetFileName() + _T("\x0d\x0a");

	CString filename = ar.GetFile()->GetFilePath();

	CStdioFile file(filename, CFile::modeRead | CFile::shareDenyNone);
	CString strLine; 
	CStringArray arrLines;
	while(file.ReadString(strLine)) 
		arrLines.Add(strLine); 

	int i;

	// Track data
	ZeroMemory(mTracks,sizeof(mTracks));

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

	int outBlockTempo = 1;
	int kC64FramesPerSecond = 50;
	CString temp;
	temp.Format( _T("Conversion options:\x0d\x0aBlock tempo %d\x0d\x0a") , outBlockTempo );
	mGenericInfo += temp;

	for (i=0;i<3;i++)
	{
		// Setup something by default
		mTracks[i][0] = MusicStudio1::kMusicPlayer_StopTrack;
	}

	bool includeEmptyTime = false;

	int track = -1;
	int trackPos = 0;
	int block = -1;
	int tempo = 120;	// Only used when converting block ticks to time
	int tempoMultiplier = 16;
	bool processingItem = false;
	int startOfMIDIBlock = 0;
	bool waitFirstMIDIEvent = true;
	bool wasNoteOn = false;
	int numberOfEventsInBlock = 0;
	int beatAdjustment = 0;
	// Time in ticks
	LONGLONG theTime = 0;
	LONGLONG lastNoteTime = 0;
	LONGLONG theBlockLength = 0;
	LONGLONG theBlockLengthProcessed = 0;
	LONGLONG theSourceOffset = 0;
	bool removeMutedItems = true;
	bool removeTracksWithoutItems = true;
	bool processTrack1 = true;
	bool processTrack2 = true;
	bool processTrack3 = true;
	bool checkForDuplicates = true;
	
	CImportReaper dlg;

	INT_PTR ret = dlg.DoModal();

	if (ret != IDOK)
	{
		return false;
	}

	removeMutedItems = dlg.mRemoveMutedItems?true:false;
	removeTracksWithoutItems = dlg.mRemoveTracksWithoutItems?true:false;
	processTrack1 = dlg.mProcessTrack1?true:false;
	processTrack2 = dlg.mProcessTrack2?true:false;
	processTrack3 = dlg.mProcessTrack3?true:false;
	tempo = dlg.mReaperTempo;
	tempoMultiplier = dlg.mTempoMultiplier;
	includeEmptyTime = dlg.mIncludeEmptyTime?true:false;
	dlg.mBeatAdjustment?beatAdjustment=1:beatAdjustment=0;
	checkForDuplicates = dlg.mRemoveDuplicateBlocks?true:false;
	kC64FramesPerSecond = dlg.mC64FramesPerSecond;
	outBlockTempo = dlg.mOutputBlockTempo;
	
	
	// After the dialog customised values
	int blockTicksPerSecond = tempo * tempoMultiplier; // (1920)

	int level = 0;
	int startItem = 0;
	if (removeMutedItems)
	{
		// Remove muted items
		bool isMuted = false;
		for (i=0 ; i < arrLines.GetCount() ; i++)
		{
			CString currentLine = arrLines.GetAt(i);
			currentLine.Trim();
			if (currentLine.Find(L"<ITEM") == 0)
			{
				level = 1;
				startItem = i;
				isMuted = false;
				continue;
			}
			if (startItem > 0 && currentLine.Find(L"<") == 0)
			{
				level++;
				continue;
			}
			if (startItem > 0 && currentLine.Find(L">") == 0)
			{
				level--;
				if (level <= 0)
				{
					if (isMuted)
					{
						arrLines.RemoveAt(startItem , i-startItem);
					}
					startItem = 0;
				}
				continue;
			}
			if (startItem > 0 && currentLine.Find(L"MUTE 1") == 0)
			{
				isMuted = true;
				continue;
			}
		}
	}

	if (removeTracksWithoutItems)
	{
		// Remove any tracks without items
		startItem = 0;
		int countItems = 0;
		for (i=0 ; i < arrLines.GetCount() ; i++)
		{
			CString currentLine = arrLines.GetAt(i);
			currentLine.Trim();
			if (currentLine.Find(L"<TRACK ") == 0)
			{
				level = 1;
				countItems = 0;
				startItem = i;
				continue;
			}
			if (startItem > 0 && currentLine.Find(L"<ITEM") == 0)
			{
				countItems++;
			}
			if (startItem > 0 && currentLine.Find(L"<") == 0)
			{
				level++;
				continue;
			}
			if (startItem > 0 && currentLine.Find(L">") == 0)
			{
				level--;
				if (level <= 0)
				{
					if (countItems <= 0)
					{
						arrLines.RemoveAt(startItem , i-startItem);
					}
					startItem = 0;
				}
				continue;
			}
		}
	}

	bool allowCurrentTrack = true;
	// Process data
	for (i=0 ; i < arrLines.GetCount() ; i++)
	{
		CString currentLine = arrLines.GetAt(i);
		currentLine.Trim();

		if (processingItem)
		{
			if (currentLine.Find(L"POSITION ") == 0)
			{
				int start = 0;
				CString tok = currentLine.Tokenize(L" ",start);
				tok = currentLine.Tokenize(L" ",start);
				double thePosition = _ttof(tok);

				theTime = (LONGLONG) (thePosition * blockTicksPerSecond);
				LONGLONG timeDelta = (theTime - lastNoteTime);
				if ( timeDelta > 0)
				{
					if (allowCurrentTrack)
					{
						if (includeEmptyTime)
						{
							// Need a blank block of this new position
							MCHECKLIMITS();
							BlockInit(block, outBlockTempo, track, trackPos, theBlockLengthProcessed, numberOfEventsInBlock, checkForDuplicates);
							int wantLength = (int)((timeDelta * kC64FramesPerSecond) / (outBlockTempo * blockTicksPerSecond));
							// Split any extra long blocks
							while (wantLength > kMaxInternalTrackerRows)
							{
								mBlockTrackerLengths[block] = kMaxInternalTrackerRows;
								mBlockEditTrackerMaxCalcedRow[block] = kMaxInternalTrackerRows;
								MCHECKLIMITS();
								BlockInit(block, outBlockTempo, track, trackPos, theBlockLengthProcessed, numberOfEventsInBlock, checkForDuplicates);
								wantLength -= kMaxInternalTrackerRows;
							}
							mBlockTrackerLengths[block] = wantLength;
							mBlockEditTrackerMaxCalcedRow[block] = wantLength;
						}
					}

					lastNoteTime = theTime;
				}
				continue;
			}

			if (currentLine.Find(L"LENGTH ") == 0)
			{
				int start = 0;
				CString tok = currentLine.Tokenize(L" ",start);
				tok = currentLine.Tokenize(L" ",start);
				double theLength = _ttof(tok);

				theBlockLength = (LONGLONG) (theLength * blockTicksPerSecond);
				continue;
			}

			if (currentLine.Find(L"BEAT ") == 0)
			{
				int start = 0;
				CString tok = currentLine.Tokenize(L" ",start);
				tok = currentLine.Tokenize(L" ",start);

				beatAdjustment = _ttoi(tok);
				continue;
			}

			if (currentLine.Find(L"<SOURCE MIDI") == 0)
			{
				startOfMIDIBlock = i + 1;
				waitFirstMIDIEvent = true;

				MCHECKLIMITS();
				if (allowCurrentTrack)
				{
					BlockInit(block, outBlockTempo, track, trackPos, theBlockLengthProcessed, numberOfEventsInBlock, checkForDuplicates);
				}

				continue;
			}

			if (currentLine.Find(L"SOFFS ") == 0)
			{
				int start = 0;
				CString tok = currentLine.Tokenize(L" ",start);
				tok = currentLine.Tokenize(L" ",start);	// Ignore the offset time?
				tok = currentLine.Tokenize(L" ",start);	// This value seems to fit the observed data in the editor better?
				double thePosition = _ttof(tok);
				theSourceOffset = (LONGLONG) (thePosition * blockTicksPerSecond);// / 2;	// Why divide 2? It fitted the observed data in "one block.RPP"
			}

			if ((startOfMIDIBlock > 0) && (theBlockLengthProcessed >= theBlockLength))
			{
				// End of the block encountered during block processing so exit now
				processingItem = false;
				continue;
			}


			if (currentLine.Find(L"e ") == 0 || currentLine.Find(L"E ") == 0)
			{
				if (allowCurrentTrack)
				{
					int dbg = 0;
				}
				int start = 0;
				CString tok = currentLine.Tokenize(L" ",start);
				tok = currentLine.Tokenize(L" ",start);
				int thisEventTicks = _ttoi(tok);
				thisEventTicks /= (beatAdjustment+1);	// Bodge fix for "one block.RPP"
				if (waitFirstMIDIEvent)
				{
					thisEventTicks -= (int) theSourceOffset;
					if (thisEventTicks < 0)
					{
						thisEventTicks = 0;
					}
					waitFirstMIDIEvent = false;
				}
				LONGLONG anyExtraDelta = (theBlockLengthProcessed + thisEventTicks) - theBlockLength;
				if (anyExtraDelta > 0)
				{
					thisEventTicks -= (int) anyExtraDelta;
				}
				int potentialBlockLength = (int)(((theBlockLengthProcessed + thisEventTicks) * kC64FramesPerSecond) / (outBlockTempo * blockTicksPerSecond));
				if (potentialBlockLength >= kMaxInternalTrackerRows)
				{
					bool makeNewBlock = true;
					if (!includeEmptyTime)
					{
						bool anyNotes = false;
						if (allowCurrentTrack)
						{
							for (int j = 0 ; !anyNotes && j < kMaxInternalTrackerRows ; j++)
							{
								if (!mBlockTrackerRows[block][j][0].IsEmpty())
								{
									anyNotes = true;
									break;
								}
							}
						}
						if (!anyNotes)
						{
							makeNewBlock = false;
						}
					}
					// Need to reduce the remaining block length by how events we've processed already
					theBlockLength -= theBlockLengthProcessed;
					if (makeNewBlock)
					{
						// Going to need a new block
						MCHECKLIMITS();
						if (allowCurrentTrack)
						{
							BlockInit(block, outBlockTempo, track, trackPos, theBlockLengthProcessed, numberOfEventsInBlock, checkForDuplicates);
						}
					}
					else
					{
						theBlockLengthProcessed = 0;
						numberOfEventsInBlock = 0;
					}
				}

				lastNoteTime += thisEventTicks;
				theBlockLengthProcessed += thisEventTicks;
				numberOfEventsInBlock++;

				if (allowCurrentTrack)
				{
					mBlockTrackerRows[block][0][2] = "HRD";
				}

				// Update the entire processed block length for the events it holds
				int blockRow = (int)((theBlockLengthProcessed * kC64FramesPerSecond) / (outBlockTempo * blockTicksPerSecond));
				mBlockTrackerLengths[block] = blockRow;
				mBlockEditTrackerMaxCalcedRow[block] = mBlockTrackerLengths[block];

				CString command = currentLine.Tokenize(L" ",start);

				if (command == "90")
				{
					// Note on
					tok = currentLine.Tokenize(L" ",start);
					int theNum = 0;
					_stscanf(tok,_T("%x"),&theNum);

					tok = currentLine.Tokenize(L" ",start);

					// Check for the release...
					if (tok == "00")
					{
						if (wasNoteOn)
						{
							if (allowCurrentTrack)
							{
								mBlockTrackerRows[ block ][ blockRow ][0] = "===";
							}
							wasNoteOn = false;
						}
					}
					else
					{
						if (allowCurrentTrack)
						{
							mBlockTrackerRows[ block ][ blockRow ][0] = MusicStudio1::BlockEntry::GetNoteFromNumber(theNum - 12).c_str();
							mBlockTrackerRows[ block ][ blockRow ][1] = "1";	// Always envelope 1
						}
						wasNoteOn = true;
					}
				}
				else if (command == "80")
				{
					if (wasNoteOn)
					{
						if (allowCurrentTrack)
						{
							mBlockTrackerRows[ block ][ blockRow ][0] = "===";
						}
						wasNoteOn = false;
					}
				}
				continue;
			}

			// This seems to be after the end of MIDI events...
			if (currentLine.Find(L"CHASE_CC_TAKEOFFS ") == 0)
			{
				if (theBlockLengthProcessed < theBlockLength)
				{
					// Loop back again to fill any remaining length for this block
					// Note: Try to handle full and partial loops of output block data
					i = startOfMIDIBlock;
					waitFirstMIDIEvent = true;
					continue;
				}

				// End of the block
				processingItem = false;
				continue;
			}

			continue;
		}

		if (currentLine.Find(L"TEMPO ") == 0)
		{
			int start = 0;
			CString tok = currentLine.Tokenize(L" ",start);
			tok = currentLine.Tokenize(L" ",start);
			tempo = _ttoi(tok);
			blockTicksPerSecond = tempo * tempoMultiplier;
			continue;
		}

		if (currentLine.Find(L"<TRACK") == 0)
		{
			track++;
			trackPos = 0;
			theTime = 0;
			lastNoteTime = 0;
			processingItem = false;
			if (track > 2)
			{
				return ReportParseError("Too many track blocks found in the file.");
			}
			allowCurrentTrack = true;
			if (track == 0 && !processTrack1)
			{
				allowCurrentTrack = false;
			}
			if (track == 1 && !processTrack2)
			{
				allowCurrentTrack = false;
			}
			if (track == 2 && !processTrack3)
			{
				allowCurrentTrack = false;
			}
			continue;
		}

		if (currentLine.Find(L"<ITEM") == 0)
		{
			processingItem = true;
			startOfMIDIBlock = 0;
			theSourceOffset = 0;
			beatAdjustment = 0;
			wasNoteOn = false;
			continue;
		}
	}


	if (track < 0)
	{
		return ReportParseError("No tracks found.");
	}
	if (block < 0)
	{
		return ReportParseError("No blocks found.");
	}

	return true;
}
