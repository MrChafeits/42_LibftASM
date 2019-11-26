#include <string.h>
#include <sys/cdefs.h>
typedef struct {
  const char *name;
  void (*fn)(void);
  long test;
} impl_t;

#define IMPL(name, test)                                                       \
  impl_t tst_##name = {__STRING(name), (void (*)(void))name, test};

#ifdef TEST_MAIN

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/param.h>
#include <time.h>
#include <unistd.h>

typedef struct s_tstbuf {
  impl_t *impl;
  unsigned char *buf1, *buf2;
  int ret, do_srandom;
  unsigned int seed;
  size_t page_size;
} s_tstbuf;

#define TEST_FUNCTION test_main
#define TIMEOUT (4 * 60)
// static unsigned char *buf1, *buf2;
// static int ret, do_srandom;
// static unsigned int seed;
// static size_t page_size;

#ifndef ITERATIONS
static size_t iterations = 100000;
#define ITERATIONS iterations
#endif

#define CALL(impl, ...) (*(proto_t)(impl)->fn)(__VA_ARGS__)

#ifndef BUF1PAGES
#define BUF1PAGES 1
#endif

static void test_init(s_tstbuf *tst) {
  tst->page_size = 2 * getpagesize();
#ifdef MIN_PAGE_SIZE
  if (tst->page_size < MIN_PAGE_SIZE)
    tst->page_size = MIN_PAGE_SIZE;
#endif
  tst->buf1 = mmap(0, (BUF1PAGES + 1) * tst->page_size, PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANON, -1, 0);
  if (tst->buf1 == MAP_FAILED)
    errc(EXIT_FAILURE, errno, "mmap failed");
  if (mprotect(tst->buf1 + BUF1PAGES * tst->page_size, tst->page_size,
               PROT_NONE))
    errc(EXIT_FAILURE, errno, "mprotect failed");
  tst->buf2 = mmap(0, 2 * tst->page_size, PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANON, -1, 0);
  if (tst->buf2 == MAP_FAILED)
    errc(EXIT_FAILURE, errno, "mmap failed");
  if (mprotect(tst->buf2 + tst->page_size, tst->page_size, PROT_NONE))
    errc(EXIT_FAILURE, errno, "mprotect failed");
  if (tst->do_srandom) {
    printf("Setting seed to 0x%x\n", tst->seed);
    srandom(tst->seed);
  }

  memset(tst->buf1, 0xa5, BUF1PAGES * tst->page_size);
  memset(tst->buf2, 0x5a, tst->page_size);
}

static int test_setup(void **state) {
  s_tstbuf *buf = calloc(1, sizeof(*buf));
  if (buf == NULL)
    return EXIT_FAILURE;
  test_init(buf);
  *state = buf;
  return EXIT_SUCCESS;
}

static int test_teardown(void **state) {
  s_tstbuf *tmp = (s_tstbuf *)(*state);
  if (munmap(tmp->buf1, (BUF1PAGES + 1) * tmp->page_size))
    return EXIT_FAILURE;
  if (munmap(tmp->buf2, 2 * tmp->page_size))
    return EXIT_FAILURE;
  free(tmp);
  return EXIT_SUCCESS;
}

#endif
