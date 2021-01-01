

#ifndef __GLOBAL__
#define __GLOBAL__





typedef unsigned int  uint;
typedef unsigned char byte;
typedef unsigned int   u32;
typedef unsigned short u16;
typedef unsigned char  u8;
typedef int   s32;
typedef short s16;
typedef char  s8;
//typedef unsigned int   uint32_t;
//typedef unsigned short uint16_t;
//typedef unsigned char  uint8_t;
typedef unsigned int  size_t;
typedef unsigned int  word;

#define NULL 0

//#define BIG_ENDIAN

#ifndef __cplusplus
typedef unsigned int bool;
#define true  1
#define false 0
#endif

typedef void(*FUNC_VOID)(void);
#include <string.h>
#include "q_printf.h"
#define G_OK   0
#define G_ERR -1


#define CRITICAL_INIT()
#define CRITICAL_IN()
#define CRITICAL_OUT()

#define TEST_ENABLE 1

extern void q_printf(char *fmt, ...);
#define printf q_printf

#define hton(v) (((((uint)v>>0)&0xff)<<24)|((((uint)v>>8)&0xff)<<16)|((((uint)v>>16)&0xff)<<8)|((((uint)v>>24)&0xff)<<0))
#define htons(v) (((v>>8)&0xff)|((v&0xff)<<8))

#define container_of(ptr, type, member) ((type*)((char*)(ptr)-(uint)(&((type*)0)->member)))
#endif


