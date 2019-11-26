#define TEST_MAIN
#define TEST_NAME "strcspn"
#include "test-string.h"

// Cmocka needs these
// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
// clang-format on

#define STRPBRK_RESULT(s, pos) (pos)
#define RES_TYPE size_t

#define STRCSPN strcspn
#define CHAR char
#define SIMPLE_STRCSPN simple_strcspn
#define STUPID_STRCSPN stupid_strcspn
#define STRLEN strlen

typedef size_t (*proto_t)(const CHAR *, const CHAR *);
extern size_t ft_strcspn(const CHAR *, const CHAR *);
size_t SIMPLE_STRCSPN(const CHAR *, const CHAR *);
size_t STUPID_STRCSPN(const CHAR *, const CHAR *);

IMPL(ft_strcspn, 0);
IMPL(STUPID_STRCSPN, 0)
IMPL(SIMPLE_STRCSPN, 0)
IMPL(STRCSPN, 1)

size_t SIMPLE_STRCSPN(const CHAR *s, const CHAR *rej) {
  const CHAR *r, *str = s;
  CHAR c;

  while ((c = *s++) != '\0')
    for (r = rej; *r != '\0'; ++r)
      if (*r == c)
        return s - str - 1;
  return s - str - 1;
}

size_t STUPID_STRCSPN(const CHAR *s, const CHAR *rej) {
  size_t ns = STRLEN(s), nrej = STRLEN(rej);
  size_t i, j;

  for (i = 0; i < ns; ++i)
    for (j = 0; j < nrej; ++j)
      if (s[i] == rej[j])
        return i;
  return i;
}

#undef CHAR
#undef STRLEN

#define CHAR char
#define UCHAR unsigned char
#define STRLEN strlen
#define STRCHR strchr
#define BIG_CHAR CHAR_MAX
#define SMALL_CHAR 127

static void do_one_test(s_tstbuf *tst, const CHAR *s, const CHAR *rej,
                        RES_TYPE exp_res) {
  RES_TYPE res = CALL(tst->impl, s, rej);
  if (res != exp_res) {
    errc(0, 0, "Wrong result in function %s %p %p", tst->impl->name,
         (void *)res, (void *)exp_res);
    tst->ret = 1;
    return;
  }
}

static void do_test(s_tstbuf *tst, size_t align, size_t pos, size_t len) {
  size_t i;
  int c;
  RES_TYPE result;
  CHAR *rej, *s;

  align &= 7;
  if ((align + pos + 10) * sizeof(CHAR) >= tst->page_size || len > 240)
    return;

  rej = (CHAR *)(tst->buf2) + (random() & 255);
  s = (CHAR *)(tst->buf1) + align;

  for (i = 0; i < len; ++i) {
    rej[i] = random() & BIG_CHAR;
    if (!rej[i])
      rej[i] = random() & BIG_CHAR;
    if (!rej[i])
      rej[i] = 1 + (random() & SMALL_CHAR);
  }
  rej[len] = '\0';
  for (c = 1; c <= BIG_CHAR; ++c)
    if (STRCHR(rej, c) == NULL)
      break;

  for (i = 0; i < pos; ++i) {
    s[i] = random() & BIG_CHAR;
    if (STRCHR(rej, s[i])) {
      s[i] = random() & BIG_CHAR;
      if (STRCHR(rej, s[i]))
        s[i] = c;
    }
  }
  s[pos] = rej[random() % (len + 1)];
  if (s[pos]) {
    for (i = pos + 1; i < pos + 10; ++i)
      s[i] = random() & BIG_CHAR;
    s[i] = '\0';
  }
  result = STRPBRK_RESULT(s, pos);

  do_one_test(tst, s, rej, result);
}

static void do_random_tests(s_tstbuf *tst) {
  size_t i, j, n, align, pos, len, rlen;
  RES_TYPE result;
  int c;
  UCHAR *p = (UCHAR *)(tst->buf1 + tst->page_size) - 512;
  UCHAR *rej;

  for (n = 0; n < ITERATIONS; n++) {
    align = random() & 15;
    pos = random() & 511;
    if (pos + align >= 511)
      pos = 510 - align - (random() & 7);
    len = random() & 511;
    if (pos >= len && (random() & 1))
      len = pos + 1 + (random() & 7);
    if (len + align >= 512)
      len = 511 - align - (random() & 7);
    if (random() & 1)
      rlen = random() & 63;
    else
      rlen = random() & 15;
    rej = (UCHAR *)(tst->buf2 + tst->page_size) - rlen - 1 - (random() & 7);
    for (i = 0; i < rlen; ++i) {
      rej[i] = random() & BIG_CHAR;
      if (!rej[i])
        rej[i] = random() & BIG_CHAR;
      if (!rej[i])
        rej[i] = 1 + (random() & SMALL_CHAR);
    }
    rej[i] = '\0';
    for (c = 1; c <= BIG_CHAR; ++c)
      if (STRCHR((CHAR *)rej, c) == NULL)
        break;
    j = (pos > len ? pos : len) + align + 64;
    if (j > 512)
      j = 512;

    for (i = 0; i < j; i++) {
      if (i == len + align)
        p[i] = '\0';
      else if (i == pos + align)
        p[i] = rej[random() % (rlen + 1)];
      else if (i < align || i > pos + align)
        p[i] = random() & BIG_CHAR;
      else {
        p[i] = random() & BIG_CHAR;
        if (STRCHR((CHAR *)rej, p[i])) {
          p[i] = random() & BIG_CHAR;
          if (STRCHR((CHAR *)rej, p[i]))
            p[i] = c;
        }
      }
    }

    result = STRPBRK_RESULT((CHAR *)(p + align), pos < len ? pos : len);

    if (CALL(tst->impl, (CHAR *)(p + align), (CHAR *)rej) != result) {
      errc(0, 0,
           "Iteration %zd - wrong result in function %s (%zd, %p, %zd, %zd, "
           "%zd) %p != %p",
           n, tst->impl->name, align, rej, rlen, pos, len,
           (void *)CALL(tst->impl, (CHAR *)(p + align), (CHAR *)rej),
           (void *)result);
      tst->ret = 1;
    }
  }
}

int strcspn_test(void **state) {
  s_tstbuf *tst = (s_tstbuf *)(*state);
  size_t i;

  for (i = 0; i < 32; ++i) {
    do_test(tst, 0, 512, i);
    do_test(tst, i, 512, i);
  }

  for (i = 1; i < 8; ++i) {
    do_test(tst, 0, 16 << i, 4);
    do_test(tst, i, 16 << i, 4);
  }

  for (i = 1; i < 8; ++i)
    do_test(tst, i, 64, 10);

  for (i = 0; i < 64; ++i)
    do_test(tst, 0, i, 6);

  do_random_tests(tst);
  return tst->ret;
}

void test_ft_strcspn(void **state) {
  s_tstbuf *tst = (s_tstbuf *)(*state);
  tst->impl = &tst_ft_strcspn;
  tst->ret |= strcspn_test(state);
}

void test_SIMPLE_STRCSPN(void **state) {
  s_tstbuf *tst = (s_tstbuf *)(*state);
  tst->impl = &tst_SIMPLE_STRCSPN;
  tst->ret |= strcspn_test(state);
}

void test_STUPID_STRCSPN(void **state) {
  s_tstbuf *tst = (s_tstbuf *)(*state);
  tst->impl = &tst_STUPID_STRCSPN;
  tst->ret |= strcspn_test(state);
}

void test_STRCSPN(void **state) {
  s_tstbuf *tst = (s_tstbuf *)(*state);
  tst->impl = &tst_STRCSPN;
  tst->ret |= strcspn_test(state);
}

int strcspn_tests(void) {
  const struct CMUnitTest strcspn_tests[] = {
      cmocka_unit_test(test_ft_strcspn),
      cmocka_unit_test(test_SIMPLE_STRCSPN),
      cmocka_unit_test(test_STUPID_STRCSPN),
      cmocka_unit_test(test_STRCSPN),
  };

  return cmocka_run_group_tests(strcspn_tests, test_setup, test_teardown);
}

#ifdef SINGLE_TEST
int test_main(void) { return strcspn_tests(); }
#endif
