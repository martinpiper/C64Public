#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DecompressV.h"


int DecompressV( const u8 * source, u32 sourceLen ,u8 * dest, u32 * destLen )
{
	size_t ret = -1;

	if (ret == (size_t) -1)
	{
		return GBA_E_ERROR;
	}

	*destLen = (u32) ret;

	return GBA_E_OK;
}
