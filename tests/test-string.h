#include <sys/cdefs.h>
#include <string.h>
typedef struct
{
	const char *name;
	void (*fn)(void);
	long test;
} impl_t;

impl_t *__start_impls, *__stop_impls;

#define IMPL(name, test) \
	impl_t tst_##name = {__STRING(name), (void (*)(void))name, test};

#ifdef TEST_MAIN

#ifdef HAVE_CC_INHIBIT_LOOP_TO_LIBCALL
# define inhibit_loop_to_libcall \
	__attribute__ ((__optimize__ ("-fno-tree-loop-distribute-patterns")))
#else
# define inhibit_loop_to_libcall
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/param.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <time.h>
// #include <ifunc-impl-list.h>
#include <stdbool.h>
#include <stddef.h>
struct libc_ifunc_impl
{
	/* The name of the fucntion to be tested. */
	const char *name;
	/* The address of the function to be tested. */
	void (*fn)(void);
	/* True if this implementation is usable on this machine */
	bool usable;
};

#define TEST_FUNCTION test_main
#define TIMEOUT (4 * 60)
unsigned char *buf1, *buf2;
int ret, do_srandom;
unsigned int seed;
size_t page_size;

#ifndef ITERATIONS
size_t iterations = 100000;
#define ITERATIONS iterations
#endif

#define CALL(impl, ...) \
	(*(proto_t)(impl)->fn)(__VA_ARGS__)

#ifdef TEST_NAME
/* Increase size of FUNC_LIST if assert is triggered at run-time.  */
static struct libc_ifunc_impl func_list[32];
static int func_count;
static int impl_count = -1;
static impl_t *impl_array;

#define FOR_EACH_IMPL(impl, notall)                                 \
	impl_t *impl;                                                   \
	int count;                                                      \
	if (impl_count == -1)                                           \
	{                                                               \
		impl_count = 0;                                             \
		if (func_count != 0)                                        \
		{                                                           \
			int f;                                                  \
			impl_t *skip = NULL, *a;                                \
			for (impl = __start_impls; impl < __stop_impls; ++impl) \
				if (strcmp(impl->name, TEST_NAME) == 0)             \
					skip = impl;                                    \
				else                                                \
					impl_count++;                                   \
			a = impl_array = malloc((impl_count + func_count) *     \
									sizeof(impl_t));                \
			for (impl = __start_impls; impl < __stop_impls; ++impl) \
				if (impl != skip)                                   \
					*a++ = *impl;                                   \
			for (f = 0; f < func_count; f++)                        \
				if (func_list[f].usable)                            \
				{                                                   \
					a->name = func_list[f].name;                    \
					a->fn = func_list[f].fn;                        \
					a->test = 1;                                    \
					a++;                                            \
				}                                                   \
			impl_count = a - impl_array;                            \
		}                                                           \
		else                                                        \
		{                                                           \
			impl_count = __stop_impls - __start_impls;              \
			impl_array = __start_impls;                             \
		}                                                           \
	}                                                               \
	impl = impl_array;                                              \
	for (count = 0; count < impl_count; ++count, ++impl)            \
		if (!notall || impl->test)
#else
#define FOR_EACH_IMPL(impl, notall)                                 \
	for (impl_t *impl = __start_impls; impl < __stop_impls; ++impl) \
		if (!notall || impl->test)
#endif
#ifndef BUF1PAGES
#define BUF1PAGES 1
#endif

static void
test_init(void)
{
#ifdef TEST_NAME
	func_count = 3;//__libc_ifunc_impl_list(TEST_NAME, func_list, (sizeof func_list / sizeof func_list[0]));
#endif

	page_size = 2 * getpagesize();
#ifdef MIN_PAGE_SIZE
	if (page_size < MIN_PAGE_SIZE)
		page_size = MIN_PAGE_SIZE;
#endif
	buf1 = mmap(0, (BUF1PAGES + 1) * page_size, PROT_READ | PROT_WRITE,
				MAP_PRIVATE | MAP_ANON, -1, 0);
	if (buf1 == MAP_FAILED)
		errc(EXIT_FAILURE, errno, "mmap failed");
	if (mprotect(buf1 + BUF1PAGES * page_size, page_size, PROT_NONE))
		errc(EXIT_FAILURE, errno, "mprotect failed");
	buf2 = mmap(0, 2 * page_size, PROT_READ | PROT_WRITE,
				MAP_PRIVATE | MAP_ANON, -1, 0);
	if (buf2 == MAP_FAILED)
		errc(EXIT_FAILURE, errno, "mmap failed");
	if (mprotect(buf2 + page_size, page_size, PROT_NONE))
		errc(EXIT_FAILURE, errno, "mprotect failed");
	if (do_srandom)
	{
		printf("Setting seed to 0x%x\n", seed);
		srandom(seed);
	}

	memset(buf1, 0xa5, BUF1PAGES * page_size);
	memset(buf2, 0x5a, page_size);
}

#endif
