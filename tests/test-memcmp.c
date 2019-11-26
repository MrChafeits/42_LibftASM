// Cmocka needs these
// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
// clang-format on

#define TEST_MAIN
#define TEST_NAME "memcmp"
#include "test-string.h"
#include <limits.h>

#define MEMCMP memcmp
#define MEMCPY memcpy
#define SIMPLE_MEMCMP simple_memcmp
#define CHAR char
#define MAX_CHAR 255
#define UCHAR unsigned char
#define CHARBYTES 1

int simple_memcmp(const char *s1, const char *s2, size_t n) {
  int ret = 0;

  while (n-- && (ret = *(unsigned char *)s1++ - *(unsigned char *)s2++) == 0)
    ;
  return ret;
}

int builtin_memcmp(const void *s1, const void *s2, size_t n) {
  return __builtin_memcmp(s1, s2, n);
}

typedef int (*proto_t)(const CHAR *, const CHAR *, size_t);
extern int ft_memcmp(const void *, const void *, size_t);

IMPL(ft_memcmp, 0)
IMPL(SIMPLE_MEMCMP, 0)
IMPL(builtin_memcmp, 0)
IMPL(MEMCMP, 1)

static int check_result(impl_t *impl, const CHAR *s1, const CHAR *s2,
                        size_t len, int exp_result) {
  int result = CALL(impl, s1, s2, len);
  assert_false((exp_result == 0 && result != 0) ||
               (exp_result < 0 && result >= 0) ||
               (exp_result > 0 && result <= 0));

  return 0;
}

static void do_one_test(impl_t *impl, const CHAR *s1, const CHAR *s2,
                        size_t len, int exp_result) {
  if (check_result(impl, s1, s2, len, exp_result) < 0)
    return;
}

static void do_test(impl_t *impl, size_t align1, size_t align2, size_t len,
                    int exp_result) {
  size_t i;
  CHAR *s1, *s2;

  if (len == 0)
    return;

  align1 &= 63;
  if (align1 + (len + 1) * CHARBYTES >= page_size)
    return;

  align2 &= 63;
  if (align2 + (len + 1) * CHARBYTES >= page_size)
    return;

  s1 = (CHAR *)(buf1 + align1);
  s2 = (CHAR *)(buf2 + align2);

  for (i = 0; i < len; i++)
    s1[i] = s2[i] = 1 + (23 << ((CHARBYTES - 1) * 8)) * i % CHAR_MAX;

  s1[len] = align1;
  s2[len] = align2;
  s2[len - 1] -= exp_result;

  do_one_test(impl, s1, s2, len, exp_result);
}

static void do_random_tests(impl_t *impl) {
  size_t i, j, n, align1, align2, pos, len;
  int result;
  long r;
  UCHAR *p1 = (UCHAR *)(buf1 + page_size - 512 * CHARBYTES);
  UCHAR *p2 = (UCHAR *)(buf2 + page_size - 512 * CHARBYTES);

  for (n = 0; n < ITERATIONS; n++) {
    align1 = random() & 31;
    if (random() & 1)
      align2 = random() & 31;
    else
      align2 = align1 + (random() & 24);
    pos = random() & 511;
    j = align1;
    if (align2 > j)
      j = align2;
    if (pos + j >= 512)
      pos = 511 - j - (random() & 7);
    len = random() & 511;
    if (len + j >= 512)
      len = 511 - j - (random() & 7);
    j = len + align1 + 64;
    if (j > 512)
      j = 512;
    for (i = 0; i < j; ++i)
      p1[i] = random() & 255;
    for (i = 0; i < j; ++i)
      p2[i] = random() & 255;

    result = 0;
    if (pos >= len)
      MEMCPY((CHAR *)p2 + align2, (const CHAR *)p1 + align1, len);
    else {
      MEMCPY((CHAR *)p2 + align2, (const CHAR *)p1 + align1, pos);
      if (p2[align2 + pos] == p1[align1 + pos]) {
        p2[align2 + pos] = random() & 255;
        if (p2[align2 + pos] == p1[align1 + pos])
          p2[align2 + pos] = p1[align1 + pos] + 3 + (random() & 127);
      }

      if (p1[align1 + pos] < p2[align2 + pos])
        result = -1;
      else
        result = 1;
    }

    r = CALL(impl, (CHAR *)p1 + align1, (const CHAR *)p2 + align2, len);
    assert_false((r == 0 && result) || (r < 0 && result >= 0) ||
                 (r > 0 && result <= 0));
  }
}

static void check1(impl_t *impl) {
  CHAR s1[116] = {
      0x94, 0x63, 0x8f, 0x01, 0x74, 0x63, 0x8f, 0x01, 0x54, 0x63, 0x8f, 0x01,
      0x34, 0x63, 0x8f, 0x01, 0xb4, 0xf2, 0x93, 0x01, 0x94, 0xf2, 0x93, 0x01,
      0x54, 0xf1, 0x93, 0x01, 0x34, 0xf1, 0x93, 0x01, 0x14, 0xf1, 0x93, 0x01,
      0x14, 0xf2, 0x93, 0x01, 0x34, 0xf2, 0x93, 0x01, 0x54, 0xf2, 0x93, 0x01,
      0x74, 0xf2, 0x93, 0x01, 0x74, 0xf1, 0x93, 0x01, 0xd4, 0xf2, 0x93, 0x01,
      0x94, 0xf1, 0x93, 0x01, 0xb4, 0xf1, 0x93, 0x01, 0xd4, 0xf1, 0x93, 0x01,
      0xf4, 0xf1, 0x93, 0x01, 0xf4, 0xf2, 0x93, 0x01, 0xbc, 0x40, 0x96, 0x01,
      0xf4, 0xf3, 0x93, 0x01, 0xd4, 0xf3, 0x93, 0x01, 0xb4, 0xf3, 0x93, 0x01,
      0x94, 0xf3, 0x93, 0x01, 0x74, 0x7f, 0x93, 0x01, 0x54, 0xf3, 0x93, 0x01,
      0x34, 0xf3, 0x93, 0x01, 0x80, 0xf3, 0x93, 0x01};
  CHAR s2[116] = {
      0x94, 0x63, 0x8f, 0x01, 0x74, 0x63, 0x8f, 0x01, 0x54, 0x63, 0x8f, 0x01,
      0x34, 0x63, 0x8f, 0x01, 0xb4, 0xf2, 0x93, 0x01, 0x94, 0xf2, 0x93, 0x01,
      0x54, 0xf1, 0x93, 0x01, 0x34, 0xf1, 0x93, 0x01, 0x14, 0xf1, 0x93, 0x01,
      0x14, 0xf2, 0x93, 0x01, 0x34, 0xf2, 0x93, 0x01, 0x54, 0xf2, 0x93, 0x01,
      0x74, 0xf2, 0x93, 0x01, 0x74, 0xf1, 0x93, 0x01, 0xd4, 0xf2, 0x93, 0x01,
      0x94, 0xf1, 0x93, 0x01, 0xb4, 0xf1, 0x93, 0x01, 0xd4, 0xf1, 0x93, 0x01,
      0xf4, 0xf1, 0x93, 0x01, 0xf4, 0xf2, 0x93, 0x01, 0xbc, 0x40, 0x96, 0x01,
      0xf4, 0xf3, 0x93, 0x01, 0xd4, 0xf3, 0x93, 0x01, 0xb4, 0xf3, 0x93, 0x01,
      0x94, 0xf3, 0x93, 0x01, 0x74, 0x7f, 0x93, 0x01, 0x54, 0xf3, 0x93, 0x01,
      0x34, 0xf3, 0x93, 0x01, 0x80, 0xf3, 0x93, 0x01};
  size_t n, exp_result;

  n = 116;
  for (size_t i = 0; i < n; i++)
    for (size_t len = 0; len <= n - i; ++len) {
      exp_result = SIMPLE_MEMCMP(s1 + i, s2 + i, len);
      check_result(impl, s1 + i, s2 + i, len, exp_result);
    }
}

/* This test checks that memcmp doesn't overrun buffers.  */
static void check2(impl_t *impl) {
  size_t max_length = page_size / sizeof(CHAR);

  /* Initialize buf2 to the same values as buf1.  The bug requires the
     last compared byte to be different.  */
  memcpy(buf2, buf1, page_size);
  ((char *)buf2)[page_size - 1] ^= 0x11;

  for (size_t length = 1; length < max_length; length++) {
    CHAR *s1 = (CHAR *)buf1 + max_length - length;
    CHAR *s2 = (CHAR *)buf2 + max_length - length;

    const int exp_result = SIMPLE_MEMCMP(s1, s2, length);

    check_result(impl, s1, s2, length, exp_result);
  }
}

int memcmp_test(void **state) {
  s_tstbuf *tst = (s_tstbuf *)(*state);
  size_t i;

  check1(tst->impl);
  check2(tst->impl);

  for (i = 1; i < 16; ++i) {
    do_test(tst->impl, i * CHARBYTES, i * CHARBYTES, i, 0);
    do_test(tst->impl, i * CHARBYTES, i * CHARBYTES, i, 1);
    do_test(tst->impl, i * CHARBYTES, i * CHARBYTES, i, -1);
  }

  for (i = 0; i < 16; ++i) {
    do_test(tst->impl, 0, 0, i, 0);
    do_test(tst->impl, 0, 0, i, 1);
    do_test(tst->impl, 0, 0, i, -1);
  }

  for (i = 1; i < 10; ++i) {
    do_test(tst->impl, 0, 0, 2 << i, 0);
    do_test(tst->impl, 0, 0, 2 << i, 1);
    do_test(tst->impl, 0, 0, 2 << i, -1);
    do_test(tst->impl, 0, 0, 16 << i, 0);
    do_test(tst->impl, (8 - i) * CHARBYTES, (2 * i) * CHARBYTES, 16 << i, 0);
    do_test(tst->impl, 0, 0, 16 << i, 1);
    do_test(tst->impl, 0, 0, 16 << i, -1);
  }

  for (i = 1; i < 8; ++i) {
    do_test(tst->impl, i * CHARBYTES, 2 * (i * CHARBYTES), 8 << i, 0);
    do_test(tst->impl, i * CHARBYTES, 2 * (i * CHARBYTES), 8 << i, 1);
    do_test(tst->impl, i * CHARBYTES, 2 * (i * CHARBYTES), 8 << i, -1);
  }

  do_random_tests(tst->impl);
  tst->ret = ret;
  return tst->ret;
}

int test_setup(void **state) {
  test_init();
  s_tstbuf *buf = calloc(1, sizeof(*buf));
  if (buf == NULL)
    return 1;
  buf->buf1 = buf1;
  buf->buf2 = buf2;
  buf->do_srandom = do_srandom;
  buf->page_size = page_size;
  buf->ret = ret = 0;
  buf->seed = seed;
  *state = buf;
  return 0;
}

int test_teardown(void **state) {
  s_tstbuf *tmp = (s_tstbuf *)(*state);
  if (munmap(tmp->buf1, (BUF1PAGES + 1) * page_size))
    return 1;
  if (munmap(tmp->buf2, 2 * page_size))
    return 1;
  free(tmp);
  return 0;
}

void test_ft_memcmp(void **state) {
  s_tstbuf *tst = (s_tstbuf *)(*state);
  tst->impl = &tst_ft_memcmp;
  tst->ret |= memcmp_test(state);
}

void test_simple_memcmp(void **state) {
  s_tstbuf *tst = (s_tstbuf *)(*state);
  tst->impl = &tst_SIMPLE_MEMCMP;
  tst->ret |= memcmp_test(state);
}

void test_builtin_memcmp(void **state) {
  s_tstbuf *tst = (s_tstbuf *)(*state);
  tst->impl = &tst_builtin_memcmp;
  tst->ret |= memcmp_test(state);
}

void test_memcmp(void **state) {
  s_tstbuf *tst = (s_tstbuf *)(*state);
  tst->impl = &tst_MEMCMP;
  tst->ret |= memcmp_test(state);
}

int memcmp_tests(void) {
  const struct CMUnitTest memcmp_tests[] = {
      cmocka_unit_test(test_ft_memcmp),
      cmocka_unit_test(test_simple_memcmp),
      cmocka_unit_test(test_builtin_memcmp),
      cmocka_unit_test(test_memcmp),
  };

  return cmocka_run_group_tests(memcmp_tests, test_setup, test_teardown);
}

#ifdef SINGLE_TEST
int test_main(void) { return memcmp_tests(); }
#endif
