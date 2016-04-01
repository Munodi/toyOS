#ifndef _ARCH_X86_ASM_H_
#define _ARCH_X86_ASM_H_

#include <sdtint.h>

static __inline__ void cli()
{
    asm volatile("cli");
}

static __inline__ void sti()
{
    asm volatile("sti");
}

static __inline__ void hlt()
{
    asm volatile("hlt");
}

static inline void _cpuid(int code, std::uint32_t* a, std::uint32_t* d)
{
    asm volatile( "cpuid"
                  : "=a"(*a), "=d"(*d) : "0"(code) : "ebx", "ecx");
}

#endif /*_ARCH_X86_ASM_H_ */
