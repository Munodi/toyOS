# Makefile,based on JamesM's kernel tutorials.

TARGET=x86

PROJDIRS:=kernel lib arch


CSOURCES=$(shell find $(PROJDIRS) -mindepth 1 -name "*.c")
#CSOURCES=$(shell find -name *.c)
COBJECTS=$(patsubst %.c, %.o, $(CSOURCES))
CXXSOURCES=$(shell find $(PROJDIRS) -mindepth 1 -name "*.cpp")
CXXOBJECTS=$(patsubst %.cpp, %.o, $(CXXSOURCES))
SSOURCES=$(shell find -name *.s)
SOBJECTS=$(patsubst %.s, %.o, $(SSOURCES))

ifeq ($(TARGET),x86)
	CC=~/opt/cross/bin/i686-elf-gcc
	CXX=~/opt/cross/bin/i686-elf-g++
	LD=~/opt/cross/bin/i686-elf-ld
	CFLAGS= -O2 -I./include -nostdlib -fno-builtin -Wall -Wextra -Dx86
	CXXFLAGS= -O2 -I./include -nostdlib -fno-builtin -Wall -Wextra -fno-exceptions -fno-rtti -Dx86
	LDFLAGS=-Ti686.ld
	ASFLAGS=-felf32 -Ox -Dx86
endif

ifeq ($(TARGET),x64)
	CC=x86_64-elf-gcc
	CXX=x86_64-elf-g++
	LD=x86_64-elf-ld
	CFLAGS= -O2 -nostdlib -fno-builtin -Wall -Wextra -I./include -Dx64
	CXXFLAGS= -O2 -nostdlib -fno-builtin -Wall -Wextra -I./include -Dx64
	LDFLAGS=-Tx86_64.ld
	ASFLAGS=-felf64 -Ox -Dx64
endif

all: $(COBJECTS) $(CXXOBJECTS) $(SOBJECTS) updateinitrd link update

bochs:
	bochs -q -f bochsrc.bxrc

debug:
	bochs -q -f bochsrc_debug.bxrc

qemu:
	qemu -kernel kernel.bin

serverPrograms:
	cd servers

driverPrograms:
	cd drivers
	make all

updateinitrd:
	@rm -f initrd

update:
	@echo Updating floppy image
	(cd scripts; ./update_floppy.sh)

strip:
	strip kernel.bin
	make update

clean:
	@echo Removing object files
	@-rm -f $(COBJECTS) $(CXXOBJECTS) $(SOBJECTS) kernel.bin bochsout.txt

link:
	@echo Linking
	@$(LD) $(LDFLAGS) -o kernel.bin $(SOBJECTS) $(COBJECTS) $(CXXOBJECTS)

.s.o:
	@echo Assembling $<
	@nasm $(ASFLAGS) $<

.c.o:	
	@echo Compiling $<
	@$(CC) $(CFLAGS) -o $@ -c $<

.cpp.o:
	@echo Compiling $<
	@$(CXX) $(CXXFLAGS) -o $@ -c $<
