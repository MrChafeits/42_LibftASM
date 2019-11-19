#ifndef LIBFTASM_H
#define LIBFTASM_H

#ifndef _SIZE_T_DEFINED
#include "stddef.h"
#endif

// Define integer types with known size: int32_t, uint32_t, int64_t, uint64_t.
// If this doesn't work then insert compiler-specific defitions here:
#if defined(__GNUC__) || defined(__LP64__) || (defined(_MSC_VER) && _MSC_VER >= 1600)
  // Compilers supporting C99 or C++0x have stdint.h defining these integer types
  #include <stdint.h>
  #define INT64_SUPPORTED // Remove this if the compiler doesn't support 64-bit integers
#elif defined(_MSC_VER)
  // Older Microsoft compilers have their own definition
  typedef signed   __int16  int16_t;
  typedef unsigned __int16 uint16_t;
  typedef signed   __int32  int32_t;
  typedef unsigned __int32 uint32_t;
  typedef signed   __int64  int64_t;
  typedef unsigned __int64 uint64_t;
  #define INT64_SUPPORTED // Remove this if the compiler doesn't support 64-bit integers
#else
  typedef signed   short int  int16_t;
  typedef unsigned short int uint16_t;
  typedef signed   int        int32_t;
  typedef unsigned int       uint32_t;
  typedef signed   long long  int64_t;
  typedef unsigned long long uint64_t;
  #define INT64_SUPPORTED // Remove this if the compiler doesn't support 64-bit integers
#endif

// Turn off name mangling
#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
Function prototypes, memory and string functions
***********************************************************************/

void   ft_bzero(void *s, size_t n);
void*  ft_memcpy(void *restrict dst, const void *restrict src, size_t n);
void*  ft_memset(void *dst, int c, size_t count);
char*  ft_strcat(char *restrict s1, const char *restrict s2);
char*  ft_strdup(const char *s1);
size_t ft_strlen(const char *s);

void*  A_memcpy(void *restrict dst, const void *restrict src, size_t count);
void*  A_memmove(void *dst, const void *src, size_t count);
void*  A_memset(void *dst, int c, size_t count);
size_t A_memcmp(const void *s1, const void *s2, size_t n);
size_t GetMemcpyCacheLimit(void);
void   SetMemcpyCacheLimit(size_t);

size_t A_strlen(const char *str);

size_t A_strspn (const char *str, const char *set);
size_t A_strcspn(const char *str, const char *set);
size_t A_substring(char *dest, const char *source, size_t pos, size_t len);

/***********************************************************************
Function prototypes, ctype and stdio functions
***********************************************************************/

int    ft_isalpha(int c);
int    ft_isdigit(int c);
int    ft_isalnum(int c);
int    ft_isprint(int c);
int    ft_isascii(int c);
int    ft_tolower(int c);
int    ft_toupper(int c);

int    ft_puts(const char *s);
void   ft_cat(int fd);

/***********************************************************************
Function prototypes, miscellaneous functions
***********************************************************************/

void   A_DebugBreak(void);
int    InstructionSet(void);
void   CpuType(int *vendor, int *family, int *model);
size_t DataCacheSize(int level);

#ifdef INT64_SUPPORTED
    uint64_t ReadTSC(void);
#else
    uint32_t ReadTSC(void);
#endif


#ifdef __cplusplus
} // end of extern "C"
#endif

#endif
