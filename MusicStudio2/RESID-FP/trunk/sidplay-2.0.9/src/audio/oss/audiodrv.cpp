/***************************************************************************
                          audiodrv.cpp  -  OSS sound support
                             -------------------
    begin                : Sat Jul 8 2000
    copyright            : (C) 2000 by Simon White
    email                : s_a_white@email.com
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
 *  $Log: audiodrv.cpp,v $
 *  Revision 1.6  2002/03/04 19:07:48  s_a_white
 *  Fix C++ use of nothrow.
 *
 *  Revision 1.5  2001/12/11 19:38:13  s_a_white
 *  More GCC3 Fixes.
 *
 *  Revision 1.4  2001/07/14 16:53:29  s_a_white
 *  Updated to use new audio base class.
 *
 *  Revision 1.3  2001/01/23 21:23:23  s_a_white
 *  Replaced SID_HAVE_EXCEPTIONS with HAVE_EXCEPTIONS in new
 *  drivers.
 *
 *  Revision 1.2  2001/01/18 18:36:16  s_a_white
 *  Support for multiple drivers added.  C standard update applied (There
 *  should be no spaces before #)
 *
 *  Revision 1.1  2001/01/08 16:41:43  s_a_white
 *  App and Library Seperation
 *
 *  Revision 1.4  2000/12/24 00:44:25  s_a_white
 *  HAVE_EXCEPTIONS
 *
 *  Revision 1.3  2000/12/11 19:09:12  s_a_white
 *  AC99 Update.
 *
 ***************************************************************************/

// --------------------------------------------------------------------------
// ``Open Sound System (OSS)'' specific audio driver interface.
// --------------------------------------------------------------------------

#include "audiodrv.h"
#ifdef   HAVE_OSS

#include <stdio.h>
#ifdef HAVE_EXCEPTIONS
#   include <new>
#endif

#if defined(HAVE_NETBSD)
const char Audio_OSS::AUDIODEVICE[] = "/dev/audio";
#else
const char Audio_OSS::AUDIODEVICE[] = "/dev/dsp";
#endif

Audio_OSS::Audio_OSS()
{
    // Reset everything.
    outOfOrder();
    _swapEndian  = false;
}

Audio_OSS::~Audio_OSS ()
{
    close ();
}

void Audio_OSS::outOfOrder ()
{
    // Reset everything.
    _errorString = "None";
    _audiofd     = -1;
}

float *Audio_OSS::open (AudioConfig &cfg, const char *)
{
    int mask, format;
    int wantedFormat = 0;
    int temp;

    if (_audiofd != -1)
    {
        _errorString = "ERROR: Device already in use";
        return NULL;
    }

    if (access (AUDIODEVICE, W_OK) == -1)
    {
        _errorString = "ERROR: Could locate an audio device.";
        goto open_error;
    }

    if ((_audiofd = ::open (AUDIODEVICE, O_WRONLY, 0)) == (-1))
    {
        _errorString = "ERROR: Could not open audio device.";
        goto open_error;
    }

    format = AFMT_S16_NE;
    if (ioctl (_audiofd, SNDCTL_DSP_SETFMT, &format) == (-1))
    {
        _errorString = "AUDIO: Could not set sample format.";
        goto open_error;
    }
    
    // Set mono/stereo.
    if (ioctl (_audiofd, SNDCTL_DSP_CHANNELS, &cfg.channels) == (-1))
    {
        _errorString = "AUDIO: Could not set mono/stereo.";
        goto open_error;
    }

    // Verify and accept the number of channels the driver accepted.
    switch (cfg.channels)
    {
    case 1:
    case 2:
    break;
    default:
        _errorString = "AUDIO: Could not set mono/stereo.";
        goto open_error;
    break;
    }
        
    // Set frequency.
    if (ioctl (_audiofd, SNDCTL_DSP_SPEED, &cfg.frequency) == (-1))
    {
        _errorString = "AUDIO: Could not set frequency.";
        goto open_error;
    }

    ioctl (_audiofd, SNDCTL_DSP_GETBLKSIZE, &temp);
    cfg.bufSize = (unsigned) temp;
#ifdef HAVE_EXCEPTIONS
    _sampleBuffer = new(std::nothrow) float[cfg.bufSize];
#else
    _sampleBuffer = new float[cfg.bufSize];
#endif

    if (!_sampleBuffer)
    {
        _errorString = "AUDIO: Unable to allocate memory for sample buffers.";
        goto open_error;
    }

    // Setup internal Config
    _settings = cfg;
return _sampleBuffer;

open_error:
    if (_audiofd != -1)
    {
        close ();
        _audiofd = -1;
    }

    perror (AUDIODEVICE);
return NULL;
}

// Close an opened audio device, free any allocated buffers and
// reset any variables that reflect the current state.
void Audio_OSS::close ()
{
    if (_audiofd != (-1))
    {
        ::close (_audiofd);
        delete [] _sampleBuffer;
        outOfOrder ();
    }
}

float *Audio_OSS::write ()
{
    short tmp[_settings.bufSize];

    if (_audiofd == (-1))
    {
        _errorString = "ERROR: Device not open.";
        return NULL;
    }

    for (uint_least32_t n = 0; n < _settings.bufSize; n ++) {
            tmp[n] = _sampleBuffer[n] * (1 << 15);
    }

    ::write (_audiofd, tmp, 2 * _settings.bufSize);
    return _sampleBuffer;
}

#endif // HAVE_OSS
