#!/usr/bin/env python3
import ninja_syntax
import glob

TARGET='x86'

PROJ_DIRS = ['kernel', 'lib', f'arch/{TARGET}']

C_SOURCES = [filename for s in PROJ_DIRS for filename in glob.glob(f'{s}/**/*.c', recursive=True)]
C_OBJECTS = [filename.replace('.c', '.o') for filename in C_SOURCES]
CXX_SOURCES = [filename for s in PROJ_DIRS for filename in glob.glob(f'{s}/**/*.cpp', recursive=True)]
CXX_OBJECTS = [filename.replace('.cpp', '.o') for filename in CXX_SOURCES]
S_SOURCES = [filename for s in PROJ_DIRS for filename in glob.glob(f'{s}/**/*.s', recursive=True)]
S_OBJECTS = [filename.replace('.s', '.o') for filename in S_SOURCES]
OBJ_FILES = C_OBJECTS + CXX_OBJECTS + S_OBJECTS

CFLAGS = '-O2 -I./include -nostdlib -fno-builtin -Wall -Wextra'
CXXFLAGS = '-O2 -I./include -g -nostdlib -fno-builtin -Wall -Wextra -Wconversion -Werror=return-type -fno-exceptions -fno-rtti -std=gnu++11'

if TARGET == 'x86':
    CC = '~/opt/cross/bin/i686-elf-gcc'
    CXX = '~/opt/cross/bin/i686-elf-g++'
    LD = '~/opt/cross/bin/i686-elf-ld'
    CFLAGS += ' -Dx86'
    CXXFLAGS += ' -Dx86'
    LDFLAGS = f'-Tarch/{TARGET}/i686.ld'
    ASFLAGS = '-felf32 -Ox -Dx86'
elif TARGET == 'x64':
    CC = 'x86_64-elf-gcc'
    CXX = 'x86_64-elf-g++'
    LD = 'x86_64-elf-ld'
    CFLAGS += ' -Dx64'
    CXXFLAGS += ' -Dx64'
    LDFLAGS = f'-Tarch/{TARGET}/x86_64.ld'
    ASFLAGS = '-felf64 -Ox -Dx64'

ninja = ninja_syntax.Writer(open('build.ninja', 'w'))
ninja.variable('ninja_required_version', '1.7')

ninja.rule('CC',
    command=f'{CC} -c $in -o $out -MD -MF $out.d {CFLAGS}',
    depfile='$out.d',
    deps='gcc')

ninja.rule('CXX',
    command=f'{CXX} -c $in -o $out -MD -MF $out.d {CXXFLAGS}',
    depfile='$out.d',
    deps='gcc')

ninja.rule('nasm',
    command=f'nasm {ASFLAGS} $in')

ninja.rule('link',
    command=f'{LD} {LDFLAGS} -o $out $in',
    description='linking $out')

ninja.rule('iso',
    command='mkdir -p isodir/boot/grub && cp -t isodir $in && grub-mkrescue -o $out isodir')

for source in CXX_SOURCES:
    ninja.build(source.replace('.cpp', '.o'), 'CXX', source)
for source in  S_SOURCES:
    ninja.build(source.replace('.s', '.o'), 'nasm', source)
ninja.build('kernel.bin', 'link', OBJ_FILES)
ninja.build('toyOS.iso', 'iso', ['kernel.bin', 'scripts/grub.cfg'])