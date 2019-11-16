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

extern int ft_isalpha(int c);
extern int ft_isdigit(int c);
extern int ft_isalnum(int c);
extern int ft_isascii(int c);
extern int ft_isprint(int c);
extern int ft_toupper(int c);
extern int ft_tolower(int c);

static int
test_ctype(void)
{
	unsigned short int c;
	int lose = 0;
	for (c = 0; c <= UCHAR_MAX; ++c) {
		if (ft_isalpha(c) != isalpha(c))
			++lose;
		if (ft_isdigit(c) != isdigit(c))
			++lose;
		if (ft_isalnum(c) != isalnum(c))
			++lose;
		if (ft_isascii(c) != isascii(c))
			++lose;
		if (ft_isprint(c) != isprint(c))
			++lose;
		if (ft_toupper(c) != toupper(c))
			++lose;
		if (ft_tolower(c) != tolower(c))
			++lose;
	}
	printf("Total BOGUS'd results %d\n", lose);
	return lose;
}

int
test_main(void)
{
	return test_ctype();
}
