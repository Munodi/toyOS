#ifndef _TOYOS_BOOT_TERMINAL_HPP_
#define _TOYOS_BOOT_TERMINAL_HPP_

class bootTerminal
{
    public:
        static void cls();
        static void init();
        static void prints(const char*);
        static void printc(char);
        static void printh(unsigned);
        static void print10(unsigned);
        static void seriousErrorPrint(const char*);   // used in assert, in x86 should drop down to real mode print message and halt
};

#endif //_TOYOS_BOOT_TERMINAL_HPP_
