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
ft_memmove.s \
ft_atoi.s \
ft_memcmp.s \
ft_rdtsc.s \
ft_strspn.s \
ft_strcspn.s \
ft_strstr.s \
ft_rdtsc.s \
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
A_memmove.s

TSTSRC = main.c \
test-atoi.c \
test-bzero.c \
test-ctype.c \
test-debugbreak.c \
test-memcmp.c \
test-memcpy.c \
test-memmove.c \
test-memset.c \
test-puts.c \
test-strcat.c \
test-strdup.c \
test-strlen.c \
test-strspn.c

SRCS = $(addprefix $(SRCDIR)/, $(SRC))
OBJS = $(SRCS:.s=.o)

TSTSRCS = $(addprefix tests/, $(TSTSRC))
TSTOBJS = $(TSTSRCS:.c=.o)

INCDIR = include

CCFLAGS = -Wall -Wextra -Werror
INCLUDES = -I$(INCDIR) $(shell pkg-config --cflags cmocka)
LDFLAGS = -L. -lfts $(shell pkg-config --libs cmocka)

HAVE_USR_INC = $(shell test -d /usr/include; echo "$$?")
ifneq ("$(HAVE_USR_INC)", "0")
	CCFLAGS += -Wno-nullability-completeness
	INCLUDES += -I$(shell xcode-select -p)/SDKs/MacOSX.sdk/usr/include
else
	LDFLAGS +=
	INCLUDES +=
endif

CFLAGS = $(CCFLAGS) $(INCLUDES)

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $?
	$(RANLIB) $@

test: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS)
test: $(NAME)
	$(CC) $(CFLAGS) -o $@ $(TSTSRCS)

.PHONY: test-atoi
test-atoi: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS) -DSINGLE_TEST
test-atoi: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/main.c

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

.PHONY: test-memmove
test-memmove: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS) -DSINGLE_TEST
test-memmove: $(NAME)
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
	@echo $(TSTSRC:.c=)
	$(RM) test test-ctype test-strlen test-strcat test-memset test-memcpy
	$(RM) test-bzero test-strdup test-puts cat_stdout.txt puts_stdout.txt test-strspn test-atoi test-memmove test-strcspn test-rdtsc test-strstr
	$(RM) -r *.dSYM

.PHONY: clean
clean:
	-$(RM) $(OBJS)

fclean: clean test-clean
	-$(RM) $(NAME)

re: fclean all
