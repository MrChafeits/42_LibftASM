# 42_LibftASM
Introduction to 64-bit NASM by reimplementing several Libft functions

## General Instructions (adapted from subject pdf)
* Static library must be names libfts.a
* You must use the Netwide Assembler and write exclusively 64 bit assembly
* You must use Intel syntax, not GAS/AT&T
* Your Makefile must compile the project and must contain the usual rules. It must recompile and re-link the program only if necessary.
* You must submit a main that will test your functions and that will compile with
your library to show that it’s functional.

## Part 1 - Simple libc functions
In this first part, you have to recode a set of libc functions as described in their respective
man. Your functions must have the same prototypes and the same behavior as the originals. Their name must be prefixef with "ft_". For example strlen becomes ft_strlen.
You must recode the following functions:
* bzero
* strcat
* isalpha
* isdigit
* isalnum
* isascii
* isprint
* toupper
* tolower
* puts (obviously, you can use the write syscall)

## Part 2 - A little less simple libc functions
In this part, you must recode some another set of libc functions but with the Instruction
Repeat String Operations.
A little bit of documentation
You must recode the following functions:
* strlen
* memset
* memcpy
* strdup (obviously you are allowed to call malloc)

## Part 3 - Cat
To conclude, you must code a ft_cat function which will take a file descriptor (for
example 0...) as argument and that will have the same behavior as the cat command it’ll
return void.

## Bonus Part
I implemented the following functions for the project bonus:
* ft_atoi
* ft_debugbreak
* ft_memcmp
* ft_memmove
* ft_rdtsc
* ft_strcspn
* ft_strspn
* ft_strstr

Shoutouts to [acarlson](https://github.com/acarlson99) and [arherrer](https://github.com/hexamage) for their occasional assistance
