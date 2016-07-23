#pragma once
#define _forceinline inline
#define _inline static inline
#define __cdecl
#define _cdecl
#define __single_inheritance
#define RESTRICT
#ifndef TRUE
typedef int BOOL;


#define FALSE 0
#define TRUE !FALSE
#endif
#include <dlfcn.h>
#ifndef max
#define max( a, b )                 (((a) > (b)) ? (a) : (b))
#define min( a, b )                 (((a) < (b)) ? (a) : (b))
#endif
#include <ctype.h>

