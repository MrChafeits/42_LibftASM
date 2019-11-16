#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <limits.h>
#include <sys/mman.h>

extern unsigned char *buf1, *buf2;
extern int ret, do_srandom;
extern unsigned int seed;
extern size_t page_size;
#define ITERATIONS 100000

extern int ft_puts(const char *s);

//! Must use repeat string operation
extern char *ft_strdup(const char *s1);

extern void ft_cat(int fd);

extern int test_main(void);

int
main(int argc, const char *argv[])
{
	if (argc > 1)
		puts(argv[1]);
	return test_main();
}
