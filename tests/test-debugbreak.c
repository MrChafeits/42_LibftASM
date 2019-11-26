#include <stdio.h>

// Cmocka needs these
// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
// clang-format on
// TODO

extern void ft_debugbreak(void);

int debugbreak_tests(void) {
    puts("before");
    ft_debugbreak();
    puts("after");
    return 0;
}

#ifdef SINGLE_TEST
int test_main(void) {
    return debugbreak_tests();
}
#endif
