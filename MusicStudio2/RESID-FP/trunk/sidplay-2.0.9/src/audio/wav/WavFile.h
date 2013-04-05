/*
 * A basic WAV output file type - Interface.
 * Initial implementation by Michael Schwendt <mschwendt@yahoo.com>
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/***************************************************************************
 *  $Log: WavFile.h,v $
 *  Revision 1.6  2004/02/26 18:19:52  s_a_white
 *  Updates for VC7 (use real libstdc++ headers instead of draft ones).
 *
 *  Revision 1.5  2002/01/10 18:57:00  s_a_white
 *  Interface changes and fixes for bigendian machines.
 *
 *  Revision 1.4  2001/11/16 19:33:27  s_a_white
 *  Removed old compatibility open.
 *
 *  Revision 1.3  2001/10/30 23:35:35  s_a_white
 *  Added pause support.
 *
 *  Revision 1.2  2001/07/03 17:54:05  s_a_white
 *  Support for new audio interface for better compatibility.
 *
 *  Revision 1.1  2001/01/08 16:41:43  s_a_white
 *  App and Library Seperation
 *
 *  Revision 1.5  2000/12/11 19:07:42  s_a_white
 *  AC99 Update.
 *
 ***************************************************************************/

#ifndef WAV_FILE_HEADER_H
#define WAV_FILE_HEADER_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include "../AudioBase.h"

struct wavHeader                        // little endian format
{
    char mainChunkID[4];                // 'RIFF' (ASCII)

    unsigned char length[4];            // file length

    char chunkID[4];                    // 'WAVE' (ASCII)
    char subChunkID[4];                    // 'fmt ' (ASCII)
    char subChunkLen[4];                // length of subChunk, always 16 bytes
    unsigned char format[2];            // currently always = 1 = PCM-Code

    unsigned char channels[2];            // 1 = mono, 2 = stereo
    unsigned char sampleFreq[4];        // sample-frequency
    unsigned char bytesPerSec[4];        // sampleFreq * blockAlign
    unsigned char blockAlign[2];        // bytes per sample * channels
    unsigned char bitsPerSample[2];
    
    char dataChunkID[4];                // keyword, begin of data chunk; = 'data' (ASCII)

    unsigned char dataChunkLen[4];        // length of data
};

class WavFile: public AudioBase
{
private:
    unsigned long int byteCount;

    static const wavHeader defaultWavHdr;
    wavHeader wavHdr;

    std::ostream *file;
    bool headerWritten;  // whether final header has been written
    int precision;

public:

    WavFile();
    
    // Only unsigned 8-bit, and signed 16-bit, samples are supported.
    // Endian-ess is adjusted if necessary.
    //
    // If number of sample bytes is given, this can speed up the
    // process of closing a huge file on slow storage media.

    float *open(AudioConfig &cfg, const char *name);
    
    // After write call old buffer is invalid and you should
    // use the new buffer provided instead.
    float *write();
    void  close();
    void  pause() {;}
    const char *extension () const { return ".wav"; }
    ~WavFile() { close(); }
    
    // Rev 1.3 (saw) - Changed, see AudioBase.h
    float *reset ()
    {
        if (file != 0)
            return _sampleBuffer;
        return NULL;
    }

    // Stream state.
    bool fail() const { return (file->fail() != 0); }
    bool bad()  const { return (file->bad()  != 0); }

    operator bool()  const { return (file->good() != 0); }
    bool operator!() const { return (file->fail() != 0); }
};

#endif /* WAVE_FILE_HEADER_H */
