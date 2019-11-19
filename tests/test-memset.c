#include <string.h>
#include <limits.h>

// Cmocka needs these
// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
// clang-format on

extern void *ft_memset(void *b, int c, size_t len);

#pragma mark - Definitions -

#define N 400

#pragma mark - Declarations -

static char buf1[N];
static char buf2[N];

#pragma mark - Private Functions -

static char* aligned(void* p) {
	return (char*)(((intptr_t)p + 63) & -64);
}

static void test_align(int align, size_t len) {
	char* s = aligned(buf1 + 64) + align;
	char* want = aligned(buf2 + 64) + align;
	char* p;
	int i;

	assert_false(len + 64 > (size_t)(buf1 + N - s));
	assert_false(len + 64 > (size_t)(buf2 + N - want));

	for(i = 0; i < N; i++)
		buf1[i] = buf2[i] = ' ';

	for(i = 0; i < (int)len; i++)
		want[i] = '#';

	p = ft_memset(s, '#', len);

	assert_ptr_equal(p, s);

	for(i = -64; i < (int)len + 64; i++)
		assert_int_equal(s[i], want[i]);
}

static void memset_aligned_test(void** state __unused) {
	for(int i = 0; i < 16; i++)
		for(size_t j = 0; j < 200; j++)
			test_align(i, j);
}

static void check_input_(char c, void** state __unused) {
	ft_memset(buf1, c, 10);
	for(int i = 0; i < 10; i++)
		assert_int_equal(buf1[i], c);
}

static void check_input(void** state) {
	check_input_('c', state);
	check_input_(0, state);
	check_input_(-1, state);
	check_input_((char)INT_MAX, state);
	check_input_((char)-INT_MAX, state);
	check_input_((char)0xab, state);
}

#pragma mark - Public Functions -

int memset_tests(void) {
	const struct CMUnitTest memset_tests[] = {
		cmocka_unit_test(memset_aligned_test),
		cmocka_unit_test(check_input),
	};

	return cmocka_run_group_tests(memset_tests, NULL, NULL);
}
#ifdef SINGLE_TEST
int test_main(void) {
	return memset_tests();
}
#endif
