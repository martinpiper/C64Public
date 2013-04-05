/***************************************************************************
                          libini.h  -  Header file of functions to
                                       manipulate an ini file.
                             -------------------
    begin                : Fri Apr 21 2000
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

#ifndef _libini_h_
#define _libini_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#define INI_ADD_EXTRAS
#define INI_ADD_LIST_SUPPORT

#define INI_LINKAGE

#ifndef INI_EXTERN
#   ifdef DLL_EXPORT          /* defined by libtool (if required) */
#       define INI_EXTERN __declspec(dllexport)
#   endif
#   ifdef LIBINI_DLL_IMPORT   /* define if linking with this dll */
#       define INI_EXTERN extern __declspec(dllimport)
#   endif
#   ifndef INI_EXTERN     /* static linking or !_WIN32 */
#      if defined(__GNUC__) && (__GNUC__ >= 4)
#          define INI_EXTERN __attribute__ ((visibility("default")))
#      else
#          define INI_EXTERN
#      endif
#   endif
#endif

#ifndef INI_ADD_EXTRAS
#undef  INI_ADD_LIST_SUPPORT
#endif

typedef void* ini_fd_t;

/* Rev 1.2 Added new fuction */
INI_EXTERN ini_fd_t INI_LINKAGE ini_open     (const char *name, const char *mode,
                                              const char *comment);
INI_EXTERN int      INI_LINKAGE ini_close    (ini_fd_t fd);
INI_EXTERN int      INI_LINKAGE ini_flush    (ini_fd_t fd);
INI_EXTERN int      INI_LINKAGE ini_delete   (ini_fd_t fd);

/* Rev 1.2 Added these functions to make life a bit easier, can still be implemented
 * through ini_writeString though. */
INI_EXTERN int INI_LINKAGE ini_locateKey     (ini_fd_t fd, const char *key);
INI_EXTERN int INI_LINKAGE ini_locateHeading (ini_fd_t fd, const char *heading);
INI_EXTERN int INI_LINKAGE ini_deleteKey     (ini_fd_t fd);
INI_EXTERN int INI_LINKAGE ini_deleteHeading (ini_fd_t fd);

/* Returns the number of bytes required to be able to read the key as a
 * string from the file. (1 should be added to this length to account
 * for a NULL character).  If delimiters are used, returns the length
 * of the next data element in the key to be read */
INI_EXTERN int INI_LINKAGE ini_dataLength (ini_fd_t fd);

/* Default Data Type Operations
 * Arrays implemented to help with reading, for writing you should format the
 * complete array as a string and perform an ini_writeString. */
INI_EXTERN int INI_LINKAGE ini_readString  (ini_fd_t fd, char *str, size_t size);
INI_EXTERN int INI_LINKAGE ini_writeString (ini_fd_t fd, const char *str);
INI_EXTERN int INI_LINKAGE ini_readInt     (ini_fd_t fd, int *value);


#ifdef INI_ADD_EXTRAS
    /* Read Operations */
    INI_EXTERN int INI_LINKAGE ini_readLong    (ini_fd_t fd, long   *value);
    INI_EXTERN int INI_LINKAGE ini_readDouble  (ini_fd_t fd, double *value);
    INI_EXTERN int INI_LINKAGE ini_readBool    (ini_fd_t fd, int    *value);

    /* Write Operations */
    INI_EXTERN int INI_LINKAGE ini_writeInt    (ini_fd_t fd, int    value);
    INI_EXTERN int INI_LINKAGE ini_writeLong   (ini_fd_t fd, long   value);
    INI_EXTERN int INI_LINKAGE ini_writeDouble (ini_fd_t fd, double value);
    INI_EXTERN int INI_LINKAGE ini_writeBool   (ini_fd_t fd, int    value);

    /* Extra Functions */
    INI_EXTERN int INI_LINKAGE ini_append      (ini_fd_t fddst, ini_fd_t fdsrc);
#endif /* INI_ADD_EXTRAS */


#ifdef INI_ADD_LIST_SUPPORT
    /* Rev 1.1 Added - List Operations (Used for read operations only)
     * Be warned, once delimiters are set, every key that is read will be checked for the
     * presence of sub strings.  This will incure a speed hit and therefore once a line
     * has been read and list/array functionality is not required, set delimiters
     * back to NULL.
     */

    /* Returns the number of elements in an list being seperated by the provided delimiters */
    INI_EXTERN int INI_LINKAGE ini_listLength      (ini_fd_t fd);
    /* Change delimiters, default "" */
    INI_EXTERN int INI_LINKAGE ini_listDelims      (ini_fd_t fd, const char *delims);
    /* Set index to access in a list.  When read the index will automatically increment */
    INI_EXTERN int INI_LINKAGE ini_listIndex       (ini_fd_t fd, unsigned long index);
#endif /* INI_ADD_LIST_SUPPORT */

#ifdef __cplusplus
}
#endif

#endif /* _libini_h_ */
