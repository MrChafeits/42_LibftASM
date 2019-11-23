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
/* TODO: rewrite this to be more cmocka-friendly */

extern void *ft_memmove(void *, const void *, size_t);
extern void *A_memmove(void *, const void *, size_t);

char *simple_memmove(char *, const char *, size_t);

typedef char *(*proto_t)(char *, const char *, size_t);

IMPL(simple_memmove, 0)
IMPL(ft_memmove, 0)
IMPL(A_memmove, 0)
IMPL(memmove, 0)

char *inhibit_loop_to_libcall simple_memmove(char *dst, const char *src,
                                             size_t n) {
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

  // res = CALL(impl, dst, src, len);
  res = (*(proto_t)(impl)->fn)(dst, src, len);
  #ifndef DEBUG
  assert_int_equal(res, dst);
  #else
  if (res != dst) {
    fprintf(stderr, "Wrong result in function %s %p %p", impl->name,
	     res, dst);
      ret = 1;
      return;
  }
  #endif

  #ifndef DEBUG
  assert_int_equal(memcmp(dst, orig_src, len), 0);
  #else
  if (memcmp(dst, orig_src, len) !=  0) {
    fprintf(stderr, "Wrong result in function %s dst \"%s\" src \"%s\"",
	     impl->name, dst, src);
      ret = 1;
      return;
  }
  #endif
}

static void do_test(size_t align1, size_t align2, size_t len) {
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
  // func_count = 3;
  FOR_EACH_IMPL(impl, 0) {
      do_one_test(impl, s2, (char *)(buf2 + align1), s1, len);
  }
}

static void do_random_tests(void) {
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

    FOR_EACH_IMPL(impl, 1) {
      memset(p2 + dststart, c, dstend - dststart);
      memcpy(p2 + srcstart, p1 + srcstart, srcend - srcstart);
      res = (unsigned char *)CALL(impl, (char *)(p2 + align2),
                                  (char *)(p2 + align1), len);
      #ifndef DEBUG
      assert_int_equal(res, p2 + align2);
      #else
      if (res != p2 + align2) {
        fprintf(stderr, "Iteration %zd - wrong result in function %s (%zd, %zd, %zd) %p != %p",
		     n, impl->name, align1, align2, len, res, p2 + align2);
	      ret = 1;
      }
      #endif

      #ifndef DEBUG
      assert_int_equal(memcmp(p1 + align1, p2 + align2, len), 0);
      #else
      if (memcmp(p1 + align1, p2 + align2, len) != 0) {
        fprintf(stderr, "Iteration %zd - different strings, %s (%zd, %zd, %zd)",
		     n, impl->name, align1, align2, len);
	      ret = 1;
      }
      #endif

      for (i = dststart; i < dstend; ++i) {
        if (i >= align2 && i < align2 + len) {
          i = align2 + len - 1;
          continue;
        }
        if (i >= srcstart && i < srcend) {
          i = srcend - 1;
          continue;
        }
        #ifndef DEBUG
        assert_int_equal(p2[i], c);
        #else
        if (p2[i] != c) {
          fprintf(stderr, "Iteration %zd - garbage in memset area, %s (%zd, %zd, %zd)",
            n, impl->name, align1, align2, len);
          ret = 1;
		      break;
        }
        #endif
      }
      #ifndef DEBUG
      assert_false(srcstart < align2 && memcmp(p2 + srcstart, p1 + srcstart, (srcend > align2 ? align2 : srcend) - srcstart));
      #else
      if (srcstart < align2 && memcmp(p2 + srcstart, p1 + srcstart, (srcend > align2 ? align2 : srcend) - srcstart)) {
        fprintf(stderr, "Iteration %zd - garbage before dst, %s (%zd, %zd, %zd)",
		     n, impl->name, align1, align2, len);
	      ret = 1;
	      break;
      }
      #endif

      i = srcstart > align2 + len ? srcstart : align2 + len;
      #ifndef DEBUG
      assert_false(srcend > align2 + len && memcmp(p2 + i, p1 + i, srcend - i));
      #else
      if (srcend > align2 + len && memcmp(p2 + i, p1 + i, srcend - i)) {
        fprintf(stderr, "Iteration %zd - garbage after dst, %s (%zd, %zd, %zd)",
		     n, impl->name, align1, align2, len);
	      ret = 1;
	      break;
      }
      #endif
    }
  }
}

static void do_test2(void) {
  size_t size = 0x20000000;
  uint32_t *large_buf;
//                         0x120000000
//                         0x10d66f000
  large_buf = mmap((void *)0x100000000, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
  // large_buf = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);

  if (large_buf == MAP_FAILED) {
    fputs("Large mmap failed", stderr);
    ret = 1;
    #ifndef DEBUG
    assert_int_not_equal(large_buf, MAP_FAILED);
    #endif
    return;
  }

  if ((uintptr_t)large_buf > 0x120000000 - 128 ||
      0x120000000 - (uintptr_t)large_buf > 0x20000000) {
    fputs("Large mmap allocated improperly", stderr);
    ret = 1;
    munmap((void *)large_buf, size);
    #ifndef DEBUG
    assert_false((uintptr_t)large_buf>0x120000000-128||0x120000000-(uintptr_t)large_buf>0x20000000);
    #endif
    return;
  }

  size_t bytes_move = 0x120000000 - (uintptr_t)large_buf;
  size_t arr_size = bytes_move / sizeof(uint32_t);
  size_t i;

  FOR_EACH_IMPL(impl, 0) {
    for (i = 0; i < arr_size; i++)
      large_buf[i] = (uint32_t)i;

    uint32_t *dst = &large_buf[33];

    CALL(impl, (char *)dst, (char *)large_buf, bytes_move);

    for (i = 0; i < arr_size; i++) {
      #ifndef DEBUG
      assert_int_equal(dst[i], (uint32_t)i);
      #else
      if (dst[i] != (uint32_t)i) {
        fprintf(stderr, "Wrong result in function %s dst \"%p\" src \"%p\" offset \"%zd\"",
		     impl->name, dst, large_buf, i);
	      ret = 1;
	      break;
      }
      #endif
    }
  }

  munmap((void *)large_buf, size);
}

void test_group_one(void **state __unused) {
  for (size_t i = 0; i < 14; ++i) {
    do_test(0, 32, 1 << i);
    do_test(32, 0, 1 << i);
    do_test(0, i, 1 << i);
    do_test(i, 0, 1 << i);
  }
}

void test_group_two(void **state __unused) {
  for (size_t i = 0; i < 32; ++i) {
    do_test(0, 32, i);
    do_test(32, 0, i);
    do_test(0, i, i);
    do_test(i, 0, i);
  }
}

void test_group_three(void **state __unused) {
  for (size_t i = 3; i < 32; ++i) {
    if ((i & (i - 1)) == 0)
      continue;
    do_test(0, 32, 16 * i);
    do_test(32, 0, 16 * i);
    do_test(0, i, 16 * i);
    do_test(i, 0, 16 * i);
  }
}

void test_group_four(void **state __unused) {
  do_random_tests();
}

void test_group_five(void **state __unused) {
  do_test2();
}

int test_deinit(void **state __unused) {
  int ret;
  ret = munmap(buf1, (BUF1PAGES+1)*page_size);
  ret |= munmap(buf2, 2 * page_size);
  return ret;
}
int memmove_tests(void) {
  impl_t *fbf = calloc(5, sizeof(*fbf));
  fbf[0] = tst_A_memmove;
  fbf[1] = tst_ft_memmove;
  fbf[2] = tst_simple_memmove;
  fbf[3] = tst_memmove;
  fbf[4] = tst_memmove;

  __start_impls = fbf;

  __stop_impls = fbf+3;

  test_init();

  #ifndef DEBUG
  const struct CMUnitTest memmove_tests[] = {
    cmocka_unit_test(test_group_one),
    cmocka_unit_test(test_group_two),
    cmocka_unit_test(test_group_three),
    cmocka_unit_test(test_group_four),
    cmocka_unit_test(test_group_five),
  };
  ret |= cmocka_run_group_tests(memmove_tests, NULL, NULL);
  #else
  test_group_one(0);
  test_group_two(0);
  test_group_three(0);
  test_group_four(0);
  test_group_five(0);
  #endif
  test_deinit(0);
  return ret;
}

#ifdef SINGLE_TEST
int test_main(void) {
  return memmove_tests();
}
#endif
