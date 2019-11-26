#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Cmocka needs these
// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
// clang-format on

uint64_t ft_rdtsc(void);

int rdtsc_tests(void) {
  char buf1[65536];
  char buf2[65536];
  uint64_t start1 = ft_rdtsc();
  memset(buf1, '\0', 65536);
  uint64_t stop1 = ft_rdtsc();
  puts("----- ft_rdtsc -----");
  printf("start: %llu\nstop:  %llu\ntime:  %llu\n", start1, stop1,
         stop1 - start1);

  uint64_t start2 = clock();
  memset(buf2, '\0', 65536);
  uint64_t stop2 = clock();
  puts("----- clock -----");
  printf("start: %llu\nstop:  %llu\ntime:  %llu\n", start2, stop2,
         stop2 - start2);
  return 0;
}

#ifdef SINGLE_TEST
int test_main(void) { return rdtsc_tests(); }
#endif
