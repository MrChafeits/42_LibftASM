// Cmocka needs these
// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
// clang-format on

#ifndef MEMCPY_RESULT
#define MEMCPY_RESULT(dst, len) dst
#define MIN_PAGE_SIZE 131072
#define TEST_MAIN
#define TEST_NAME "memcpy"
#include "test-string.h"

static char *simple_memcpy(char *, const char *, size_t);
static char *builtin_memcpy(char *, const char *, size_t);
extern void *ft_memcpy(void *, const void *, size_t);

IMPL(simple_memcpy, 0)
IMPL(builtin_memcpy, 0)
IMPL(ft_memcpy, 1)
IMPL(memcpy, 1)

static char *simple_memcpy(char *dst, const char *src, size_t n) {
  char *ret = dst;
  while (n--)
    *dst++ = *src++;
  return ret;
}

static char *builtin_memcpy(char *dst, const char *src, size_t n) {
  return __builtin_memcpy(dst, src, n);
}
#endif

typedef char *(*proto_t)(char *, const char *, size_t);

static void do_one_test(impl_t *impl, char *dst, const char *src, size_t len) {
  size_t i;

  /* Must clear the destination buffer set by the previous run.  */
  for (i = 0; i < len; i++)
    dst[i] = 0;

  assert_int_equal(CALL(impl, dst, src, len), MEMCPY_RESULT(dst, len));

  assert_memory_equal(dst, src, len);
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

  do_one_test(impl, s2, s1, len);
}

static void do_random_tests(impl_t *impl) {
  size_t i, j, n, align1, align2, len, size1, size2, size;
  int c;
  unsigned char *p1, *p2;
  unsigned char *res;

  for (n = 0; n < ITERATIONS; n++) {
    if (n == 0) {
      len = getpagesize();
      size = len + 512;
      size1 = size;
      size2 = size;
      align1 = 512;
      align2 = 512;
    } else {
      if ((random() & 255) == 0)
        size = 65536;
      else
        size = 768;
      if (size > page_size)
        size = page_size;
      size1 = size;
      size2 = size;
      i = random();
      if (i & 3)
        size -= 256;
      if (i & 1)
        size1 -= 256;
      if (i & 2)
        size2 -= 256;
      if (i & 4) {
        len = random() % size;
        align1 = size1 - len - (random() & 31);
        align2 = size2 - len - (random() & 31);
        if (align1 > size1)
          align1 = 0;
        if (align2 > size2)
          align2 = 0;
      } else {
        align1 = random() & 63;
        align2 = random() & 63;
        len = random() % size;
        if (align1 + len > size1)
          align1 = size1 - len;
        if (align2 + len > size2)
          align2 = size2 - len;
      }
    }
    p1 = buf1 + page_size - size1;
    p2 = buf2 + page_size - size2;
    c = random() & 255;
    j = align1 + len + 256;
    if (j > size1)
      j = size1;
    for (i = 0; i < j; ++i)
      p1[i] = random() & 255;

    j = align2 + len + 256;
    if (j > size2)
      j = size2;
    memset(p2, c, j);
    res = (unsigned char *)CALL(impl, (char *)(p2 + align2),
                                (char *)(p1 + align1), len);

    assert_int_equal(res, MEMCPY_RESULT(p2 + align2, len));

    for (i = 0; i < align2; ++i)
      assert_int_equal(p2[i], c);

    for (i = align2 + len; i < j; ++i)
      assert_int_equal(p2[i], c);

    assert_memory_equal(p1 + align1, p2 + align2, len);
  }
}

static void do_test1(impl_t *impl) {
  size_t size = 0x100000;
  void *large_buf;

  large_buf = mmap(NULL, size * 2 + page_size, PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANON, -1, 0);
  if (large_buf == MAP_FAILED) {
    puts("Failed to allocat large_buf, skipping do_test1");
    return;
  }

  if (mprotect(large_buf + size, page_size, PROT_NONE))
    errc(EXIT_FAILURE, errno, "mprotect failed");

  size_t arrary_size = size / sizeof(uint32_t);
  uint32_t *dest = large_buf;
  uint32_t *src = large_buf + size + page_size;
  size_t i;

  for (i = 0; i < arrary_size; i++)
    src[i] = (uint32_t)i;

  memset(dest, -1, size);
  CALL(impl, (char *)dest, (char *)src, size);
  for (i = 0; i < arrary_size; i++)
    assert_int_equal(dest[i], src[i]);

  munmap((void *)dest, size);
  munmap((void *)src, size);
}

static int memcpy_test(void **state) {
  s_tstbuf *tst = (s_tstbuf*)(*state);
  size_t i;

  test_init();

  for (i = 0; i < 18; ++i) {
    do_test(tst->impl, 0, 0, 1 << i);
    do_test(tst->impl, i, 0, 1 << i);
    do_test(tst->impl, 0, i, 1 << i);
    do_test(tst->impl, i, i, 1 << i);
  }

  for (i = 0; i < 32; ++i) {
    do_test(tst->impl, 0, 0, i);
    do_test(tst->impl, i, 0, i);
    do_test(tst->impl, 0, i, i);
    do_test(tst->impl, i, i, i);
  }

  for (i = 3; i < 32; ++i) {
    if ((i & (i - 1)) == 0)
      continue;
    do_test(tst->impl, 0, 0, 16 * i);
    do_test(tst->impl, i, 0, 16 * i);
    do_test(tst->impl, 0, i, 16 * i);
    do_test(tst->impl, i, i, 16 * i);
  }

  do_test(tst->impl, 0, 0, getpagesize());

  do_random_tests(tst->impl);

  do_test1(tst->impl);
  tst->ret = ret;
  return tst->ret;
}

static void test_ft_memcpy(void **state) {
  s_tstbuf *tst = (s_tstbuf*)(*state);
  tst->impl = &tst_ft_memcpy;
  tst->ret |= memcpy_test(state);
}

static void test_simple_memcpy(void **state) {
  s_tstbuf *tst = (s_tstbuf*)(*state);
  tst->impl = &tst_simple_memcpy;
  tst->ret |= memcpy_test(state);
}

static void test_builtin_memcpy(void **state) {
  s_tstbuf *tst = (s_tstbuf*)(*state);
  tst->impl = &tst_builtin_memcpy;
  tst->ret |= memcpy_test(state);
}

static void test_memcpy(void **state) {
  s_tstbuf *tst = (s_tstbuf*)(*state);
  tst->impl = &tst_memcpy;
  tst->ret |= memcpy_test(state);
}

int memcpy_tests(void) {
  const struct CMUnitTest memcpy_tests[] = {
    cmocka_unit_test(test_ft_memcpy),
    cmocka_unit_test(test_simple_memcpy),
    cmocka_unit_test(test_builtin_memcpy),
    cmocka_unit_test(test_memcpy),
  };

  return cmocka_run_group_tests(memcpy_tests, test_setup, test_teardown);
}

#ifdef SINGLE_TEST
int test_main(void) { return memcpy_tests(); }
#endif
