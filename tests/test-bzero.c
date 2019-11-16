#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <limits.h>
#include <sys/mman.h>
#include <assert.h>

extern void *ft_bzero(void *b, size_t len);

#define ITERATIONS 100000
// typedef struct {
// 	const char *name;
// 	void (*fn)(void);
// 	long test;
// } impl_t;
// extern impl_t __start_impls[], __stop_impls[];
// #define IMPL(name, test) \
// impl_t tst_ ## name \
// __attribute__((section("impls"), aligned(sizeof(void*)))) \
//  = {__STRING(name), (void (*)(void))name, test};
// #define CALL(impl, ...) \
// (*(proto_t)(impl)->fn)(__VA_ARGS__)

static void
do_one_bzero_test(char *s, size_t n)
{
	char tstbuf[n];
	bzero(tstbuf, n);
	ft_bzero(s, n);
	if (memcmp(s, tstbuf, n) != 0) {
		puts("BOGUS result from ft_bzero");
	}
}

unsigned char *buf1, *buf2;
int ret, do_srandom;
unsigned int seed;
size_t page_size;

#ifndef BUF1PAGES
# define BUF1PAGES 1
#endif

static void
test_init(void)
{
	page_size = 2 * getpagesize();
	buf1 = mmap(0, (BUF1PAGES + 1) * page_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	assert(buf1 != MAP_FAILED);
	assert(mprotect(buf1 + BUF1PAGES * page_size, page_size, PROT_NONE) == 0);

	buf2 = mmap(0, 2 * page_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	assert(buf2 != MAP_FAILED);
	assert(mprotect(buf2 + page_size, page_size, PROT_NONE) == 0);

	if (do_srandom) {
		printf("Setting seed to 0x%x\n", seed);
		srandom(seed);
	}

	memset(buf1, 0xa5, BUF1PAGES * page_size);
	memset(buf2, 0x5a, page_size);
}

static void
do_bzero_test(size_t align, size_t len)
{
	align &= 7;
	if ((align + len) * sizeof(char) > page_size)
		return;
	do_one_bzero_test((char*)(buf1)+align, len);
}

int
test_main(void)
{
	size_t i;
	int c = 0;

	test_init();

	{
		for (i = 0; i < 18; ++i)
			do_bzero_test(0, 1 << i);
		for (i = 1; i < 32; ++i) {
			do_bzero_test(i, i);
			if (i & (i - 1))
				do_bzero_test(0, i);
		}
		do_bzero_test(1, 14);
		do_bzero_test(3, 1024);
		do_bzero_test(4, 64);
		do_bzero_test(2, 25);
	}
	return ret;
}
