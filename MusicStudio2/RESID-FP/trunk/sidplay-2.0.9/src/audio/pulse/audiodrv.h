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

#ifndef audio_pulse_h_
#define audio_pulse_h_

#include "config.h"
#ifdef   HAVE_PULSE

#ifndef AudioDriver
#define AudioDriver Audio_Pulse
#endif

#include <pulse/simple.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "../AudioBase.h"

class Audio_Pulse: public AudioBase
{	
private:  // ------------------------------------------------------- private
    pa_simple *_audioHandle;
    void outOfOrder ();

public:  // --------------------------------------------------------- public
    Audio_Pulse();
    ~Audio_Pulse();

    float *open  (AudioConfig &cfg, const char *name);
    void  close ();
    float *reset ()
    {
        return _sampleBuffer;
    }
    float *write ();
    void  pause () {;}
};

#endif // HAVE_PULSE
#endif // audio_pulse_h_
