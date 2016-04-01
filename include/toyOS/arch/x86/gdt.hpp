#ifndef _TOYOS_ARCH_X86_GDT_HPP_
#define _TOYOS_ARCH_X86_GDT_HPP_

#include <toyOS/config.hpp>
#include <cstdint>

class gdt
{
    public:
        static void init();
        static void* getESP(std::uint32_t processorNumber);
        static void setESP(std::uint32_t processorNumber, void* ESP);
    private:
        gdt();
};


#endif // _TOYOS_ARCH_X86_GDT_HPP_
