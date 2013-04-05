/*
 * A basic WAV output file type - Implementation.
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
 *  $Log: WavFile.cpp,v $
 *  Revision 1.11  2004/02/26 18:19:52  s_a_white
 *  Updates for VC7 (use real libstdc++ headers instead of draft ones).
 *
 *  Revision 1.10  2002/03/04 19:07:48  s_a_white
 *  Fix C++ use of nothrow.
 *
 *  Revision 1.9  2002/01/10 18:57:01  s_a_white
 *  Interface changes and fixes for bigendian machines.
 *
 *  Revision 1.8  2001/12/11 19:38:13  s_a_white
 *  More GCC3 Fixes.
 *
 *  Revision 1.7  2001/11/22 08:55:29  s_a_white
 *  Bugfix
 *
 *  Revision 1.6  2001/11/22 08:43:48  s_a_white
 *  Added WAV_ to HAVE_IOS_OPENMODE
 *
 *  Revision 1.5  2001/11/21 23:18:08  s_a_white
 *  Wrapped ios::openmode as not supported by VC.
 *
 *  Revision 1.4  2001/11/21 19:54:15  s_a_white
 *  GCC3 update
 *
 *  Revision 1.3  2001/09/17 19:07:11  s_a_white
 *  Sample enconding support added.
 *
 *  Revision 1.2  2001/07/03 17:54:05  s_a_white
 *  Support for new audio interface for better compatibility.
 *
 *  Revision 1.1  2001/01/08 16:41:43  s_a_white
 *  App and Library Seperation
 *
 *  Revision 1.4  2000/12/11 19:07:42  s_a_white
 *  AC99 Update.
 *
 ***************************************************************************/

#include <stdlib.h>
#include <sidplay/sidendian.h>
#include "WavFile.h"
#include "WavFileDefs.h"

#ifdef WAV_HAVE_EXCEPTIONS
#   include <new>
#endif

#ifdef WAV_HAVE_IOS_OPENMODE
    typedef std::ios::openmode openmode;
#else
    typedef int openmode;
#endif


const wavHeader WavFile::defaultWavHdr = {
    // ASCII keywords are hex-ified.
    {0x52,0x49,0x46,0x46}, {0,0,0,0}, {0x57,0x41,0x56,0x45},
    {0x66,0x6d,0x74,0x20}, {16,0,0,0},
    {1,0}, {0,0}, {0,0,0,0}, {0,0,0,0}, {0,0}, {0,0},
    {0x64,0x61,0x74,0x61}, {0,0,0,0}
};

WavFile::WavFile()
: wavHdr(defaultWavHdr),
  file(0)
{
    precision = 32;
    headerWritten = false;
}

float* WavFile::open(AudioConfig &cfg, const char* name)
{
    unsigned long  int freq;
    unsigned short int channels, bits, format;
    unsigned short int blockAlign;
    unsigned long  int bufSize;

    precision = cfg.precision;

    bits        = precision;
    format      = (precision == 16 ) ? 1 : 3;
    channels    = cfg.channels;
    freq        = cfg.frequency;
    blockAlign  = (bits>>3)*channels;
    bufSize     = freq * blockAlign;
    cfg.bufSize = bufSize;

    if (name == NULL)
        return NULL;

    if (file && !file->fail())
        close();
   
    byteCount = 0;

    // We need to make a buffer for the user
#if defined(WAV_HAVE_EXCEPTIONS)
    _sampleBuffer = new(std::nothrow) float[bufSize];
#else
    _sampleBuffer = new float[bufSize];
#endif
    if (!_sampleBuffer)
        return NULL;

    // Fill in header with parameters and expected file size.
    endian_little32(wavHdr.length,sizeof(wavHeader)-8);
    endian_little16(wavHdr.channels,channels);
    endian_little16(wavHdr.format,format);
    endian_little32(wavHdr.sampleFreq,freq);
    endian_little32(wavHdr.bytesPerSec,freq*blockAlign);
    endian_little16(wavHdr.blockAlign,blockAlign);
    endian_little16(wavHdr.bitsPerSample,bits);
    endian_little32(wavHdr.dataChunkLen,0);

    openmode createAttr = std::ios::out;
#if defined(WAV_HAVE_IOS_BIN)
    createAttr |= std::ios::bin;
#else
    createAttr |= std::ios::binary;
#endif

    if (strncmp("-", name, 2) == 0) {
        file = &std::cout;
    } else {
        file = new std::ofstream(name, createAttr|std::ios::trunc);
    }

    _settings = cfg;
    return _sampleBuffer;
}

const uint_least32_t fixedSize = 1024;

float* WavFile::write()
{
    short int buf16[fixedSize];
    unsigned long i;
    if (file && !file->fail()) {
        unsigned long int bytes = fixedSize;
        if (!headerWritten) {
            file->write((char*)&wavHdr,sizeof(wavHeader));
            headerWritten = true;
        }

        /* XXX endianness... */
        if (precision == 16) {
            bytes *= 2;
            for (i=0;i<fixedSize;i++) {
                buf16[i] =  (_sampleBuffer[i] * 32768);
            }
            file->write((char*)&buf16, bytes);
        } else {
            bytes *= 4;
            file->write((char*)_sampleBuffer, bytes);
        }
        byteCount += bytes;

    }
    return _sampleBuffer;
}

void WavFile::close()
{
    if (file && !file->fail()) {
        endian_little32(wavHdr.length,byteCount+sizeof(wavHeader)-8);
        endian_little32(wavHdr.dataChunkLen,byteCount);
        if (file != &std::cout) {
            file->seekp(0, std::ios::beg);
            file->write((char*)&wavHdr,sizeof(wavHeader));
            delete file;
        }
        file = 0;
        delete[] _sampleBuffer;
    }
}
