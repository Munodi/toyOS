#include <toyOS/panic.hpp>
#include <toyOS/bootTerminal.hpp>

void PANIC(const char* s)
{
    bootTerminal::prints(s);
    #if defined(x86) || defined(x64)
        asm volatile ("cli\nhlt");
    #else
        for(;;);
    #endif
    return;
}
