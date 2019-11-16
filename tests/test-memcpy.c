#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

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

//! Must use repeat string operation
extern void *ft_memcpy(void *restrict dst, const void *restrict src, size_t n);

//! Faster versions utilizing AVX instructions
extern void *A_memcpy(void *restrict dst, const void *restrict src, size_t n);

static void
do_one_test(char *dst, const char *src, size_t len)
{
	size_t i;

	/* Must clear the destination buffer set by the previous run. */
	for (i = 0; i < len; ++i)
		dst[i] = 0;
	if (ft_memcpy(dst, src, len) != dst) {
		printf("Wrong result in function ft_memcpy %p %p\n", ft_memcpy(dst, src, len), dst);
		ret = 1;
		return;
	}
	if (memcmp(dst, src, len) != 0) {
		printf("Wrong result in function ft_memset dst %p \"%.*s\" src %p \"%.*s\" len %zu\n",
			dst, (int)len, dst, src, (int)len, src, len);
		ret = 1;
		return;
	}
}

static void
do_test(size_t align1, size_t align2, size_t len)
{
	size_t i, j;
	char *s1, *s2;

	align1 &= 63;
	if (align1 + len >= page_size)
		return;

	align2 &= 63;
	if (align2 + len >= page_size)
		return;
	s1 = (char*)(buf1+align1);
	s2 = (char*)(buf2+align2);

	for (i = 0, j = 1; i < len; i++, j += 23)
		s1[i] = j;

	do_one_test(s2, s1, len);
}

static void
do_random_tests(void)
{
	size_t i, j, n, align1, align2, len, size1, size2, size;
	int c;
	unsigned char *p1, *p2;
	unsigned char *res;

	for (n = 0; n < ITERATIONS; n++) {
		if (n == 0) {
			len = getpagesize ();
			size = len + 512;
			size1 = size;
			size2 = size;
			align1 = 512;
			align2 = 512;
		} else {
			if ((random () & 255) == 0)
				size = 65536;
			else
				size = 768;
			if (size > page_size)
				size = page_size;
			size1 = size;
			size2 = size;
			i = random ();
			if (i & 3)
				size -= 256;
			if (i & 1)
				size1 -= 256;
			if (i & 2)
				size2 -= 256;
			if (i & 4) {
				len = random () % size;
				align1 = size1 - len - (random () & 31);
				align2 = size2 - len - (random () & 31);
				if (align1 > size1)
					align1 = 0;
				if (align2 > size2)
					align2 = 0;
			} else {
				align1 = random () & 63;
				align2 = random () & 63;
				len = random () % size;
				if (align1 + len > size1)
					align1 = size1 - len;
				if (align2 + len > size2)
					align2 = size2 - len;
			}
		}
		p1 = buf1 + page_size - size1;
		p2 = buf2 + page_size - size2;
		c = random () & 255;
		j = align1 + len + 256;
		if (j > size1)
			j = size1;
		for (i = 0; i < j; ++i)
			p1[i] = random () & 255;
		j = align2 + len + 256;
		if (j < size2)
			j = size2;
		memset(p2, c, j);
		res = (unsigned char*)ft_memcpy((char*)(p2+align2), (char*)(p1+align1), len);
		if (res != p2 + align2) {
			printf("Iteration %zd - wrong result in function ft_memcpy(%zd, %zd, %zd) %p != %p",
				n, align1, align2, len, res, p2 + align2);
			ret = 1;
		}
		for (i = 0; i < align2; ++i) {
			if (p2[i] != c) {
				printf("Iteration %zd - garbage before, ft_memcpy(%zd, %zd, %zd)",
					n, align1, align2, len);
				ret = 1;
				break;
			}
		}
		for (i = align2 + len; i < j; ++i) {
			if (p2[i] != c) {
				printf("Iteration %zd - garbage after, ft_memcpy(%zd, %zd, %zd)",
					n, align1, align2, len);
				ret = 1;
				break;
			}
		}
		if (memcmp(p1 + align1, p2 + align2, len)) {
			printf("Iteration %zd - different strings, ft_memcpy(%zd, %zd, %zd)",
				n, align1, align2, len);
			ret = 1;
		}
	}
}

static void
do_test1(void)
{
	size_t size = 0x100000;
	void *large_buf;

	large_buf = mmap(NULL, size * 2 + page_size, PROT_READ | PROT_WRITE,
					MAP_PRIVATE | MAP_ANON, -1, 0);
	if (large_buf == MAP_FAILED) {
		puts("failed to allocate large_buf, skipping do_test1");
		return;
	}

	assert(mprotect(large_buf + size, page_size, PROT_NONE) == 0);

	size_t array_size = size / sizeof(uint32_t);
	uint32_t *dest = large_buf;
	uint32_t *src = large_buf + size + page_size;
	size_t i;

	for (i = 0; i < array_size; ++i)
		src[i] = (uint32_t)i;

	{
		memset(dest, -1, size);
		ft_memcpy((char*)dest, (char*)src, size);
		for (i = 0; i < array_size; ++i)
			if (dest[i] != src[i]) {
				printf("Wrong result in function ft_memcpy dst \"%p\" src \"%p\" offset \"%zd\"", dest, src, i);
				ret = 1;
				break;
			}
	}
	munmap((void*)dest, size);
	munmap((void*)src, size);
}

int
test_main(void)
{
	size_t i;

	test_init();

	for (i = 0; i < 18; ++i) {
		do_test(0, 0, 1 << i);
		do_test(i, 0, 1 << i);
		do_test(0, i, 1 << i);
		do_test(i, i, 1 << i);
	}

	for (i = 0; i < 32; ++i) {
		do_test(0, 0, i);
		do_test(i, 0, i);
		do_test(0, i, i);
		do_test(i, i, i);
	}

	for (i = 3; i < 32; ++i) {
		if ((i & (i - 1)) == 0)
			continue;
		do_test(0, 0, 16 * i);
		do_test(i, 0, 16 * i);
		do_test(0, i, 16 * i);
		do_test(i, i, 16 * i);
	}

	do_test(0, 0, getpagesize());

	do_random_tests();

	do_test1();

	return ret;
}
