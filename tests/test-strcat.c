#include <string.h>

// Cmocka needs these
// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
// clang-format on

typedef char *(*proto_t)(char *restrict s1, const char *restrict s2);

extern char *ft_strcat(char *restrict s1, const char *restrict s2);

proto_t arr[] = {
    ft_strcat,
};

static void test_strcat(proto_t fn, void **state __unused) {
  char b[32];
  strcpy(b, "abc");
  assert_ptr_equal(fn(b, "123456"), b);

  // Check if strcat failed to null-terminate
  assert_int_equal(b[9], 0);
  assert_int_equal(strlen(b), strlen("abc") + strlen("123456"));

  // Check if the string is correct
  assert_int_equal(strcmp(b, "abc123456"), 0);
}

static void strcat_test(void **state) {
  for (size_t ii = 0; ii < sizeof(arr) / sizeof(*arr); ++ii)
    test_strcat(arr[ii], state);
}

#pragma mark - Public Functions -

int strcat_tests(void) {
  const struct CMUnitTest strcat_tests[] = {
      cmocka_unit_test(strcat_test),
  };

  return cmocka_run_group_tests(strcat_tests, NULL, NULL);
}
#ifdef SINGLE_TEST
int test_main(void) { return strcat_tests(); }
#endif
