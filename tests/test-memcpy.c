#include <string.h>

// Cmocka needs these
// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
// clang-format on

#pragma mark - Definitions -

#define N 80
typedef void *(*proto_t)(void *restrict dst, const void *restrict src, size_t n);

#pragma mark - Declarations -

//! Must use repeat string operation
extern void *ft_memcpy(void *restrict dst, const void *restrict src, size_t n);

//! Faster versions utilizing AVX instructions
extern void *A_memcpy(void *restrict dst, const void *restrict src, size_t n);

static proto_t arr[] = {
	ft_memcpy,
	A_memcpy,
};
static char buf[512];

#pragma mark - Private Functions -

static void *aligned(void *p)
{
	return (void *)(((intptr_t)p + 63) & -64);
}

static void test_align(proto_t fn, unsigned dalign, unsigned salign, size_t len)
{
	char *src = aligned(buf);
	char *dst = aligned(buf + 128);
	char *want = aligned(buf + 256);
	char *p;
	unsigned i;

	assert_false(salign + len > N);
	assert_false(dalign + len > N);

	for (i = 0; i < N; i++)
	{
		src[i] = '#';
		dst[i] = want[i] = ' ';
	}

	for (i = 0; i < len; i++)
	{
		src[salign + i] = want[dalign + i] = '0' + (char)i;
	}

	p = fn(dst + dalign, src + salign, len);

	assert_ptr_equal(p, dst + dalign);

	for (i = 0; i < N; i++)
	{
		assert_int_equal(dst[i], want[i]);
	}
}

static void memcpy_aligned_test(void __unused **state)
{
	for (size_t n = 0; n < sizeof(arr) / sizeof(*arr); ++n)
	{
		for (unsigned i = 0; i < 16; i++)
		{
			for (unsigned j = 0; j < 16; j++)
			{
				for (size_t k = 0; k < 64; k++)
				{
					test_align(arr[n], i, j, k);
				}
			}
		}
	}
}

#pragma mark - Public Functions -

int memcpy_tests(void)
{
	const struct CMUnitTest memcpy_tests[] = {cmocka_unit_test(memcpy_aligned_test)};

	return cmocka_run_group_tests(memcpy_tests, NULL, NULL);
}

int test_main(void) {
	return memcpy_tests();
}
