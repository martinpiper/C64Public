#ifndef _AGB_H_
#define _AGB_H_

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef signed int s32;
typedef signed short s16;
typedef signed char s8;

extern u32 LONG_OFFSET_THRESHOLD;

#define GBA_E_OK					0
#define GBA_E_ERROR					(-1)
#define GBA_E_INVALID_ARGUMENT		(-2)
#define GBA_E_OUT_OF_MEMORY			(-3)

#endif
