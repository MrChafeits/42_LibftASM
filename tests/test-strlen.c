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
#include <assert.h>

unsigned char *buf1, *buf2;
int ret, do_srandom;
unsigned int seed;
size_t page_size;
#define ITERATIONS 100000

//! Must use repeat string operation
extern size_t ft_strlen(const char *s);

//! Faster versions utilizing AVX instructions
extern size_t A_strlen(const char *s);

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
do_one_test(const char *s, size_t exp_len)
{
	size_t len = ft_strlen(s);
	if (len != exp_len) {
		printf("Wrong result in function ft_strlen %zd %zd\n", len, exp_len);
		ret = 1;
		return;
	}
}

static void
do_test(size_t align, size_t len)
{
	size_t i;

	align &= 63;
	if (align + sizeof(char) * len >= page_size)
		return;

	char *buf = (char*)(buf1);

	for (i = 0; i < len; ++i)
		buf[align + i] = 1 + 11111 * i % CHAR_MAX;
	buf[align + len] = 0;
	do_one_test((char*)(buf + align), len);
}

static void
do_random_tests(void)
{
	size_t i, j, n, align, len;
	char *p = (char*)(buf1 + page_size - 512 * sizeof(char));

	for (n = 0; n < ITERATIONS; n++) {
		align = random() & 15;
		len = random() & 511;
		if (len + align > 510)
			len = 511 - align - (random() & 7);
		j = len + align + 64;
		if (j > 512)
			j = 512;

		for (i = 0; i < j; i++) {
			if (i == len + align)
				p[i] = 0;
			else {
				p[i] = random() & 255;
				if (i >= align && i < len + align && !p[i])
					p[i] = (random() & 127) + 1;
			}
		}
	}

	if (ft_strlen((char*)(p+align)) != len) {
		printf("Iteration %zd - wrong result in function ft_strlen(%zd) %zd != %zd, p %p",
			n, align, ft_strlen((char*)(p + align)), len, p);
		ret = 1;
	}
}

int
test_main(void)
{
	size_t i;

	test_init();

	for (i = 1; i < 8; ++i) {
		do_test(sizeof(char) * i, i);
		do_test(0, i);
	}

	for (i = 2; i <= 12; ++i) {
		do_test(0, 1 << i);
		do_test(sizeof(char) * 7, 1 << i);
		do_test(sizeof(char) * i, 1 << i);
		do_test(sizeof(char) * i, (size_t)((1 << i) / 1.5));
	}

	do_random_tests();
	return ret;
}
