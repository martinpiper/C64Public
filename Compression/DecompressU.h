#ifndef _GBADECOMPRESSU_H_
#define _GBADECOMPRESSU_H_
#include "common.h"

/**
 * Decompresses memory compressed with CompressU
 * \param source the source memory pointer
 * \param sourceLen the size of the source memory buffer
 * \param dest the destination memory buffer
 * \param destLen the pointer that will the length of the decompressed data
 * \return An error code or 0 for no error
 */
extern int DecompressU( const u8 * source, u32 sourceLen ,u8 * dest, u32 * destLen );

#endif
