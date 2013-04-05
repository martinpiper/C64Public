/***************************************************************************
                          sidtypes.h  -  type definition file
                             -------------------
    begin                : Mon Jul 3 2000
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

#ifndef _sidtypes_h_
#define _sidtypes_h_

#include <stdint.h>

#include "sidplay/sidconfig.h"

typedef struct
{
    /* distortion tunables */
    float           rate, point, voice_nonlinearity, cf_threshold;
    /* type 3 tunables */
    float           baseresistance, offset, steepness, minimumfetresistance;
    /* type 4 tunables */
    float           k, b;
} sid_filter_t;

typedef unsigned int uint;
typedef float    float32_t;
typedef double   float64_t;

#endif /* _sidtypes_h_ */
