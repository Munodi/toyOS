# Makefile

TARGET=x86

PROJDIRS:=kernel lib arch/$(TARGET)

CSOURCES:=$(shell find $(PROJDIRS) -mindepth 1 -name "*.c")
COBJECTS:=$(patsubst %.c, %.o, $(CSOURCES))
CXXSOURCES:=$(shell find $(PROJDIRS) -mindepth 1 -name "*.cpp")
CXXOBJECTS:=$(patsubst %.cpp, %.o, $(CXXSOURCES))
SSOURCES:=$(shell find $(PROJDIRS) -mindepth 1 -name "*.s")
SOBJECTS:=$(patsubst %.s, %.o, $(SSOURCES))

DEPFILES:=$(patsubst %.o,%.d,$(COBJECTS) $(CXXOBJECTS))

CFLAGS:= -O2 -I./include -nostdlib -fno-builtin -Wall -Wextra
CXXFLAGS:= -O2 -I./include -g -nostdlib -fno-builtin -Wall -Wextra -Wconversion -Werror=return-type -fno-exceptions -fno-rtti -std=gnu++11

ifeq ($(TARGET),x86)
	CC=~/opt/cross/bin/i686-elf-gcc
	CXX=~/opt/cross/bin/i686-elf-g++
	LD=~/opt/cross/bin/i686-elf-ld
	CFLAGS:= $(CFLAGS) -Dx86
	CXXFLAGS:= $(CXXFLAGS) -Dx86
	LDFLAGS:=-Tarch/$(TARGET)/i686.ld
	ASFLAGS=-felf32 -Ox -Dx86
endif

ifeq ($(TARGET),x64)
	CC=x86_64-elf-gcc
	CXX=x86_64-elf-g++
	LD=x86_64-elf-ld
	CFLAGS:= $(CFLAGS) -Dx64
	CXXFLAGS:= $(CXXFLAGS) -Dx64
	LDFLAGS:=-Tarch/$(TARGET)/x86_64.ld
	ASFLAGS=-felf64 -Ox -Dx64
endif

.PHONY: updateinitrd

all: toyOS.iso

bochs:
	bochs -q -f bochsrc.bxrc

debug:
	bochs -q -f bochsrc_debug.bxrc

#serverPrograms:
	cd servers
	make all

#driverPrograms:
	cd drivers
	make all

initrd.tar:
	cd servers && make all
	cd drivers && make all
	@rm -f initrd.tar
	tar cf initrd.tar -b 8 --format=ustar -C servers servers
	tar rf initrd.tar -b 8 --format=ustar -C drivers drivers

toyOS.iso: kernel.bin initrd.tar scripts/grub.cfg
	@echo Creating iso
	@mkdir -p isodir/boot/grub
	@cp kernel.bin isodir/boot/kernel.bin
	@cp scripts/grub.cfg isodir/boot/grub/grub.cfg
	cp initrd.tar isodir/boot/initrd.tar
	@grub-mkrescue -o toyOS.iso isodir

strip:
	strip kernel.bin
	make update

clean:
	@echo Removing object files
	@-rm -f $(COBJECTS) $(CXXOBJECTS) $(SOBJECTS) $(DEPFILES) toyOS.iso kernel.bin initrd.tar bochsout.txt
	@-rm -rf isodir

-include $(DEPFILES)

kernel.bin: $(COBJECTS) $(CXXOBJECTS) $(SOBJECTS)
	@echo Linking
	@$(LD) $(LDFLAGS) -o kernel.bin $(SOBJECTS) $(COBJECTS) $(CXXOBJECTS)

.s.o:
	@echo Assembling $<
	@nasm $(ASFLAGS) $<

.c.o:
	@echo Compiling $<
	@$(CC) $(CFLAGS) -MMD -MP -o $@ -c $<

.cpp.o:
	@echo Compiling $<
	@$(CXX) $(CXXFLAGS) -MMD -MP -o $@ -c $<
