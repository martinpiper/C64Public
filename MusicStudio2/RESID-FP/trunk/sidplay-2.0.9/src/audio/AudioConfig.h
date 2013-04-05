/***************************************************************************
                          AudioConfig.h  -  description
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
 *  $Log: AudioConfig.h,v $
 *  Revision 1.3  2002/01/10 22:03:27  s_a_white
 *  Namespace not used yet (enable later).
 *
 *  Revision 1.2  2002/01/10 18:58:17  s_a_white
 *  Interface changes for 2.0.8.
 *
 *  Revision 1.1  2001/01/08 16:41:43  s_a_white
 *  App and Library Seperation
 *
 *  Revision 1.3  2000/12/11 19:07:14  s_a_white
 *  AC99 Update.
 *
 ***************************************************************************/

#ifndef _AudioConfig_h_
#define _AudioConfig_h_

#include <stdint.h>

class AudioConfig
{
public:
    uint_least32_t frequency;
    int            precision;
    int            channels;
    uint_least32_t bufSize;       // sample buffer size
    
    AudioConfig()
    {
        frequency = 48000;
        precision = 16;
        channels  = 1;
        bufSize   = 0;
    }
};

#endif  // _AudioConfig_h_
