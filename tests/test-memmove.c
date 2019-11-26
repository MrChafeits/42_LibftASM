// Cmocka needs these
// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
// clang-format on

#define TEST_MAIN
#define TEST_NAME "memmove"
#include "test-string.h"

extern void *ft_memmove(void *, const void *, size_t);

char *simple_memmove(char *, const char *, size_t);

typedef char *(*proto_t)(char *, const char *, size_t);

IMPL(simple_memmove, 0)
IMPL(ft_memmove, 0)
IMPL(memmove, 0)

char *simple_memmove(char *dst, const char *src, size_t n) {
  char *ret = dst;
  if (src < dst) {
    dst += n;
    src += n;
    while (n--)
      *--dst = *--src;
  } else
    while (n--)
      *dst++ = *src++;
  return ret;
}

static void do_one_test(impl_t *impl, char *dst, char *src,
                        const char *orig_src, size_t len) {
  /* This also clears the destination buffer set by the previous run.  */
  memcpy(src, orig_src, len);
  char *res;

  res = CALL(impl, dst, src, len);
  assert_int_equal(res, dst);

  assert_memory_equal(dst, orig_src, len);
}

static void do_test(impl_t *impl, size_t align1, size_t align2, size_t len) {
  size_t i, j;
  char *s1, *s2;

  align1 &= 63;
  if (align1 + len >= page_size)
    return;

  align2 &= 63;
  if (align2 + len >= page_size)
    return;

  s1 = (char *)(buf1 + align1);
  s2 = (char *)(buf2 + align2);

  for (i = 0, j = 1; i < len; i++, j += 23)
    s1[i] = j;

  do_one_test(impl, s2, (char *)(buf2 + align1), s1, len);
}

static void do_random_tests(impl_t *impl) {
  size_t i, n, align1, align2, len, size;
  size_t srcstart, srcend, dststart, dstend;
  int c;
  unsigned char *p1, *p2;
  unsigned char *res;

  for (n = 0; n < ITERATIONS; n++) {
    if ((random() & 255) == 0)
      size = 65536;
    else
      size = 512;
    if (size > page_size)
      size = page_size;
    if ((random() & 3) == 0) {
      len = random() & (size - 1);
      align1 = size - len - (random() & 31);
      align2 = size - len - (random() & 31);
      if (align1 > size)
        align1 = 0;
      if (align2 > size)
        align2 = 0;
    } else {
      align1 = random() & (size / 2 - 1);
      align2 = random() & (size / 2 - 1);
      len = random() & (size - 1);
      if (align1 + len > size)
        align1 = size - len;
      if (align2 + len > size)
        align2 = size - len;
    }

    p1 = buf1 + page_size - size;
    p2 = buf2 + page_size - size;
    c = random() & 255;
    srcend = align1 + len + 256;
    if (srcend > size)
      srcend = size;
    if (align1 > 256)
      srcstart = align1 - 256;
    else
      srcstart = 0;
    for (i = srcstart; i < srcend; ++i)
      p1[i] = random() & 255;
    dstend = align2 + len + 256;
    if (dstend > size)
      dstend = size;
    if (align2 > 256)
      dststart = align2 - 256;
    else
      dststart = 0;

    memset(p2 + dststart, c, dstend - dststart);
    memcpy(p2 + srcstart, p1 + srcstart, srcend - srcstart);
    res = (unsigned char *)CALL(impl, (char *)(p2 + align2),
                                (char *)(p2 + align1), len);
    assert_int_equal(res, p2 + align2);

    assert_memory_equal(p1 + align1, p2 + align2, len);

    for (i = dststart; i < dstend; ++i) {
      if (i >= align2 && i < align2 + len) {
        i = align2 + len - 1;
        continue;
      }
      if (i >= srcstart && i < srcend) {
        i = srcend - 1;
        continue;
      }
      assert_int_equal(p2[i], c);
    }
    if (srcstart < align2)
      assert_memory_equal(p2 + srcstart, p1 + srcstart,
                          (srcend > align2 ? align2 : srcend) - srcstart);

    i = srcstart > align2 + len ? srcstart : align2 + len;
    if (srcend > align2 + len)
      assert_memory_equal(p2 + i, p1 + i, srcend - i);
  }
}

static void do_test2(impl_t *impl) {
  size_t size = 0x20000000;
  uint32_t *large_buf;
  large_buf = mmap((void *)0x100000000, size, PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANON, -1, 0);

  if (large_buf == MAP_FAILED) {
#ifdef DEBUG
    fputs("Large mmap failed", stderr);
#endif
    ret = 1;
    assert_false(ret);
    return;
  }

  if ((uintptr_t)large_buf > 0x120000000 - 128 ||
      0x120000000 - (uintptr_t)large_buf > 0x20000000) {
#ifdef DEBUG
    fputs("Large mmap allocated improperly", stderr);
#endif
    ret = 1;
    munmap((void *)large_buf, size);
    assert_false(ret);
    return;
  }

  size_t bytes_move = 0x120000000 - (uintptr_t)large_buf;
  size_t arr_size = bytes_move / sizeof(uint32_t);
  size_t i;

  for (i = 0; i < arr_size; i++)
    large_buf[i] = (uint32_t)i;

  uint32_t *dst = &large_buf[33];

  CALL(impl, (char *)dst, (char *)large_buf, bytes_move);

  for (i = 0; i < arr_size; i++) {
    assert_int_equal(dst[i], (uint32_t)i);
  }

  munmap((void *)large_buf, size);
}

int memmove_test(void **state) {
  s_tstbuf *tst = (s_tstbuf *)(*state);
  size_t i;

  for (i = 0; i < 14; ++i) {
    do_test(tst->impl, 0, 32, 1 << i);
    do_test(tst->impl, 32, 0, 1 << i);
    do_test(tst->impl, 0, i, 1 << i);
    do_test(tst->impl, i, 0, 1 << i);
  }

  for (i = 0; i < 32; ++i) {
    do_test(tst->impl, 0, 32, i);
    do_test(tst->impl, 32, 0, i);
    do_test(tst->impl, 0, i, i);
    do_test(tst->impl, i, 0, i);
  }

  for (i = 3; i < 32; ++i) {
    if ((i & (i - 1)) == 0)
      continue;
    do_test(tst->impl, 0, 32, 16 * i);
    do_test(tst->impl, 32, 0, 16 * i);
    do_test(tst->impl, 0, i, 16 * i);
    do_test(tst->impl, i, 0, 16 * i);
  }

  do_random_tests(tst->impl);

  do_test2(tst->impl);
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

void test_ft_memmove(void **state) {
  s_tstbuf *tst = (s_tstbuf *)(*state);
  tst->impl = &tst_ft_memmove;
  tst->ret |= memmove_test(state);
}

void test_simple_memmove(void **state) {
  s_tstbuf *tst = (s_tstbuf *)(*state);
  tst->impl = &tst_simple_memmove;
  tst->ret |= memmove_test(state);
}

void test_memmove(void **state) {
  s_tstbuf *tst = (s_tstbuf *)(*state);
  tst->impl = &tst_memmove;
  tst->ret |= memmove_test(state);
}

int memmove_tests(void) {
  const struct CMUnitTest memmove_tests[] = {
      cmocka_unit_test(test_ft_memmove),
      cmocka_unit_test(test_simple_memmove),
      cmocka_unit_test(test_memmove),
  };

  return cmocka_run_group_tests(memmove_tests, test_setup, test_teardown);
}

#ifdef SINGLE_TEST
int test_main(void) { return memmove_tests(); }
#endif
