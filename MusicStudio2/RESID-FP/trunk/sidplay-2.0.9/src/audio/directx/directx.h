/***************************************************************************
                          audiodrv.h  -  ``DirectSound for Windows''
                                         specific audio driver interface.
                             -------------------
    begin                : Mon Jul 31 2000
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
 *  $Log: directx.h,v $
 *  Revision 1.5  2001/10/30 23:35:35  s_a_white
 *  Added pause support.
 *
 *  Revision 1.4  2001/07/03 17:54:35  s_a_white
 *  Support for new audio interface for better compatibility.
 *
 *  Revision 1.3  2001/01/23 17:50:59  s_a_white
 *  Removed duplicate #endif.
 *
 *  Revision 1.2  2001/01/18 18:35:57  s_a_white
 *  Support for multiple drivers added.  C standard update applied (There
 *  should be no spaces before #)
 *
 *  Revision 1.1  2001/01/08 16:41:43  s_a_white
 *  App and Library Seperation
 *
 ***************************************************************************/

#ifndef audio_directx_h_
#define audio_directx_h_

#include "config.h"
#ifdef   HAVE_DIRECTX

#ifndef AudioDriver
#define AudioDriver Audio_DirectX
#endif

#if DIRECTSOUND_VERSION < 0x0500
#   undef  DIRECTSOUND_VERSION
#   define DIRECTSOUND_VERSION 0x0500       /* version 5.0 */ 
#endif

#include <DSound.h>
#include <mmsystem.h>
#include <mmreg.h>

#include "../AudioBase.h"
#define AUDIO_DIRECTX_BUFFERS 2

class Audio_DirectX: public AudioBase
{    
private:  // ------------------------------------------------------- private
    HWND   hwnd;

    // DirectSound Support
    LPDIRECTSOUND       lpds;
    LPDIRECTSOUNDBUFFER lpDsb;
    LPDIRECTSOUNDNOTIFY lpdsNotify;
    void               *lpvData;
    // DirectSound Notify
    HANDLE rghEvent[AUDIO_DIRECTX_BUFFERS];
    DWORD  bufSize;

    bool isOpen;
    bool isPlaying;

private:
    HWND GetConsoleHwnd ();

public:  // --------------------------------------------------------- public
    Audio_DirectX();
    ~Audio_DirectX();

    // This first one assumes progrm is built as a
    // console application
    float *open  (AudioConfig &cfg, const char *name);
    float *open  (AudioConfig &cfg, const char *name, HWND hwnd);
    void  close ();    
    // Rev 1.3 (saw) - Changed
    float *reset ();
    float *write ();
    void  pause ();
};

#endif // HAVE_DIRECTX
#endif // audio_directx_h_
