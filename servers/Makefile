TARGET:=x86

SERVERDIRS:=idle
SERVERS := $(SERVERDIRS)

CSOURCES:=$(shell find $(SERVERS) -mindepth 1 -name "*.c")
COBJECTS:=$(patsubst %.c, %.o, $(CSOURCES))
CXXSOURCES:=$(shell find $(SERVERS) -mindepth 1 -name "*.cpp")
CXXOBJECTS:=$(patsubst %.cpp, %.o, $(CXXSOURCES))
SSOURCES:=$(shell find $(SERVERS) -mindepth 1 -name "*.s")
SOBJECTS:=$(patsubst %.s, %.o, $(SSOURCES))

CFLAGS:= -O2 -I./include -nostdlib -fno-builtin -Wall -Wextra
CXXFLAGS:= -O2 -I./include -nostdlib -fno-builtin -Wall -Wextra -Wconversion -fno-exceptions -fno-rtti -std=gnu++11

ifeq ($(TARGET),x86)
	CC=~/opt/cross/bin/i686-elf-gcc
	CXX=~/opt/cross/bin/i686-elf-g++
	LD=~/opt/cross/bin/i686-elf-ld
	CFLAGS:= $(CFLAGS) -Dx86
	CXXFLAGS:= $(CXXFLAGS) -Dx86
	ASFLAGS=-felf32 -Ox -Dx86
endif

ifeq ($(TARGET),x64)
	CC=x86_64-elf-gcc
	CXX=x86_64-elf-g++
	LD=x86_64-elf-ld
	CFLAGS:= $(CFLAGS) -Dx64
	CXXFLAGS:= $(CXXFLAGS) -Dx64
	ASFLAGS=-felf64 -Ox -Dx64
endif

.PHONY: clean

all: idle/idle
	mkdir -p servers
	cp idle/idle servers
	strip servers/*

idle/idle: idle/main.o
	@echo Linking $@
	$(LD) -o $@ idle/main.o

.c.o:
	@echo Compiling $<
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f idle/idle
	rm -f idle/main.o
	rm -rf servers
