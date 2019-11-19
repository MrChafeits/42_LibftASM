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
INCLUDES = -I$(INCDIR)
LDFLAGS = -L. -lfts

HAVE_USR_INC = $(shell test -d /usr/include; echo "$$?")
ifneq ("$(HAVE_USR_INC)", "0")
	CCFLAGS += -Wno-nullability-completeness
	INCLUDES += -I$(shell xcode-select -p)/SDKs/MacOSX.sdk/usr/include
else
	LDFLAGS += $(shell pkg-config --libs cmocka)
	INCLUDES += $(shell pkg-config --cflags cmocka)
endif

CFLAGS = $(CCFLAGS) $(INCLUDES)

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $?
	$(RANLIB) $@

test:
	@echo TODO

tests/main.o: CFLAGS = $(CCFLAGS)
tests/main.o: tests/main.c

.PHONY: test-ctype
test-ctype: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS)
test-ctype: $(NAME) tests/main.o
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/main.o

.PHONY: test-strlen
test-strlen: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS)
test-strlen: $(NAME) tests/main.o
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/main.o

.PHONY: test-strcat
test-strcat: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS)
test-strcat: $(NAME) tests/main.o
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/main.o

.PHONY: test-memset
test-memset: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS)
test-memset: $(NAME) tests/main.o
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/main.o

.PHONY: test-memcpy
test-memcpy: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS)
test-memcpy: $(NAME) tests/main.o
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/main.o

.PHONY: test-bzero
test-bzero: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS)
test-bzero: $(NAME) tests/main.o
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/main.o

.PHONY: test-strdup
test-strdup: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS)
test-strdup: $(NAME) tests/main.o
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/main.o

.PHONY: test-puts
test-puts: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS)
test-puts: $(NAME) tests/main.o
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/main.o

.PHONY: k test-clean
k test-clean:
	$(RM) test-ctype test-strlen test-strcat test-memset test-memcpy test-bzero test-strdup test-puts cat_stdout.txt puts_stdout.txt
	$(RM) -r *.dSYM

.PHONY: clean
clean:
	-$(RM) $(OBJS)

fclean: clean test-clean
	-$(RM) $(NAME)

re: fclean all
