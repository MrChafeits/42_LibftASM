AS = nasm
ASFLAGS = -fmacho64
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
	  A_strcat.s \
	  A_strlen.s

SRCS = $(addprefix $(SRCDIR)/, $(SRC))
OBJS = $(SRCS:.s=.o)

CCFLAGS = -g
INCLUDES =
LDFLAGS = -L. -lfts

HAVE_USR_INC = $(shell test -d /usr/include; echo "$$?")
ifneq ("$(HAVE_USR_INC)", "0")
	CCFLAGS += -Wno-nullability-completeness
	INCLUDES += -I$(shell xcode-select -p)/SDKs/MacOSX.sdk/usr/include
else
	INCLUDES += -I/nfs/2018/c/callen/.brew/include
endif

CFLAGS = $(CCFLAGS) $(INCLUDES)

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $?
	$(RANLIB) $@

test:
	@echo TODO

test-ctype: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS)
test-ctype: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/test-main.c

test-strlen: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS)
test-strlen: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/test-main.c

test-strcat: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS)
test-strcat: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/test-main.c

test-memset: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS)
test-memset: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/test-main.c

test-memcpy: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS)
test-memcpy: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/test-main.c

test-bzero: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS)
test-bzero: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/test-main.c

test-strdup: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS)
test-strdup: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/test-main.c

test-puts: CFLAGS = $(CCFLAGS) $(INCLUDES) $(LDFLAGS)
test-puts: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/test-main.c

test-clean:
	$(RM) test-ctype test-strlen test-strcat test-memset test-memcpy test-bzero test-strdup test-puts
	$(RM) -r *.dSYM

.PHONY: clean
clean:
	-$(RM) $(OBJS)

fclean: clean
	-$(RM) $(NAME)

re: fclean all
