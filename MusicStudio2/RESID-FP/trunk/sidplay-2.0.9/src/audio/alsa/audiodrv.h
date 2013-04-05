// --------------------------------------------------------------------------
// Advanced Linux Sound Architecture (ALSA) specific audio driver interface.
// --------------------------------------------------------------------------
/***************************************************************************
 *  $Log: audiodrv.h,v $
 *  Revision 1.6  2005/07/18 19:46:44  s_a_white
 *  Switch from obsolete alsa interface (patch by shd).
 *
 *  Revision 1.5  2002/01/10 19:04:00  s_a_white
 *  Interface changes for audio drivers.
 *
 *  Revision 1.4  2001/10/30 23:35:35  s_a_white
 *  Added pause support.
 *
 *  Revision 1.3  2001/01/23 17:50:57  s_a_white
 *  Removed duplicate #endif.
 *
 *  Revision 1.2  2001/01/18 18:35:41  s_a_white
 *  Support for multiple drivers added.  C standard update applied (There
 *  should be no spaces before #)
 *
 *  Revision 1.1  2001/01/08 16:41:43  s_a_white
 *  App and Library Seperation
 *
 ***************************************************************************/

#ifndef audio_alsa_h_
#define audio_alsa_h_

#include "config.h"
#ifdef   HAVE_ALSA

#ifndef AudioDriver
#define AudioDriver Audio_ALSA
#endif

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <alsa/asoundlib.h>
#include "../AudioBase.h"


class Audio_ALSA: public AudioBase
{	
private:  // ------------------------------------------------------- private
    snd_pcm_t * _audioHandle;
    int _alsa_to_frames_divisor;

    void outOfOrder ();

public:  // --------------------------------------------------------- public
    Audio_ALSA();
    ~Audio_ALSA();

    float *open  (AudioConfig &cfg, const char *name);
    void  close ();
    // Rev 1.2 (saw) - Changed, see AudioBase.h	
    float *reset ();
    float *write ();
    void  pause () {;}
};

#endif // HAVE_ALSA
#endif // audio_alsa_h_
