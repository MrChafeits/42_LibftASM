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

extern void *ft_memset(void *b, int c, size_t len);

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
do_one_memset_test(char *s, int c, size_t n)
{
	char tstbuf[n];
	char *res = ft_memset(s, c, n);
	if (res != s
		|| memset(tstbuf, c, n) != tstbuf
		|| memcmp(s, tstbuf, n) != 0) {
		puts("BOGUS result from ft_memset");
		return;
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
do_memset_test(size_t align, int c, size_t len)
{
	align &= 7;
	if ((align + len) * sizeof(char) > page_size)
		return;
	do_one_memset_test((char*)(buf1)+align, c, len);
}

static void
do_random_memset_tests(void)
{
	size_t i, j, k, n, align, len, size;
	int c, o;
	unsigned char *p, *res;
	unsigned char *p2 = (unsigned char*)buf2;

	for (i = 0; i < 65536 / sizeof(char); ++i)
		p2[i] = random() & CHAR_MAX;

	for (n = 0; n < ITERATIONS; n++) {
		if ((random() & 31) == 0)
			size = 65536 / sizeof(char);
		else
			size = 512;
		p = (unsigned char*)(buf1 + page_size) - size;
		len = random() & (size - 1);
		align = size - len - (random() & 31);
		if (align > size)
			align = size - len;
		if ((random() & 7) == 0)
			align &= ~63;
		if ((random() & 7) == 0)
			c = 0;
		else
			c = random() & CHAR_MAX;
		o = random() & CHAR_MAX;
		if (o == c)
			o = (c + 1) & CHAR_MAX;
		j = len + align + 128;
		if (j > size)
			j = size;
		if (align >= 128)
			k = align - 128;
		else
			k = 0;
		for (i = k; i < align; ++i)
			p[i] = o;
		for (i = align + len; i < j; ++i)
			p[i] = o;

		for (i = 0; i < len; ++i) {
			p[i + align] = p2[i];
			if (p[i+align] == c)
				p[i+align] = o;
		}
		res = (unsigned char*)ft_memset((char*)p+align, c, len);
		if (res != p + align) {
			printf("Iteration %zd - wrong result in function ft_memset(%zd, %d, %zd) %p != %p\n", n, align, c, len, res, p + align);
		}
		for (i = k; i < align; ++i) {
			if (p[i] != o) {
				printf("Iteration %zd - garbage before ft_memset(%zd, %d, %zd)\n", n, align, c, len);
				ret = 1;
				break;
			}
		}
		for (; i < align + len; ++i) {
			if (p[i] != c) {
				printf("Iteration %zd - not cleared correctly ft_memset(%zd, %d, %zd)\n", n, align, c, len);
				ret = 1;
				break;
			}
		}
		for (; i < j; ++i) {
			if (p[i] != o) {
				printf("Iteration %zd - garbage after ft_memset(%zd, %d, %zd)\n", n, align, c, len);
				ret = 1;
				break;
			}
		}
	}
}

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

int
test_main(void)
{
	size_t i;
	int c = 0;

	test_init();

	for (c = -65; c <= 130; c += 65) {
		for (i = 0; i < 18; ++i)
			do_memset_test(0, c, 1 << i);
		for (i = 1; i < 32; ++i) {
			do_memset_test(i, c, i);
			if (i & (i - 1))
				do_memset_test(0, c, i);
		}
		do_memset_test(1, c, 14);
		do_memset_test(3, c, 1024);
		do_memset_test(4, c, 64);
		do_memset_test(2, c, 25);
	}
	do_random_memset_tests();
	return ret;
}
