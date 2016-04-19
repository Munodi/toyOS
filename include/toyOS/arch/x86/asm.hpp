#pragma once

#include <cstdint>

static inline void cli()
{
	asm volatile("cli");
}

static inline void sti()
{
	asm volatile("sti");
}

static inline void hlt()
{
	asm volatile("hlt");
}

static inline std::uint32_t readCR2()
{
	std::uint32_t CR2;
	asm volatile ("mov %%cr2, %0" : "=r"(CR2));
	return CR2;
}

static inline std::uint32_t readCR3()
{
	std::uint32_t CR3;
	asm volatile ("mov %%cr3, %0" : "=r"(CR3));
	return CR3;
}

static inline void writeCR3(std::uint32_t val)
{
	asm volatile ("mov %0, %%cr3" :: "r"(val));
}

static inline void _cpuid(int code, std::uint32_t* a, std::uint32_t* d)
{
	asm volatile (	"cpuid"
					: "=a"(*a), "=d"(*d) : "0"(code) : "ebx", "ecx");
}

static inline void invlpg(void* m)
{
	/* Clobber memory to avoid optimizer re-ordering access before invlpg, which may cause nasty bugs. */
	asm volatile ( "invlpg (%0)" : : "b"(m) : "memory" );
}

static inline void bochsBreak()
{
	asm volatile("xchgw %bx, %bx");
}
