#ifndef _ARCH_X86_CONTROLREGISTERS_HPP_
#define _ARCH_X86_CONTROLREGISTERS_HPP_

#include <cstdint>

class CR2
{
    public:
        static std::uint32_t read()
        {
            std::uint32_t CR2;
            asm volatile ("mov %%cr2, %0" : "=r"(CR2));
            return CR2;
        }
};

#endif // _ARCH_X86_CONTROLREGISTERS_HPP_
