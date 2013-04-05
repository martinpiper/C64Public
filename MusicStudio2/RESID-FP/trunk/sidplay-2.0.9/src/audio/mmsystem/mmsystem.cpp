/***************************************************************************
                          mmsystem.cpp - "Waveout for Windows" specific
                                         audio driver interface for NT, Win9x
                                         and possibly Win3.1 with Win32s
                             -------------------
    begin                : Fri Aug 11 2000
    copyright            : (C) 2000 by Jarno Paananen
    email                : jpaana@s2.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *  $Log: mmsystem.cpp,v $
 *  Revision 1.7  2002/03/04 19:07:48  s_a_white
 *  Fix C++ use of nothrow.
 *
 *  Revision 1.6  2001/12/11 19:38:13  s_a_white
 *  More GCC3 Fixes.
 *
 *  Revision 1.5  2001/11/21 00:04:36  s_a_white
 *  Buffer alignment issues fixed.
 *
 *  Revision 1.4  2001/11/16 19:32:51  s_a_white
 *  Removed warning.
 *
 *  Revision 1.3  2001/09/17 19:08:55  s_a_white
 *  Sample enconding support added.
 *
 *  Revision 1.2  2001/07/03 17:54:23  s_a_white
 *  Support for new audio interface for better compatibility.
 *
 *  Revision 1.1  2001/01/08 16:41:43  s_a_white
 *  App and Library Seperation
 *
 ***************************************************************************/

#include "mmsystem.h"
#ifdef   HAVE_MMSYSTEM

#include <stdio.h>
#include <mmreg.h>

#pragma comment(lib , "winmm.lib")

#ifdef HAVE_EXCEPTIONS
#   include <new>
#endif

int gSoftMAXBUFBLOCKS = 3;
int gSoftBufSizeDivisor = 1;

Audio_MMSystem::Audio_MMSystem ()
{
    isOpen = false;
    for ( int i = 0; i < MAXBUFBLOCKS; i++ )
    {
        blockHeaderHandles[i] = 0;
        blockHandles[i]       = 0;
        blockHeaders[i]       = NULL;
        blocks[i]             = NULL;
    }
    waveHandle = 0;
}

Audio_MMSystem::~Audio_MMSystem()
{
    close();
}

float *Audio_MMSystem::open (AudioConfig &cfg, const char *)
{
    WAVEFORMATEX  wfm;

    if (isOpen)
    {
        _errorString = "MMSYSTEM ERROR: Audio device already open.";
        return NULL;
    }
    isOpen = true;

    /* Initialise blocks */
    memset (blockHandles, 0, sizeof (blockHandles));
    memset (blockHeaders, 0, sizeof (blockHeaders));
    memset (blockHeaderHandles, 0, sizeof (blockHeaderHandles));

    // Format
    memset (&wfm, 0, sizeof(WAVEFORMATEX));
    wfm.wFormatTag      = WAVE_FORMAT_IEEE_FLOAT;
    wfm.nChannels       = cfg.channels;
    wfm.nSamplesPerSec  = cfg.frequency;
    wfm.wBitsPerSample  = 32;
    wfm.nBlockAlign     = wfm.wBitsPerSample / 8 * wfm.nChannels;
    wfm.nAvgBytesPerSec = wfm.nSamplesPerSec * wfm.nBlockAlign;
    wfm.cbSize          = 0;

    // Rev 1.3 (saw) - Calculate buffer to hold 250ms of data
    bufSize = wfm.nSamplesPerSec / 4 * wfm.nBlockAlign;

	bufSize = bufSize / gSoftBufSizeDivisor;

    cfg.bufSize = bufSize / 4;
    waveOutOpen (&waveHandle, WAVE_MAPPER, &wfm, 0, 0, 0);
    if ( !waveHandle ) {
        _errorString = "MMSYSTEM ERROR: Can't open wave out device.";
        goto Audio_MMSystem_openError;
    }

    _settings    = cfg;

    {
        /* Allocate and lock memory for all mixing blocks: */
        int i;
        for (i = 0; i < MAXBUFBLOCKS; i++ )
        {
            /* Allocate global memory for mixing block: */
            if ( (blockHandles[i] = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE,
                                                bufSize)) == NULL )
            {
                _errorString = "MMSYSTEM ERROR: Can't allocate global memory.";
                goto Audio_MMSystem_openError;
            }

            /* Lock mixing block memory: */
            if ( (blocks[i] = (float *)GlobalLock(blockHandles[i])) == NULL )
            {
                _errorString = "MMSYSTEM ERROR: Can't lock global memory.";
                goto Audio_MMSystem_openError;
            }

            /* Allocate global memory for mixing block header: */
            if ( (blockHeaderHandles[i] = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE,
                                                      sizeof(WAVEHDR))) == NULL )
            {
                _errorString = "MMSYSTEM ERROR: Can't allocate global memory.";
                goto Audio_MMSystem_openError;
            }

            /* Lock mixing block header memory: */
            WAVEHDR *header;
            if ( (header = blockHeaders[i] =
                  (WAVEHDR*)GlobalLock(blockHeaderHandles[i])) == NULL )
            {
                _errorString = "MMSYSTEM ERROR: Can't lock global memory.";
                goto Audio_MMSystem_openError;
            }

            /* Reset wave header fields: */
            memset (header, 0, sizeof (WAVEHDR));
            header->lpData         = (char*)blocks[i];
            header->dwBufferLength = bufSize;
            header->dwFlags        = WHDR_DONE; /* mark the block is done */
        }
    }

    blockNum = 0;
    _sampleBuffer = blocks[blockNum];
return _sampleBuffer;

Audio_MMSystem_openError:
    close ();
    return NULL;
}

float*Audio_MMSystem::write ()
{
    if (!isOpen)
    {
        _errorString = "MMSYSTEM ERROR: Device not open.";
        return NULL;
    }

    /* Reset wave header fields: */
    blockHeaders[blockNum]->dwFlags = 0;

    /* Prepare block header: */
    if ( waveOutPrepareHeader(waveHandle, blockHeaders[blockNum],
                              sizeof(WAVEHDR)) != MMSYSERR_NOERROR )
    {
        _errorString = "MMSYSTEM ERROR: Error in waveOutPrepareHeader.";
        return NULL;
    }

    if ( waveOutWrite(waveHandle, blockHeaders[blockNum],
                      sizeof(WAVEHDR)) != MMSYSERR_NOERROR )
    {
        _errorString = "MMSYSTEM ERROR: Error in waveOutWrite.";
        return NULL;
    }

    /* Next block, circular buffer style, and I don't like modulo. */
    blockNum++;
    blockNum %= gSoftMAXBUFBLOCKS;

    /* Wait for the next block to become free */
    while ( !(blockHeaders[blockNum]->dwFlags & WHDR_DONE) )
        Sleep(20);

    if ( waveOutUnprepareHeader(waveHandle, blockHeaders[blockNum],
                                sizeof(WAVEHDR)) != MMSYSERR_NOERROR )
    {
        _errorString = "MMSYSTEM ERROR: Error in waveOutUnprepareHeader.";
        return NULL;
    }

    _sampleBuffer = blocks[blockNum];
    return _sampleBuffer;
}

// Rev 1.2 (saw) - Changed, see AudioBase.h    
float *Audio_MMSystem::reset (void)
{
    if (!isOpen)
        return NULL;

    // Stop play and kill the current music.
    // Start new music data being added at the begining of
    // the first buffer
    if ( waveOutReset(waveHandle) != MMSYSERR_NOERROR )
    {
        _errorString = "MMSYSTEM ERROR: Error in waveOutReset.";
        return NULL;
    }
    blockNum = 0;
    _sampleBuffer = blocks[blockNum];
    return _sampleBuffer;
}

void Audio_MMSystem::close (void)
{
    if ( !isOpen )
        return;

    isOpen        = false;
    _sampleBuffer = NULL;

    /* Reset wave output device, stop playback, and mark all blocks done: */
    if ( waveHandle )
    {
        waveOutReset(waveHandle);

        /* Make sure all blocks are indeed done: */
        int doneTimeout = 500;
        int allDone;
        int i;

        for (;;) {
            allDone = 1;
            for ( i = 0; i < MAXBUFBLOCKS; i++ ) {
                if ( blockHeaders[i] && (blockHeaders[i]->dwFlags & WHDR_DONE) == 0 )
                    allDone = 0;
            }

            if ( allDone || (doneTimeout == 0) )
                break;
            doneTimeout--;
            Sleep(20);
        }

        /* Unprepare all mixing blocks, unlock and deallocate
           all mixing blocks and mixing block headers: */
        for ( i = 0; i < MAXBUFBLOCKS; i++ )
        {
            if ( blockHeaders[i] )
                waveOutUnprepareHeader(waveHandle, blockHeaders[i], sizeof(WAVEHDR));

            if ( blockHeaderHandles[i] )
            {
                GlobalUnlock(blockHeaderHandles[i]);
                GlobalFree(blockHeaderHandles[i]);
            }
            if ( blockHandles[i] )
            {
                GlobalUnlock(blockHandles[i]);
                GlobalFree(blockHandles[i]);
            }
        }

        /* Close wave output device: */
        waveOutClose(waveHandle);
    }
}

#endif // HAVE_MMSYSTEM
