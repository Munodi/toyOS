#ifndef _TOYOS_ARCH_X86_PIC_HPP_
#define _TOYOS_ARCH_X86_PIC_HPP_

class PIC
{
    public:
        static void init();
        static bool usingPIC();
        static void sendEOI(unsigned char irq);
        static void disable();
    private:
        static void remapIRQs();
        static bool m_usingPIC;
};

#endif // _TOYOS_ARCH_X86_PIC_HPP_
