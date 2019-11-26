#include <stdlib.h>

// Cmocka needs these
// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
// clang-format on

extern int ft_atoi(const char *);

static void atoi_test(void **state __unused) {
  // Integer tests
  assert_int_equal(ft_atoi("0"), 0);
  assert_int_equal(ft_atoi("10"), 10);
  assert_int_equal(ft_atoi("12345"), 12345);

  assert_int_equal(ft_atoi("-1"), -1);
  assert_int_equal(ft_atoi("-2147483647"), -2147483647);

  // INT_MAX
  assert_int_equal(ft_atoi("2147483647"), 2147483647);

  // UINT_MAX
  assert_int_equal((unsigned)ft_atoi("4294967295"), 4294967295);

  // Stop at decimal
  assert_int_equal(ft_atoi("0.1"), 0);
  assert_int_equal(ft_atoi("1.1"), 1);
  assert_int_equal(ft_atoi("2147483647.1232"), 2147483647);

  // Hex doesn't work with ft_atoi
  assert_int_equal(ft_atoi("0xFFEE"), 0);

  // Bad input
  assert_int_equal(ft_atoi("12-a"), 12);
  assert_int_equal(ft_atoi("-a"), 0);
  assert_int_equal(ft_atoi("102xxa"), 102);
  assert_int_equal(ft_atoi("a13"), 0);
}

int atoi_tests(void) {
  const struct CMUnitTest atoi_tests[] = {cmocka_unit_test(atoi_test)};

  return cmocka_run_group_tests(atoi_tests, NULL, NULL);
}

#ifdef SINGLE_TEST
int test_main(void) { return atoi_tests(); }
#endif
