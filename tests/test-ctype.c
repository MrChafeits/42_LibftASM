#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Cmocka needs these
// clang-format off
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
// clang-format on

extern int ft_isalpha(int c);
extern int ft_isdigit(int c);
extern int ft_isalnum(int c);
extern int ft_isascii(int c);
extern int ft_isprint(int c);
extern int ft_toupper(int c);
extern int ft_tolower(int c);

static void isalpha_test(void **state __unused) {
  unsigned short int c;
  for (c = 0; c <= UCHAR_MAX; ++c)
    assert_int_equal(ft_isalpha(c), isalpha(c));
}

static void isdigit_test(void **state __unused) {
  unsigned short int c;
  for (c = 0; c <= UCHAR_MAX; ++c)
    assert_int_equal(ft_isdigit(c), isdigit(c));
}

static void isalnum_test(void **state __unused) {
  unsigned short int c;
  for (c = 0; c <= UCHAR_MAX; ++c)
    assert_int_equal(ft_isalnum(c), isalnum(c));
}

static void isascii_test(void **state __unused) {
  unsigned short int c;
  for (c = 0; c <= UCHAR_MAX; ++c)
    assert_int_equal(ft_isascii(c), isascii(c));
}

static void isprint_test(void **state __unused) {
  unsigned short int c;
  for (c = 0; c <= UCHAR_MAX; ++c)
    assert_int_equal(ft_isprint(c), isprint(c));
}

static void toupper_test(void **state __unused) {
  unsigned short int c;
  for (c = 0; c <= UCHAR_MAX; ++c)
    assert_int_equal(ft_toupper(c), toupper(c));
}

static void tolower_test(void **state __unused) {
  unsigned short int c;
  for (c = 0; c <= UCHAR_MAX; ++c)
    assert_int_equal(ft_tolower(c), tolower(c));
}

int ctype_tests(void) {
  const struct CMUnitTest ctype_tests[] = {
      cmocka_unit_test(isalpha_test), cmocka_unit_test(isdigit_test),
      cmocka_unit_test(isalnum_test), cmocka_unit_test(isascii_test),
      cmocka_unit_test(isprint_test), cmocka_unit_test(toupper_test),
      cmocka_unit_test(tolower_test),
  };
  return cmocka_run_group_tests(ctype_tests, NULL, NULL);
}
#ifdef SINGLE_TEST
int test_main(void) { return ctype_tests(); }
#endif
