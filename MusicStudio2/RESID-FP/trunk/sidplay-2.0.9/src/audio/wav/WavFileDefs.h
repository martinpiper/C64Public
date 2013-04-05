/*
 * A basic WAV output file type - Preprocessor definitions.
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
 *  $Log: WavFileDefs.h,v $
 *  Revision 1.2  2001/11/22 08:45:38  s_a_white
 *  Added support for WAV_HAVE_IOS_OPENMODE
 *
 *  Revision 1.1  2001/01/08 16:41:43  s_a_white
 *  App and Library Seperation
 *
 *  Revision 1.3  2000/12/11 19:07:42  s_a_white
 *  AC99 Update.
 *
 ***************************************************************************/

#ifndef WAV_FILE_DEFS_H
#define WAV_FILE_DEFS_H

#include "config.h"

#undef WAV_WORDS_BIGENDIAN
#if defined(WORDS_BIGENDIAN)
  #define WAV_WORDS_BIGENDIAN
#endif

#undef HAVE_IOS_BIN
#if defined(HAVE_IOS_BIN)
  #define WAV_HAVE_IOS_BIN
#endif

#undef WAV_HAVE_EXCEPTIONS
#ifdef HAVE_EXCEPTIONS
  #define WAV_HAVE_EXCEPTIONS
#endif

#undef WAV_HAVE_BAD_COMPILER
#ifdef HAVE_BAD_COMPILER
  #define HAVE_BAD_COMPILER
#endif

#undef WAV_HAVE_IOS_OPENMODE
#ifdef HAVE_IOS_OPENMODE
  #define WAV_HAVE_IOS_OPENMODE
#endif

/* Whether to revert any changes applied to the endian-ess of the
   non-const sample buffer contents after they have been written. */
#undef WAV_REVERT_BUFFER_CHANGES

#endif /* WAV_FILE_DEFS_H */
