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

CCFLAGS =
INCLUDES =

HAVE_USR_INC = $(shell test -d /usr/include; echo "$$?")
ifneq ("$(HAVE_USR_INC)", "0")
	CCFLAGS += -Wno-nullability-completeness
	INCLUDES += -I$(shell xcode-select -p)/SDKs/MacOSX.sdk/usr/include
endif

CFLAGS = $(CCFLAGS) $(INCLUDES)

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $?
	$(RANLIB) $@

test:
	@echo TODO

test-ctype: CFLAGS = $(CCFLAGS) -I/nfs/2018/c/callen/.brew/include -L. -lfts
test-ctype: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/test-main.c

test-strlen: CFLAGS = $(CCFLAGS) -I/nfs/2018/c/callen/.brew/include -L. -lfts
test-strlen: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/test-main.c

test-strcat: CFLAGS = $(CCFLAGS) -I/nfs/2018/c/callen/.brew/include -L. -lfts
test-strcat: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/test-main.c

test-memset: CFLAGS = $(CCFLAGS) -I/nfs/2018/c/callen/.brew/include -L. -lfts
test-memset: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/test-main.c

test-memcpy: CFLAGS = $(CCFLAGS) -I/nfs/2018/c/callen/.brew/include -L. -lfts
test-memcpy: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/test-main.c

test-bzero: CFLAGS = $(CCFLAGS) -I/nfs/2018/c/callen/.brew/include -L. -lfts
test-bzero: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/test-main.c

test-strdup: CFLAGS = $(CCFLAGS) -I/nfs/2018/c/callen/.brew/include -L. -lfts
test-strdup: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/test-main.c

test-puts: CFLAGS = $(CCFLAGS) -I/nfs/2018/c/callen/.brew/include -L. -lfts
test-puts: $(NAME)
	$(CC) $(CFLAGS) -o $@ tests/$@.c tests/test-main.c

.PHONY: clean
clean:
	-$(RM) $(OBJS)

fclean: clean
	-$(RM) $(NAME)

re: fclean all
