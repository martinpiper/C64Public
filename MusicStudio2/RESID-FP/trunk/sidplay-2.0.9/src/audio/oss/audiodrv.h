/***************************************************************************
                          audiodrv.h  -  OSS sound support
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
 *  $Log: audiodrv.h,v $
 *  Revision 1.5  2001/10/30 23:35:35  s_a_white
 *  Added pause support.
 *
 *  Revision 1.4  2001/07/14 16:53:29  s_a_white
 *  Updated to use new audio base class.
 *
 *  Revision 1.3  2001/01/23 17:50:59  s_a_white
 *  Removed duplicate #endif.
 *
 *  Revision 1.2  2001/01/18 18:36:16  s_a_white
 *  Support for multiple drivers added.  C standard update applied (There
 *  should be no spaces before #)
 *
 *  Revision 1.1  2001/01/08 16:41:43  s_a_white
 *  App and Library Seperation
 *
 *  Revision 1.3  2000/12/11 19:09:12  s_a_white
 *  AC99 Update.
 *
 ***************************************************************************/

// --------------------------------------------------------------------------
// ``Open Sound System (OSS)'' specific audio driver interface.
// --------------------------------------------------------------------------

#ifndef audio_oss_h_
#define audio_oss_h_

#include "config.h"
#ifdef   HAVE_OSS

#ifndef AudioDriver
#define AudioDriver Audio_OSS
#endif

#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#if defined(HAVE_SYS_SOUNDCARD_H)
#   include <sys/soundcard.h>
#elif defined(HAVE_LINUX_SOUNDCARD_H)
#   include <linux/soundcard.h>
#elif defined(HAVE_MACHINE_SOUNDCARD_H)
#   include <machine/soundcard.h>
#elif defined(HAVE_SOUNDCARD_H)
#   include <soundcard.h>
#else
#   error Audio driver not supported.
#endif

#include "../AudioBase.h"


class Audio_OSS: public AudioBase
{	
private:  // ------------------------------------------------------- private
    static   const char AUDIODEVICE[];
    volatile int   _audiofd;

    bool _swapEndian;
    void outOfOrder ();

public:  // --------------------------------------------------------- public
    Audio_OSS();
    ~Audio_OSS();

    float *open  (AudioConfig &cfg, const char *name);
    void  close ();
    // Rev 1.2 (saw) - Changed, see AudioBase.h	
    float *reset ()
    {
        if (_audiofd != (-1))
        {
            if (ioctl (_audiofd, SNDCTL_DSP_RESET, 0) != (-1))
                return _sampleBuffer;
        }
        return NULL;
    }
    float *write ();
    void  pause () {;}
};

#endif // HAVE_OSS
#endif // audio_oss_h_
