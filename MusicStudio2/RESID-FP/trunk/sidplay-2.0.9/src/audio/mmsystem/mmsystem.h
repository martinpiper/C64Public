/***************************************************************************
                          mmsystem.h  -  ``Waveout for Windows''
                                         specific audio driver interface.
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
 *  $Log: mmsystem.h,v $
 *  Revision 1.5  2001/10/30 23:35:35  s_a_white
 *  Added pause support.
 *
 *  Revision 1.4  2001/07/03 17:54:23  s_a_white
 *  Support for new audio interface for better compatibility.
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
 ***************************************************************************/

#ifndef _audio_mmsystem_h_
#define _audio_mmsystem_h_

#include "config.h"
#ifdef   HAVE_MMSYSTEM

#ifndef AudioDriver
#define AudioDriver Audio_MMSystem
#endif

#include <windows.h>
#include <mmsystem.h>
#include "../AudioBase.h"
#include "ExternalSystem.h"


class Audio_MMSystem: public AudioBase
{
private:  // ------------------------------------------------------- private
    HWAVEOUT    waveHandle;

    // Rev 1.3 (saw) - Buffer sizes adjusted to get a
    // correct playtimes
	#define  MAXBUFBLOCKS 16
    float   *blocks[MAXBUFBLOCKS];
    HGLOBAL  blockHandles[MAXBUFBLOCKS];
    WAVEHDR *blockHeaders[MAXBUFBLOCKS];
    HGLOBAL  blockHeaderHandles[MAXBUFBLOCKS];
    int      blockNum;
    bool     isOpen;
    int      bufSize;

public:  // --------------------------------------------------------- public
    Audio_MMSystem();
    ~Audio_MMSystem();

    float *open  (AudioConfig &cfg, const char *name);
    void  close ();
    // Rev 1.2 (saw) - Changed, see AudioBase.h    
    float *reset ();
    float *write ();
    void  pause () {;}
};

#endif // HAVE_MMSYSTEM
#endif // _audio_mmsystem_h_
