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

static inline void _cpuid(int code, std::uint32_t* a, std::uint32_t* d)
{
	asm volatile(	"cpuid"
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
