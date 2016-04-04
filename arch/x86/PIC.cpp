#include <toyOS/arch/x86/PIC.hpp>
#include <toyOS/arch/x86/io.hpp>

bool PIC::m_usingPIC;

bool PIC::usingPIC()
{
    return m_usingPIC;
}

void PIC::init()
{
    remapIRQs();
    outb(PIC1_DATA, 0xFF);	// mask all irq lines
    outb(PIC2_DATA, 0xFF);	// mask all irq lines
    m_usingPIC = true;
}

void PIC::sendEOI(unsigned char intNo)
{
    if(intNo < REMAP_START or intNo >= REMAP_START + 16) return;
    if(intNo >= REMAP_START + 8)
        outb(PIC2_COMMAND,PIC_EOI);	// Send EOI to slave interrupt contoller
    outb(PIC1_COMMAND,PIC_EOI);		// Send EOI to master interrupt controller
    return;
}

void PIC::disable()
{
    outb(PIC2_DATA, 0xff);
    outb(PIC1_DATA, 0xff);
    m_usingPIC = false;
}

// remap master PIC to REMAP_START and slave PIC to REMAP_START + 8
void PIC::remapIRQs()
{
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);
    outb(PIC1_DATA, REMAP_START);
    outb(PIC2_DATA, REMAP_START + 8);
    outb(PIC1_DATA, 1 << 2);	// tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    outb(PIC2_DATA, 2);		// tell Slave PIC its cascade identity (0000 0010)
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);
    return;
}

// mask (ignore) irq
void PIC::setMask(unsigned char irqLine)
{
	std::uint16_t port;
	
	if(irqLine < 8)
	{
		port = PIC1_DATA;
	}
	else
	{
		port = PIC2_DATA;
		irqLine -= 8;
	}
	out8(port, in8(port) | (1 << irqLine));
}

// clear mask (allow) on irq
void PIC::clearMask(unsigned char irqLine)
{
	std::uint16_t port;
	
	if(irqLine < 8)
	{
		port = PIC1_DATA;
	}
	else
	{
		port = PIC2_DATA;
		irqLine -= 8;
	}
	out8(port, in8(port) & ~(1 << irqLine));
}
