#if 0
#define TEST_MAIN
#define TEST_NAME "bzero"

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
#include "test-string.h"

typedef void (*proto_t)(void *, size_t);

extern void ft_bzero(void *b, size_t len);
char *
inhibit_loop_to_libcall
simple_memset(char *s, int c, size_t n) {
	char *r = s, *end = s + n;
	while (r < end)
		*r++ = c;
	return s;
}
void simple_bzero(void *s, size_t n) {
	simple_memset(s, 0, n);
}
void builtin_bzero(void *b, size_t len) {
	__builtin_bzero(b, len);
}

IMPL(builtin_bzero, 0);
IMPL(ft_bzero, 0);
IMPL(bzero, 0);

static void
do_one_test(impl_t *impl, char *s, size_t n)
{
	char tstbuf[n];
	simple_bzero(tstbuf, n);
	CALL(impl, s, n);
	if (memcmp(s, tstbuf, n) != 0) {
		errc(0, 0, "Wrong result in function %s", impl->name);
		ret = 1;
		return;
	}
}

static void
do_test(size_t align, size_t len)
{
	align &= 7;
	if ((align + len) * sizeof(char) > page_size)
		return;
	FOR_EACH_IMPL(impl, 0)
		do_one_test(impl, (char*)(buf1)+align, len);
}

int
test_main(void)
{
	__start_impls = calloc(8, sizeof(impl_t));
	__stop_impls = calloc(8, sizeof(impl_t));

	__start_impls[0] = tst_ft_bzero;
	__start_impls[1] = tst_builtin_bzero;
	__start_impls[2] = tst_ft_bzero;
	__start_impls[3] = tst_bzero;

	__stop_impls[0] = tst_builtin_bzero;
	__stop_impls[1] = tst_bzero;
	__stop_impls[2] = tst_bzero;
	__stop_impls[3] = tst_bzero;

	size_t i;
	int c = 0;

	test_init();
	printf ("%24s", "");
	// FOR_EACH_IMPL (impl, 0)
	impl_t *impl;
	int count;
	if (impl_count == -1)
	{
		impl_count = 0;
		if (func_count != 0)
		{
			int f;
			impl_t *skip = NULL, *a;
			for (impl = __start_impls; impl < __stop_impls; ++impl)
				if (strcmp(impl->name, TEST_NAME) == 0)
					skip = impl;
				else
					impl_count++;
			a = impl_array = malloc((impl_count + func_count) *
									sizeof(impl_t));
			for (impl = __start_impls; impl < __stop_impls; ++impl)
				if (impl != skip)
					*a++ = *impl;
			for (f = 0; f < func_count; f++)
				if (func_list[f].usable)
				{
					a->name = func_list[f].name;
					a->fn = func_list[f].fn;
					a->test = 1;
					a++;
				}
			impl_count = a - impl_array;
		}
		else
		{
			impl_count = __stop_impls - __start_impls;
			impl_array = __start_impls;
		}
	}
	impl = impl_array;
	for (count = 0; count < impl_count; ++count, ++impl)
		if (!0 || impl->test)
		printf ("\t%s", impl->name);
	putchar ('\n');

	{
		for (i = 0; i < 18; ++i)
			do_test(0, 1 << i);
		for (i = 1; i < 32; ++i) {
			do_test(i, i);
			if (i & (i - 1))
				do_test(0, i);
		}
		do_test(1, 14);
		do_test(3, 1024);
		do_test(4, 64);
		do_test(2, 25);
	}
	return ret;
}
#endif
#include <string.h>

// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
// clang-format on

extern void ft_bzero(void *b, size_t len);

#pragma mark - Definitions -

#define N 1024

#pragma mark - Declarations -

static char buf1[N];
static char buf2[N];

#pragma mark - Private Functions -

static char *aligned(void *p) {
	return (char *)(((intptr_t)p + 63) & -64);
}

static void test_align(int align, size_t len) {
	char *s = aligned(buf1 + 64) + align;
	char *want = aligned(buf2 + 64) + align;
	char *p;
	int i;

	assert_false(len + 64 > (size_t)(buf1 + N - s));
	assert_false(len + 64 > (size_t)(buf2 + N - want));

	for (i = 0; i < N; ++i)
		buf1[i] = buf2[i] = ' ';
	for (i = 0; i < (int)len; ++i)
		want[i] = '\0';
	ft_bzero(s, len);
	for (i = -64; i < (int)len + 64; ++i)
		assert_int_equal(s[i], want[i]);
}

static void bzero_aligned_test(void **state) {
	for (int i = 0; i < 32; ++i)
		for (size_t j = 0; j < 512; ++j)
			test_align(i, j);
}

static void check_input_(int c, void **state) {
	ft_bzero(buf1, 10);
	for (int i = 0; i < 10; ++i) {
		assert_int_equal(buf1[i], c);
	}
}

static void check_input(void **state) {
	check_input_(0, state);
}

#pragma mark - Public Functions -

int bzero_tests(void) {
	const struct CMUnitTest bzero_tests[] = {
		cmocka_unit_test(bzero_aligned_test),
		cmocka_unit_test(check_input),
	};

	return cmocka_run_group_tests(bzero_tests, NULL, NULL);
}
int test_main(void){return bzero_tests();}
