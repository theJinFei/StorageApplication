#ifndef __MAIN_H__
#define __MAIN_H__


#ifdef _WIN32											//ЬѕМўБрвы
#include <intrin.h>
#include <windows.h>
#define INTRIN_SSE
#else
#include <x86intrin.h>
#include <sys/time.h>
#define INTRIN_SSE
#define _aligned_malloc(a, b) memalign(b, a)

#if __AVX512F_
#define INTRIN_AVX_512
#endif
#if __AVX__
#define INTRIN_AVX
#endif
#if __SSE__
#define INTRIN_SSE
#endif

#endif// _WIN32

#ifdef _WIN32
extern int gettimeofday(struct timeval *tp, void *tzp);
#endif
extern long timeDiffMacroSeconds(timeval& start, timeval& end);
extern char* timeFormatMacroToString(long cost);
extern void readData(char **buffer);
#endif


