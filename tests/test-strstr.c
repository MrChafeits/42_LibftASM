#include <string.h>
// Cmocka needs these
// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
// clang-format on

#define TEST_MAIN
#define TEST_NAME "strstr"
#include "test-string.h"

extern char *ft_strstr(const char *s1, const char *s2);

void *mempcpy(void *dest, const void *src, size_t len) {
  return memcpy(dest, src, len) + len;
}

static char *stupid_strstr(const char *s1, const char *s2) {
  ssize_t s1len = strlen(s1);
  ssize_t s2len = strlen(s2);

  if (s2len > s1len)
    return NULL;

  for (ssize_t i = 0; i <= s1len - s2len; ++i) {
    ssize_t j;
    for (j = 0; j < s2len; ++j)
      if (s1[i + j] != s2[j])
        break;
    if (j == s2len)
      return (char *)s1 + i;
  }

  return NULL;
}

static char *simple_strstr(const char *s1, const char *s2) {
  size_t ii;

  if (*s2 == '\0')
    return ((char *)s1);
  if (strlen(s2) > strlen(s1))
    return (NULL);
  while (*s1) {
    if (*s1 == *s2) {
      for (ii = 0; (s1[ii] == s2[ii] && s2[ii]); ++ii)
        ;
      if (s2[ii] == '\0')
        return ((char *)s1);
    }
    s1++;
  }
  return (NULL);
}

typedef char *(*proto_t)(const char *, const char *);

IMPL(ft_strstr, 0)
IMPL(stupid_strstr, 0)
IMPL(simple_strstr, 0)
IMPL(strstr, 1)

static int check_result(s_tstbuf *tst, const char *s1, const char *s2,
                        char *exp_result) {
  char *result = CALL(tst->impl, s1, s2);
  assert_int_equal(result, exp_result);
  if (result != exp_result) {
    errc(0, 0, "Wrong result in function %s %s %s", tst->impl->name,
         (result == NULL) ? "(null)" : result,
         (exp_result == NULL) ? "(null)" : exp_result);
    tst->ret = 1;
    return -1;
  }

  return 0;
}

static void do_one_test(s_tstbuf *tst, const char *s1, const char *s2,
                        char *exp_result) {
  if (check_result(tst, s1, s2, exp_result) < 0)
    return;
}

static void do_test(s_tstbuf *tst, size_t align1, size_t align2, size_t len1,
                    size_t len2, int fail) {
  char *s1 = (char *)(tst->buf1 + align1);
  char *s2 = (char *)(tst->buf2 + align2);

  static const char d[] = "1234567890abcdef";
#define dl (sizeof(d) - 1)
  char *ss2 = s2;
  for (size_t l = len2; l > 0; l = l > dl ? l - dl : 0) {
    size_t t = l > dl ? dl : l;
    ss2 = mempcpy(ss2, d, t);
  }
  s2[len2] = '\0';

  if (fail) {
    char *ss1 = s1;
    for (size_t l = len1; l > 0; l = l > dl ? l - dl : 0) {
      size_t t = l > dl ? dl : l;
      memcpy(ss1, d, t);
      ++ss1[len2 > 7 ? 7 : len2 - 1];
      ss1 += t;
    }
  } else {
    memset(s1, '0', len1);
    memcpy(s1 + len1 - len2, s2, len2);
  }
  s1[len1] = '\0';

  do_one_test(tst, s1, s2, fail ? NULL : s1 + len1 - len2);
}

static void check1(s_tstbuf *tst) {
  const char s1[] = "F_BD_CE_BD_EF_BF_BD_EF_BF_BD_EF_BF_BD_EF_BF_BD_C3_88_20_"
                    "EF_BF_BD_EF_BF_BD_EF_BF_BD_C3_A7_20_EF_BF_BD";
  const char s2[] = "_EF_BF_BD_EF_BF_BD_EF_BF_BD_EF_BF_BD_EF_BF_BD";
  char *exp_result;

  exp_result = stupid_strstr(s1, s2);

  check_result(tst, s1, s2, exp_result);
}

static void check2(s_tstbuf *tst) {
  const char s1[] = ", enable_static, \0, enable_shared, ";
  char *exp_result;
  char *s2 = (void *)tst->buf1 + tst->page_size - 18;

  strcpy(s2, s1);
  exp_result = stupid_strstr(s1, s1 + 18);
  check_result(tst, s1, s1 + 18, exp_result);
  check_result(tst, s2, s1 + 18, exp_result);
}

#define N 1024

static void pr23637(s_tstbuf *tst) {
  char *h = (char *)tst->buf1;
  char *n = (char *)tst->buf2;

  for (int i = 0; i < N; i++) {
    n[i] = 'x';
    h[i] = ' ';
    h[i + N] = 'x';
  }

  n[N] = '\0';
  h[N * 2] = '\0';

  /* Ensure we don't match at the first 'x'.  */
  h[0] = 'x';

  char *exp_result = stupid_strstr(h, n);
  check_result(tst, h, n, exp_result);
}

static int strstr_test(void **state) {
  s_tstbuf *tst = (s_tstbuf *)(*state);

  check1(tst);
  check2(tst);
  pr23637(tst);

  for (size_t klen = 2; klen < 32; ++klen)
    for (size_t hlen = 2 * klen; hlen < 16 * klen; hlen += klen) {
      do_test(tst, 0, 0, hlen, klen, 0);
      do_test(tst, 0, 0, hlen, klen, 1);
      do_test(tst, 0, 3, hlen, klen, 0);
      do_test(tst, 0, 3, hlen, klen, 1);
      do_test(tst, 0, 9, hlen, klen, 0);
      do_test(tst, 0, 9, hlen, klen, 1);
      do_test(tst, 0, 15, hlen, klen, 0);
      do_test(tst, 0, 15, hlen, klen, 1);

      do_test(tst, 3, 0, hlen, klen, 0);
      do_test(tst, 3, 0, hlen, klen, 1);
      do_test(tst, 3, 3, hlen, klen, 0);
      do_test(tst, 3, 3, hlen, klen, 1);
      do_test(tst, 3, 9, hlen, klen, 0);
      do_test(tst, 3, 9, hlen, klen, 1);
      do_test(tst, 3, 15, hlen, klen, 0);
      do_test(tst, 3, 15, hlen, klen, 1);

      do_test(tst, 9, 0, hlen, klen, 0);
      do_test(tst, 9, 0, hlen, klen, 1);
      do_test(tst, 9, 3, hlen, klen, 0);
      do_test(tst, 9, 3, hlen, klen, 1);
      do_test(tst, 9, 9, hlen, klen, 0);
      do_test(tst, 9, 9, hlen, klen, 1);
      do_test(tst, 9, 15, hlen, klen, 0);
      do_test(tst, 9, 15, hlen, klen, 1);

      do_test(tst, 15, 0, hlen, klen, 0);
      do_test(tst, 15, 0, hlen, klen, 1);
      do_test(tst, 15, 3, hlen, klen, 0);
      do_test(tst, 15, 3, hlen, klen, 1);
      do_test(tst, 15, 9, hlen, klen, 0);
      do_test(tst, 15, 9, hlen, klen, 1);
      do_test(tst, 15, 15, hlen, klen, 0);
      do_test(tst, 15, 15, hlen, klen, 1);

      do_test(tst, 15, 15, hlen + klen * 4, klen * 4, 0);
      do_test(tst, 15, 15, hlen + klen * 4, klen * 4, 1);
    }

  do_test(tst, 0, 0, tst->page_size - 1, 16, 0);
  do_test(tst, 0, 0, tst->page_size - 1, 16, 1);

  return tst->ret;
}

static void test_ft_strstr(void **state) {
  s_tstbuf *tst = (s_tstbuf *)(*state);
  tst->impl = &tst_ft_strstr;
  tst->ret |= strstr_test(state);
}

static void test_SIMPLE_strstr(void **state) {
  s_tstbuf *tst = (s_tstbuf *)(*state);
  tst->impl = &tst_simple_strstr;
  tst->ret |= strstr_test(state);
}

static void test_STUPID_strstr(void **state) {
  s_tstbuf *tst = (s_tstbuf *)(*state);
  tst->impl = &tst_stupid_strstr;
  tst->ret |= strstr_test(state);
}

static void test_strstr(void **state) {
  s_tstbuf *tst = (s_tstbuf *)(*state);
  tst->impl = &tst_strstr;
  tst->ret |= strstr_test(state);
}

int strstr_tests(void) {
  const struct CMUnitTest strstr_tests[] = {
      cmocka_unit_test(test_ft_strstr),
      cmocka_unit_test(test_SIMPLE_strstr),
      cmocka_unit_test(test_STUPID_strstr),
      cmocka_unit_test(test_strstr),
  };

  return cmocka_run_group_tests(strstr_tests, test_setup, test_teardown);
}

#ifdef SINGLE_TEST
int test_main(void) { return strstr_tests(); }
#endif
