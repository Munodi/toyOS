AR=i586-elf-ar
CC=i586-elf-gcc
CFLAGS=-Wall -Wextra -Werror -Wconversion -Wmissing-declarations -nostdinc -nostdlib -nostartfiles -I../include
OBJS=itoa.o strlen.o strcmp.o abs.o labs.o memcpy.o memset.o
SRCS=itoa.o strlen.c strcmp.c abs.c labs.c memcpy.c memset.c


all: libc.a

libc.a: $(OBJS)
	$(AR) rcs libc.a $(OBJS)


.PHONY: clean

clean:
	rm -f libc.a $(OBJS)

%.o: %.c
	@$(CC) $(CFLAGS) -c $<
