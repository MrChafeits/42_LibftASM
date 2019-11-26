#include <stdlib.h>
#include <string.h>

// Cmocka needs these
// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
// clang-format on

//! Must use repeat string operation
extern char *ft_strdup(const char *s1);

size_t simple_strlen(const char *s) {
  const char *p = s;
  while (*p)
    ++p;
  return p - s;
}

static void strdup_test(void **state __unused) {
  const char *base = "this is a base string";
  char *dup;

  dup = ft_strdup(base);
  assert_ptr_not_equal(dup, NULL);
  assert_int_equal(strlen(dup), 21);
  assert_int_equal(strncmp(dup, base, strlen(dup)), 0);
  free(dup);
}

int strdup_tests(void) {
  const struct CMUnitTest strdup_tests[] = {
      cmocka_unit_test(strdup_test),
  };

  return cmocka_run_group_tests(strdup_tests, NULL, NULL);
}

#ifdef SINGLE_TEST
int test_main(void) { return strdup_tests(); }
#endif
