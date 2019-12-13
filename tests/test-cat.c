#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// Cmocka needs these
// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
// clang-format on

typedef struct {
  const char *name;
  void (*fn)(void);
  long test;
} impl_t;

typedef struct s_tstbuf {
  impl_t *impl;
  char *filename;
  int do_fd, fd;
  int ret;
  size_t page_size;
} t_tstbuf;

typedef void (*proto_t)(int);

#define MAX(a, b)	(((a) > (b)) ? (a) : (b))
#define CALL(impl, ...) (*(proto_t)(impl)->fn)(__VA_ARGS__)
#define IMPL(name, test)                                                       \
  impl_t tst_##name = {__STRING(name), (void (*)(void))name, test};

extern void ft_cat(int fd);

static void raw_cat(int rfd) {
  int off, wfd;
  ssize_t nr, nw;
  static size_t bsize;
  static char *buf = NULL;
  struct stat sbuf;

  wfd = fileno(stdout);
  if (buf == NULL) {
    if (fstat(wfd, &sbuf), 0)
      return;
    bsize = MAX(sbuf.st_blksize, 1024);
    buf = malloc(bsize);
    if (buf == NULL)
      return;
  }
  while ((nr = read(rfd, buf, bsize)) > 0)
    for (off = 0; nr; nr -= nw, off += nw)
      if ((nw = write(wfd, buf + off, (size_t)nr)) < 0)
        return;
  if (nr < 0)
    return;
}

IMPL(ft_cat, 0);
IMPL(raw_cat, 0);

static int cat_test(void **state) {
  t_tstbuf *buf = (t_tstbuf*)(*state);
  char outfile[] = "cat_stdout.txt";
  fpos_t pos;
  FILE *fpin, *fpout;
  int in, out, fd, fud;

  if (buf->do_fd == 0)
    fud = open(buf->filename, O_RDONLY);
  else
    fud = buf->fd;
  fd = dup(fileno(stdout));
  fgetpos(stdout, &pos);
  freopen(outfile, "w+", stdout);

  CALL(buf->impl, fud);

  fflush(stdout);
  if (fud >= 0) {
    assert_int_equal(close(fud), 0);
  } else {
    assert_int_equal(close(fud), -1);
  }
  dup2(fd, fileno(stdout));
  close(fd);

  clearerr(stdout);
  fsetpos(stdout, &pos);
  chmod(outfile, 0644);
  fpout = fopen(outfile, "r+");
  fpin = fopen(__FILE__, "r+");
  assert_true(fpout != NULL && fpin != NULL);

  do {
    in = getc(fpin), out = getc(fpout);
  } while (in != EOF && out != EOF && in == out);

  assert_int_equal(fclose(fpout), 0);
  assert_int_equal(fclose(fpin), 0);
  return 0;
}

void test_raw_cat_basic(void **state) {
  t_tstbuf *tst = (t_tstbuf *)(*state);
  tst->filename = __FILE__;
  tst->do_fd = 0;
  tst->impl = &tst_raw_cat;
  tst->ret |= cat_test(state);
}

void test_raw_cat_bin(void **state) {
  t_tstbuf *tst = (t_tstbuf *)(*state);
  tst->filename = "/bin/sleep";
  tst->do_fd = 0;
  tst->impl = &tst_raw_cat;
  tst->ret |= cat_test(state);
}

void test_raw_cat_badfd(void **state) {
  t_tstbuf *tst = (t_tstbuf *)(*state);
  tst->do_fd = 1;
  tst->fd = -42;
  tst->impl = &tst_raw_cat;
  tst->ret |= cat_test(state);
}

void test_ft_cat_basic(void **state) {
  t_tstbuf *tst = (t_tstbuf *)(*state);
  tst->filename = __FILE__;
  tst->do_fd = 0;
  tst->impl = &tst_ft_cat;
  tst->ret |= cat_test(state);
}

void test_ft_cat_bin(void **state) {
  t_tstbuf *tst = (t_tstbuf *)(*state);
  tst->filename = "/bin/sleep";
  tst->do_fd = 0;
  tst->impl = &tst_ft_cat;
  tst->ret |= cat_test(state);
}

void test_ft_cat_badfd(void **state) {
  t_tstbuf *tst = (t_tstbuf *)(*state);
  tst->do_fd = 1;
  tst->fd = -42;
  tst->impl = &tst_ft_cat;
  tst->ret |= cat_test(state);
}

static int test_setup(void **state) {
  t_tstbuf *buf = calloc(1, sizeof(*buf));
  if (buf == NULL)
    return EXIT_FAILURE;
  *state = buf;
  return EXIT_SUCCESS;
}

static int test_teardown(void **state) {
  t_tstbuf *tmp = (t_tstbuf *)(*state);
  free(tmp);
  return EXIT_SUCCESS;
}

int cat_tests(void) {
  const struct CMUnitTest cat_tests[] = {
      cmocka_unit_test(test_ft_cat_basic),
      cmocka_unit_test(test_raw_cat_basic),
      cmocka_unit_test(test_ft_cat_bin),
      cmocka_unit_test(test_raw_cat_bin),
      cmocka_unit_test(test_ft_cat_badfd),
      cmocka_unit_test(test_raw_cat_badfd),
  };

  return cmocka_run_group_tests(cat_tests, test_setup, test_teardown);
}

#ifdef SINGLE_TEST
int test_main(void) { return cat_tests(); }
#endif
