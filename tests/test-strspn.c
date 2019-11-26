#define TEST_MAIN
#define TEST_NAME "strspn"
#include "test-string.h"

// Cmocka needs these
// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
// clang-format on

#define STRSPN strspn
#define CHAR char
#define UCHAR unsigned char
#define SIMPLE_STRSPN simple_strspn
#define STUPID_STRSPN stupid_strspn
#define STRLEN strlen
#define STRCHR strchr
#define BIG_CHAR CHAR_MAX
#define SMALL_CHAR 127

typedef size_t (*proto_t)(const CHAR *, const CHAR *);
size_t SIMPLE_STRSPN(const CHAR *, const CHAR *);
size_t STUPID_STRSPN(const CHAR *, const CHAR *);
extern size_t ft_strspn(const CHAR *, const CHAR *);

IMPL(ft_strspn, 0);
IMPL(STUPID_STRSPN, 0);
IMPL(SIMPLE_STRSPN, 0);
IMPL(STRSPN, 0);

size_t SIMPLE_STRSPN(const CHAR *s, const CHAR *acc) {
  const CHAR *r, *str = s;
  CHAR c;

  while ((c = *s++) != '\0') {
    for (r = acc; *r != '\0'; ++r)
      if (*r == c)
        break;
    if (*r == '\0')
      return s - str - 1;
  }
  return s - str - 1;
}

size_t STUPID_STRSPN(const CHAR *s, const CHAR *acc) {
  size_t ns = STRLEN(s), nacc = STRLEN(acc);
  size_t i, j;

  for (i = 0; i < ns; ++i) {
    for (j = 0; j < nacc; ++j)
      if (s[i] == acc[j])
        break;
    if (j == nacc)
      return i;
  }
  return i;
}

static void do_one_test(impl_t *impl, const CHAR *s, const CHAR *acc,
                        size_t exp_res) {
  size_t res = CALL(impl, s, acc);
  assert_int_equal(res, exp_res);
}

static void do_test(impl_t *impl, size_t align, size_t pos, size_t len) {
  size_t i;
  CHAR *acc, *s;

  align &= 7;
  if ((align + pos + 10) * sizeof(CHAR) >= page_size || len > 240 || !len)
    return;

  acc = (CHAR *)(buf2) + (random() & 255);
  s = (CHAR *)(buf1) + align;

  for (i = 0; i < len; ++i) {
    acc[i] = random() & BIG_CHAR;
    if (!acc[i])
      acc[i] = random() & BIG_CHAR;
    if (!acc[i])
      acc[i] = 1 + (random() & SMALL_CHAR);
  }
  acc[len] = '\0';

  for (i = 0; i < pos; ++i)
    s[i] = acc[random() % len];
  s[pos] = random() & BIG_CHAR;
  if (STRCHR(acc, s[pos]))
    s[pos] = '\0';
  else {
    for (i = pos + 1; i < pos + 10; ++i)
      s[i] = random() & BIG_CHAR;
    s[i] = '\0';
  }

  do_one_test(impl, s, acc, pos);
}

static void do_random_tests(impl_t *impl) {
  size_t i, j, n, align, pos, alen, len;
  UCHAR *p = (UCHAR *)(buf1 + page_size) - 512;
  UCHAR *acc;

  for (n = 0; n < ITERATIONS; n++) {
    align = random() & 15;
    if (random() & 1)
      alen = random() & 63;
    else
      alen = random() & 15;
    if (!alen)
      pos = 0;
    else
      pos = random() & 511;
    if (pos + align >= 511)
      pos = 510 - align - (random() & 7);
    len = random() & 511;
    if (len + align >= 512)
      len = 511 - align - (random() & 7);
    acc = (UCHAR *)(buf2 + page_size) - alen - 1 - (random() & 7);
    for (i = 0; i < alen; ++i) {
      acc[i] = random() & BIG_CHAR;
      if (!acc[i])
        acc[i] = random() & BIG_CHAR;
      if (!acc[i])
        acc[i] = 1 + (random() & SMALL_CHAR);
    }
    acc[i] = '\0';
    j = (pos > len ? pos : len) + align + 64;
    if (j > 512)
      j = 512;

    for (i = 0; i < j; i++) {
      if (i == len + align)
        p[i] = '\0';
      else if (i == pos + align) {
        p[i] = random() & BIG_CHAR;
        if (STRCHR((CHAR *)acc, p[i]))
          p[i] = '\0';
      } else if (i < align || i > pos + align)
        p[i] = random() & BIG_CHAR;
      else
        p[i] = acc[random() % alen];
    }

    assert_int_equal(CALL(impl, (CHAR *)(p + align), (CHAR *)acc), (pos < len ? pos : len));
  }
}

int strspn_test(void **state) {
  s_tstbuf *tst = (s_tstbuf*)(*state);
  size_t i;

  for (i = 0; i < 32; ++i) {
    do_test(tst->impl, 0, 512, i);
    do_test(tst->impl, i, 512, i);
  }

  for (i = 1; i < 8; ++i) {
    do_test(tst->impl, 0, 16 << i, 4);
    do_test(tst->impl, i, 16 << i, 4);
  }

  for (i = 1; i < 8; ++i)
    do_test(tst->impl, i, 64, 10);

  for (i = 0; i < 64; ++i)
    do_test(tst->impl, 0, i, 6);

  do_random_tests(tst->impl);
  return 0;
}

void test_ft_strspn(void **state) {
  s_tstbuf *tst = (s_tstbuf*)(*state);
  tst->impl = &tst_ft_strspn;
  tst->ret |= strspn_test(state);
}

void test_SIMPLE_STRSPN(void **state) {
  s_tstbuf *tst = (s_tstbuf*)(*state);
  tst->impl = &tst_SIMPLE_STRSPN;
  tst->ret |= strspn_test(state);
}

void test_STUPID_STRSPN(void **state) {
  s_tstbuf *tst = (s_tstbuf*)(*state);
  tst->impl = &tst_STUPID_STRSPN;
  tst->ret |= strspn_test(state);
}

void test_STRSPN(void **state) {
  s_tstbuf *tst = (s_tstbuf*)(*state);
  tst->impl = &tst_STRSPN;
  tst->ret |= strspn_test(state);
}

int strspn_tests(void) {
  const struct CMUnitTest strspn_tests[] = {
    cmocka_unit_test(test_ft_strspn),
    cmocka_unit_test(test_SIMPLE_STRSPN),
    cmocka_unit_test(test_STUPID_STRSPN),
    cmocka_unit_test(test_STRSPN),
  };

  return cmocka_run_group_tests(strspn_tests, test_setup, test_teardown);
}

#ifdef SINGLE_TEST
int test_main(void) {
  return strspn_tests();
}
#endif
