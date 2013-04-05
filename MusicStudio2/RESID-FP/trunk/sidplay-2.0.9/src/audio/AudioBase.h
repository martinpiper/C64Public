/***************************************************************************
                          AudioBase.h  -  description
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
 *  $Log: AudioBase.h,v $
 *  Revision 1.4  2001/11/16 19:34:29  s_a_white
 *  Added extension to be used for file audio devices.
 *
 *  Revision 1.3  2001/10/30 23:34:45  s_a_white
 *  Added pause.
 *
 *  Revision 1.2  2001/07/03 17:53:29  s_a_white
 *  Added call to get pointer to current music buffer.
 *
 *  Revision 1.1  2001/01/08 16:41:43  s_a_white
 *  App and Library Seperation
 *
 *  Revision 1.4  2000/12/11 19:07:14  s_a_white
 *  AC99 Update.
 *
 ***************************************************************************/

#ifndef _AudioBase_h_
#define _AudioBase_h_

#include <string.h>
#include "AudioConfig.h"

class AudioBase
{
protected:
    AudioConfig _settings;
    const char *_errorString;
    float      *_sampleBuffer;

public:
    AudioBase ()
    {
        _errorString  = "None";
        _sampleBuffer = NULL;
    }
    virtual ~AudioBase () {;}

    // All drivers must support these
    virtual float *open(AudioConfig &cfg, const char *name) = 0;
    virtual float *reset() = 0;
    virtual float *write() = 0;
    virtual void  close () = 0;
    virtual void  pause () = 0;
    virtual const char *extension () const { return ""; }
    float *buffer () { return _sampleBuffer; }

    void getConfig (AudioConfig &cfg) const {
        cfg = _settings;
    }
    
    const char *getErrorString () const {
        return _errorString;
    }
};

#endif // _AudioBase_h_
