#pragma once

//#define PIC1			0x20		/* IO base address for master PIC */
//#define PIC2			0xA0		/* IO base address for slave PIC */
//#define PIC1_COMMAND	PIC1
//#define PIC1_DATA		(PIC1+1)
//#define PIC2_COMMAND	PIC2
//#define PIC2_DATA		(PIC2+1)
//#define PIC_EOI			0x20		/* End-of-interrupt command code */

class PIC
{
    public:
        static void init();
        static bool usingPIC();
        static void sendEOI(unsigned char intNo);
        static void disable();
        static void setMask(unsigned char irqLine);
        static void clearMask(unsigned char irqLine);
        
        static const unsigned char REMAP_START = 32;
    private:
        static void remapIRQs();
        static bool m_usingPIC;
        
        static const unsigned char PIC1 = 0x20;	/* IO base address for master PIC */
        static const unsigned char PIC2 = 0xA0;	/* IO base address for slave PIC */
        static const unsigned char PIC1_COMMAND = PIC1;
		static const unsigned char PIC1_DATA = (PIC1+1);
		static const unsigned char PIC2_COMMAND = PIC2;
		static const unsigned char PIC2_DATA = (PIC2+1);
		static const unsigned char PIC_EOI = 0x20;	/* End-of-interrupt command code */
};

