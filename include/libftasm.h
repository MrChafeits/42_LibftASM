#ifndef LIBFTASM_H
#define LIBFTASM_H
// clang-format off
#include <sys/cdefs.h>

#ifndef _SIZE_T_DEFINED
#include "stddef.h"
#endif

#include <stdint.h>

// Turn off name mangling
__BEGIN_DECLS

/***********************************************************************
Function prototypes, memory and string functions
***********************************************************************/

void   ft_bzero(void *s, size_t n);
void*  ft_memcpy(void *restrict dst, const void *restrict src, size_t n);
void*  ft_memset(void *dst, int c, size_t count);
char*  ft_strcat(char *restrict s1, const char *restrict s2);
char*  ft_strdup(const char *s1);
size_t ft_strlen(const char *s);

// bonus
int    ft_memcmp(const void *p1, const void *p2, size_t n);
void*  ft_memmove(void *dst, const void *src, size_t n);
size_t ft_strspn(const char *str, const char *sep);
size_t ft_strcspn(const char *str, const char *sep);
char*  ft_strstr(const char *s1, const char *s2);

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

int    ft_atoi(const char *str);
void   ft_debugbreak(void);

uint64_t ft_readtsc(void);

__END_DECLS

// clang-format on

#endif
