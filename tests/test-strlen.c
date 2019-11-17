#include <string.h>

// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
// clang-format on

//! Faster versions utilizing AVX instructions
extern size_t A_strlen(const char *s);

//! Must use repeat string operation
extern size_t ft_strlen(const char *s);

static void ft_strlen_test(void **state) {
	assert_int_equal(ft_strlen(""), 0);
	assert_int_equal(ft_strlen("a"), 1);
	assert_int_equal(ft_strlen("ab"), 2);
	assert_int_equal(ft_strlen("abasildjfaskjdf;askdfuoisudfoiajsdfaf"), 37);
}

static void A_strlen_test(void **state) {
	assert_int_equal(A_strlen(""), 0);
	assert_int_equal(A_strlen("a"), 1);
	assert_int_equal(A_strlen("ab"), 2);
	assert_int_equal(A_strlen("abasildjfaskjdf;askdfuoisudfoiajsdfaf"), 37);
}

#pragma mark - Public Functions -

int strlen_tests(void) {
	const struct CMUnitTest strlen_tests[] = {
		cmocka_unit_test(ft_strlen_test),
		cmocka_unit_test(A_strlen_test),
	};

	return cmocka_run_group_tests(strlen_tests, NULL, NULL);
}
int test_main(){return strlen_tests();}
