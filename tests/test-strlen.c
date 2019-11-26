#include <string.h>

// Cmocka needs these
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

static void ft_strlen_test(void **state __unused) {
  assert_int_equal(ft_strlen(""), 0);
  assert_int_equal(ft_strlen("a"), 1);
  assert_int_equal(ft_strlen("ab"), 2);
  assert_int_equal(ft_strlen("abasildjfaskjdf;askdfuoisudfoiajsdfaf"), 37);
}

int strlen_tests(void) {
  const struct CMUnitTest strlen_tests[] = {
      cmocka_unit_test(ft_strlen_test),
  };

  return cmocka_run_group_tests(strlen_tests, NULL, NULL);
}

#ifdef SINGLE_TEST
int test_main(void) { return strlen_tests(); }
#endif
