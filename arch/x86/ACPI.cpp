#include <toyOS/arch/x86/ACPI.hpp>
#include <toyOS/bootTerminal.hpp>

void findRSDP()
{
    char* ptr = (char*)0xC0000000;
    for(int i = 0; i <= 1024*1024; i+=16)
    {
        if(ptr[0] == 'R'
           && ptr[1] == 'S'
           && ptr[2] == 'D'
           && ptr[3] == ' '
           && ptr[4] == 'P'
           && ptr[5] == 'T'
           && ptr[6] == 'R'
           && ptr[7] == ' ')
           {
               bootTerminal::printh((unsigned)(ptr - 0xC0000000));
               bootTerminal::printc('\n');
           }
        ptr+=16;
    }
    return;
}
