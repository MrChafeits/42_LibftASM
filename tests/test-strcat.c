#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <limits.h>
#include <sys/mman.h>

extern unsigned char *buf1, *buf2;
extern int ret, do_srandom;
extern unsigned int seed;
extern size_t page_size;
#define ITERATIONS 100000

//! Faster versions utilizing AVX instructions
extern char *A_strcat(char *restrict s1, const char *restrict s2);

extern char *ft_strcat(char *restrict s1, const char *restrict s2);

#define BIG_CHAR CHAR_MAX
#define SMALL_CHAR 127

static void
do_one_test(char *dst, const char *src)
{
	size_t k = strlen(dst);
	if (ft_strcat(dst, src) != dst) {
		printf("Wrong result in function ft_strcat %p %p", ft_strcat(dst, src), dst);
		ret = 1;
		return;
	}

	if (strcmp(dst + k, src) != 0) {
		printf("Wrong result in function ft_strcat dst \"%s\" src \"%s\"", dst, src);
		ret = 1;
		return;
	}
}

static void
do_test(size_t align1, size_t align2, size_t len1, size_t len2, int max_char)
{
	size_t i;
	char *s1, *s2;

	align1 &= 7;
	if ((align1 + len1) * sizeof(char) >= page_size)
		return;

	align2 &= 7;
	if ((align2 + len1 + len2) * sizeof(char) >= page_size)
		return;

	s1 = (char*)(buf1) + align1;
	s2 = (char*)(buf2) + align2;

	for (i = 0; i < len1; ++i)
		s1[i] = 32 + 23 * i % (max_char - 32);
	s1[len1] = '\0';

	for (i = 0; i < len2; i++)
		s2[i] = 32 + 23 * i % (max_char - 32);

	s2[len2] = '\0';
	do_one_test(s2, s1);
}

static void
do_random_test(void)
{
	size_t i, j, n, align1, align2, len1, len2;
	unsigned char *p1 = (unsigned char*)(buf1 + page_size) - 512;
	unsigned char *p2 = (unsigned char*)(buf2 + page_size) - 512;
	unsigned char *p3 = (unsigned char*)buf1;
	unsigned char *res;

	for (n = 0; n < ITERATIONS; n++) {
		align1 = random() & 31;
		if (random() & 1)
			align2 = random() & 31;
		else
			align2 = align1 + (random() & 24);
		len1 = random() & 511;
		if (len1 + align2 > 512)
			len2 = random() & 7;
		else
			len2 = (512 - len1 - align2) * (random() & (1024 * 1024 - 1)) / (1024 * 1024);
		j = align1;
		if (align2 + len2 > j)
			j = align2 + len2;
		if (len1 + j >= 511)
			len1 = 510 - j - (random() & 7);
		if (len1 >= 512)
			len1 = 0;
		if (align1 + len1 < 512 - 8) {
			j = 510 - align1 - len1 - (random() & 31);
			if (j > 0 && j < 512)
				align1 += j;
		}
		j = len1 + align1 + 64;
		if (j > 512)
			j = 512;
		for (i =0; i < j; i++) {
			if (i == len1 + align1)
				p1[i] = 0;
			else {
				p1[i] = random() & BIG_CHAR;
				if (i >= align1 && i < len1 + align1 && !p1[i])
					p1[i] = (random() & SMALL_CHAR) + 3;
			}
		}
		for (i = 0; i < len2; i++) {
			p3[i] = random() & BIG_CHAR;
			if (!p3[i])
				p3[i] = (random() & SMALL_CHAR) + 3;
		}
		p3[len2] = 0;


		{
			memset(p2 - 64, '\1', align2 + 64);
			memset(p2 + align2 + len2 + 1, '\1', 512 - align2 - len2 - 1);
			memcpy(p2 + align2, p3, len2 + 1);
			res = (unsigned char*)ft_strcat((char*)(p2 + align2), (char*)(p1 + align1));
			if (res != p2 + align2) {
				printf("Iteration %zd - wrong result in function ft_strcat(%zd, %zd, %zd %zd) %p != %p",
					n, align1, align2, len1, len2, res, p2 + align2);
				ret = 1;
			}
			for (j = 0; j < align2 + 64; ++j) {
				if (p2[j - 64] != '\1') {
					printf("Iteration %zd - garbage before, ft_strcat(%zd, %zd, %zd, %zd)",
						n, align1, align2, len1, len2);
					ret = 1;
					break;
				}
			}
			if (memcmp(p2 + align2, p3, len2)) {
				printf("Iteration %zd - garbage in string before, ft_strcat(%zd, %zd, %zd, %zd)",
					n, align1, align2, len1, len2);
				ret = 1;
			}
			for (j = align2 + len1 + len2 + 1; j < 512; ++j) {
				if (p2[j] != '\1') {
					printf("Iteration %zd - garbage after, ft_strcat(%zd, %zd, %zd, %zd)",
						n, align1, align2, len1, len2);
				ret = 1;
				break;
				}
			}
			if (memcmp(p1 + align1, p2 + align2 + len2, len1 + 1)) {
				printf("Iteration %zd - different strings, %s (%zd, %zd, %zd, %zd)",
					n, align1, align2, len1, len2);
				ret = 1;
			}
		}
	}
}

int
test_main(void)
{
	size_t i;

	test_init();


	for (i = 0; i < 16; ++i) {
		do_test(0, 0, i, i, SMALL_CHAR);
		do_test(0, 0, i, i, BIG_CHAR);
		do_test(0, i, i, i, SMALL_CHAR);
		do_test(i, 0, i, i, BIG_CHAR);
	}

	for (i = 1; i < 8; ++i) {
		do_test(0, 0, 8 << i, 8 << i, SMALL_CHAR);
		do_test(8 - i, 2 * i, 8 << i, 8 << i, SMALL_CHAR);
		do_test(0, 0, 8 << i, 2 << i, SMALL_CHAR);
		do_test(8 - i, 2 * i, 8 << i, 2 << i, SMALL_CHAR);
	}

	for (i = 1; i < 8; ++i) {
		do_test(i, 2 * i, 8 << i, 1, SMALL_CHAR);
		do_test(2 * i, i, 8 << i, 1, BIG_CHAR);
		do_test(i, i, 8 << i, 10, SMALL_CHAR);
		do_test(i, i, 8 << i, 10, BIG_CHAR);
	}

	do_random_tests();
	return ret;
}
#endif
#include <string.h>

// Cmocka needs these
// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
// clang-format on

typedef char *(*proto_t)(char *restrict s1, const char *restrict s2);

extern char *ft_strcat(char *restrict s1, const char *restrict s2);

proto_t arr[] = {ft_strcat,};

static void test_strcat(proto_t fn, void **state __unused) {
	char b[32];
	strcpy(b, "abc");
	assert_ptr_equal(fn(b, "123456"), b);

	// Check if strcat failed to null-terminate
	assert_int_equal(b[9], 0);
	assert_int_equal(strlen(b), strlen("abc") + strlen("123456"));

	// Check if the string is correct
	assert_int_equal(strcmp(b, "abc123456"), 0);
}

static void strcat_test(void **state) {
	for (size_t ii = 0; ii < sizeof(arr)/sizeof(*arr); ++ii)
		test_strcat(arr[ii], state);
}

#pragma mark - Public Functions -

int strcat_tests(void) {
	const struct CMUnitTest strcat_tests[] = {
		cmocka_unit_test(strcat_test),
	};

	return cmocka_run_group_tests(strcat_tests, NULL, NULL);
}

int test_main(void) {
	return strcat_tests();
}
