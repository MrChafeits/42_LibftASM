AS = nasm
ASFLAGS = -fmacho64 -Isrc -DPOSITIONINDEPENDENT -DUNIX
ARFLAGS = -cru

RANLIB = ranlib
NAME = libfts.a

INCDIR = include
SRCDIR = src

SRC = ft_bzero.s \
	  ft_strcat.s \
	  ft_isalpha.s \
	  ft_isdigit.s \
	  ft_isalnum.s \
	  ft_isascii.s \
	  ft_isprint.s \
	  ft_toupper.s \
	  ft_tolower.s \
	  ft_puts.s \
	  ft_strlen.s \
	  ft_memset.s \
	  ft_memcpy.s \
	  ft_strdup.s \
	  ft_cat.s \
	  A_memcpy.s \
	  A_strlen.s \
	  A_strspn.s \
	  A_rdtsc.s \
	  A_instrset.s \
	  A_cachesize.s \
	  A_debugbreak.s \
	  A_unalignedisfaster.s \
	  A_cputype.s \
	  A_memcmp.s \
	  A_memmove.s \
	  A_substring.s

SRCS = $(addprefix $(SRCDIR)/, $(SRC))
OBJS = $(SRCS:.s=.o)

INCDIR = include

CCFLAGS = -Wall -Wextra -Werror
INCLUDES = -I$(INCDIR) $(shell pkg-config --cflags cmocka)
LDFLAGS = -L. -lfts $(shell pkg-config --libs cmocka)

HAVE_USR_INC = $(shell test -d /usr/include; echo "$$?")
ifneq ("$(HAVE_USR_INC)", "0")
	CCFLAGS += -Wno-nullability-completeness
	INCLUDES += -I$(shell xcode-select -p)/SDKs/MacOSX.sdk/usr/include
else
	# LDFLAGS +=
	# INCLUDES +=
endif

CFLAGS = $(CCFLAGS) $(INCLUDES)

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $?
	$(RANLIB) $@

test: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS)
test: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/main.c tests/test-ctype.c tests/test-strlen.c tests/test-strcat.c tests/test-memset.c tests/test-memcpy.c tests/test-bzero.c tests/test-puts.c tests/test-strdup.c

.PHONY: test-ctype
test-ctype: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS) -DSINGLE_TEST
test-ctype: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/main.c

.PHONY: test-strlen
test-strlen: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS) -DSINGLE_TEST
test-strlen: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/main.c

.PHONY: test-strcat
test-strcat: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS) -DSINGLE_TEST
test-strcat: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/main.c

.PHONY: test-memset
test-memset: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS) -DSINGLE_TEST
test-memset: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/main.c

.PHONY: test-memcpy
test-memcpy: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS) -DSINGLE_TEST
test-memcpy: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/main.c

.PHONY: test-bzero
test-bzero: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS) -DSINGLE_TEST
test-bzero: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/main.c

.PHONY: test-strdup
test-strdup: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS) -DSINGLE_TEST
test-strdup: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/main.c

.PHONY: test-puts
test-puts: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS) -DSINGLE_TEST
test-puts: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/main.c

.PHONY: test-strspn
test-strspn: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS) -DSINGLE_TEST
test-strspn: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/main.c

.PHONY: k test-clean
k test-clean:
	$(RM) test test-ctype test-strlen test-strcat test-memset test-memcpy
	$(RM) test-bzero test-strdup test-puts cat_stdout.txt puts_stdout.txt test-strspn
	$(RM) -r *.dSYM

.PHONY: clean
clean:
	-$(RM) $(OBJS)

fclean: clean test-clean
	-$(RM) $(NAME)

re: fclean all
