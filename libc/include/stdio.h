#ifndef _STDIO_H
#define _STDIO_H

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C"{
#endif

int printf(const char* __restrict,...);
int putchar(int);
int puts(const char*);

#ifdef __cplusplus
}
#endif

#endif //_STDIO_H
