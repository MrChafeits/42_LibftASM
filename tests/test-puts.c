#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <err.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/stat.h>

// Cmocka needs these
// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
// clang-format on

#pragma mark - Declarations -

extern int ft_puts(const char *s);
extern void ft_cat(int fd);

#pragma mark - Private Functions -

unsigned char chk[64];
char buf[] = "testing ft_puts output \n \t \v \b \e \a\xa5\x5a\x90\x00should't see this";
size_t bl = 37;

static void puts_test(void **state __unused) {
	char outfile[] = "puts_stdout.txt";
	fpos_t pos;
	FILE *fp;
	int fd;

	fd = dup(fileno(stdout));
	fgetpos(stdout, &pos);
	freopen(outfile, "w+", stdout);
	ft_puts(buf);
	fflush(stdout);
	dup2(fd, fileno(stdout));
	close(fd);
	clearerr(stdout);
	fsetpos(stdout, &pos);
	chmod(outfile, 0644);
	fp = fopen(outfile, "r+");
	assert_true(fp != NULL);
	int c, ii=0;

	do {
		chk[ii++] = c = getc(fp);
	} while (c != EOF);
	assert_int_equal(memcmp(buf, chk, bl), 0);
	assert_int_equal(chk[bl], '\n');
	assert_int_equal(fclose(fp), 0);
}

static void cat_test(void **state __unused) {
	char outfile[] = "cat_stdout.txt";
	fpos_t pos;
	FILE *fpin, *fpout;
	int in, out, fd, fud;

	fud = open(__FILE__, O_RDONLY);
	fd = dup(fileno(stdout));
	fgetpos(stdout, &pos);
	freopen(outfile, "w+", stdout);
	ft_cat(fud);
	fflush(stdout);
	assert_int_equal(close(fud), 0);
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
}

#pragma mark - Public Functions -

int puts_tests(void)
{
	const struct CMUnitTest puts_tests[] = {
		cmocka_unit_test(puts_test),
		cmocka_unit_test(cat_test),
	};

	return cmocka_run_group_tests(puts_tests, NULL, NULL);
}

int test_main(void)
{
	return puts_tests();
}
