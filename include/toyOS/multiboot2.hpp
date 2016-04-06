#pragma once

// Based on multiboot2.h from GRUB, but I've written it myself in a different
// style, also, where I've noticed different member names in section 3.4 of the
// Multiboot Specification I've gone with those names (eg. multiboot2.h must be
// written by a BSD dev as is uses slices and partitions, rather than partitions
// and sub_partitions)
// Its licence is below

/*  multiboot2.h - Multiboot 2 header file.  */
/*  Copyright (C) 1999,2003,2007,2008,2009,2010  Free Software Foundation, Inc.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL ANY
 *  DEVELOPER OR DISTRIBUTOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <cstdint>

#define MULTIBOOT2_BOOTLOADER_MAGIC 0x36d76289

enum class MULTIBOOT2_TAG_TYPES : std::uint32_t
{
	END,
	CMDLINE,
	BOOT_LOADER_NAME,
	MODULE,
	BASIC_MEMINFO,
	BOOTDEV,
	MMAP,
	VBE,
	FRAMEBUFFER,
	ELF_SECTIONS,
	APM
};

struct Multiboot2_mmapEntry
{
	std::uint64_t base_addr;
	std::uint64_t length;
	std::uint32_t type;
	std::uint32_t res;
	
	static const std::uint32_t AVAILABLE = 1;
	static const std::uint32_t RESERVED = 2;
	static const std::uint32_t ACPI_RECLAIMABLE = 3;
	static const std::uint32_t MEMORY_NVS = 4;
} __attribute__((packed));

struct Multiboot2Tag
{
	MULTIBOOT2_TAG_TYPES type;
	std::uint32_t size;
};

struct Multiboot2BootInfo
{
	std::uint32_t total_size;
	std::uint32_t reserved;
	Multiboot2Tag tags[];
};

struct Multiboot2Tag_string
{
	MULTIBOOT2_TAG_TYPES type;
	std::uint32_t size;
	char string[];
};

struct Multiboot2Tag_module
{
	MULTIBOOT2_TAG_TYPES type;
	std::uint32_t size;
	std::uint32_t mod_start;
	std::uint32_t mod_end;
	char string[];
};

struct Multiboot2Tag_basicMeminfo
{
	MULTIBOOT2_TAG_TYPES type;
	std::uint32_t size;
	std::uint32_t mem_lower;
	std::uint32_t mem_upper;
};

struct Multiboot2Tag_bootdev
{
	MULTIBOOT2_TAG_TYPES type;
	std::uint32_t size;
	std::uint32_t biosdev;
	std::uint32_t partition;
	std::uint32_t sub_partition;
};

struct Multiboot2Tag_mmap
{
	MULTIBOOT2_TAG_TYPES type;
	std::uint32_t size;
	std::uint32_t entry_size;
	std::uint32_t entry_version;
	Multiboot2_mmapEntry entries[];
};
