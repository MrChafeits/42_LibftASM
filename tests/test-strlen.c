#define TEST_NAME "strlen"
#define TEST_MAIN

#include "test-string.h"

#include <limits.h>
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
// #include <cmocka.h>

typedef size_t (*proto_t)(const char *);

//! Faster versions utilizing AVX instructions
extern size_t A_strlen(const char *s);

//! Must use repeat string operation
extern size_t ft_strlen(const char *s);

IMPL(strlen, 0);
IMPL(ft_strlen, 0);
IMPL(A_strlen, 0);

void null_test_success(void **state) {}

int setup(void **state)
{
	(void)state;
	page_size = 2 * getpagesize();
	buf1 = mmap(0, (BUF1PAGES + 1) * page_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	assert(buf1 != MAP_FAILED);
	assert(mprotect(buf1 + BUF1PAGES * page_size, page_size, PROT_NONE) == 0);

	buf2 = mmap(0, 2 * page_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	assert(buf2 != MAP_FAILED);
	assert(mprotect(buf2 + page_size, page_size, PROT_NONE) == 0);

	if (do_srandom)
	{
		printf("Setting seed to 0x%x\n", seed);
		srandom(seed);
	}

	memset(buf1, 0xa5, BUF1PAGES * page_size);
	memset(buf2, 0x5a, page_size);
	return 0;
}

int teardown(void **state)
{
	munmap(buf1, (BUF1PAGES + 1) * page_size);
	munmap(buf2, 2 * page_size);
	return 0;
}

/* static void
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
} */

static void
do_one_test(impl_t *impl, const char *s, size_t exp_len)
{
	size_t len = CALL(impl, s);
	if (len != exp_len)
	{
		printf("Wrong result in function %s %zd %zd\n", impl->name, len, exp_len);
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

	char *buf = (char *)(buf1);

	for (i = 0; i < len; ++i)
		buf[align + i] = 1 + 11111 * i % CHAR_MAX;
	buf[align + len] = 0;
	FOR_EACH_IMPL(impl, 0) {
		do_one_test(impl, (char *)(buf + align), len);
	}
}

static void
do_random_tests(void)
{
	size_t i, j, n, align, len;
	char *p = (char *)(buf1 + page_size - 512 * sizeof(char));

	for (n = 0; n < ITERATIONS; n++)
	{
		align = random() & 15;
		len = random() & 511;
		if (len + align > 510)
			len = 511 - align - (random() & 7);
		j = len + align + 64;
		if (j > 512)
			j = 512;

		for (i = 0; i < j; i++)
		{
			if (i == len + align)
				p[i] = 0;
			else
			{
				p[i] = random() & 255;
				if (i >= align && i < len + align && !p[i])
					p[i] = (random() & 127) + 1;
			}
		}
		FOR_EACH_IMPL(impl, 1) {
		if (CALL(impl, (char *)(p + align)) != len)
		{
			printf("Iteration %zd - wrong result in function %s(%zd) %zd != %zd, p %p",
				   n, impl->name, align, CALL(impl, (char *)(p + align)), len, p);
			ret = 1;
		}
		}
	}
}

int test_main(void)
{
	__start_impls = calloc(4, sizeof(*__start_impls));
	__stop_impls = calloc(4, sizeof(*__stop_impls));
	__start_impls[0] = tst_ft_strlen;
	__start_impls[1] = tst_A_strlen;
	__start_impls[2] = tst_strlen;
	__start_impls[3] = tst_strlen;

	__stop_impls[0] = tst_strlen;
	__stop_impls[1] = tst_strlen;
	__stop_impls[2] = tst_strlen;
	__stop_impls[3] = tst_strlen;
	size_t i;

	printf("%zd\n", A_strlen("AAAAFJFLKJSGKJSEJKEIOSDFOIDSFJKFSDKLJFJKLSSELFKJ"));
	test_init();

	printf("%20s", "");
	FOR_EACH_IMPL(impl, 0)
		printf("\t%s", impl->name);
	putchar('\n');

	for (i = 1; i < 8; ++i)
	{
		do_test(sizeof(char) * i, i);
		do_test(0, i);
	}

	for (i = 2; i <= 12; ++i)
	{
		do_test(0, 1 << i);
		do_test(sizeof(char) * 7, 1 << i);
		do_test(sizeof(char) * i, 1 << i);
		do_test(sizeof(char) * i, (size_t)((1 << i) / 1.5));
	}

	do_random_tests();
	return ret;
}
