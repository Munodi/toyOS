#include <cstdint>
#include <cstring>
#include <toyOS/bootTerminal.hpp>
#include <toyOS/arch/x86/idt.hpp>
#include <toyOS/arch/x86/PIC.hpp>
#include <toyOS/arch/x86/io.hpp>
#include <toyOS/arch/x86/asm.hpp>
#include <toyOS/irqHandler.hpp>

struct IdtEntry
{
	void setGate(std::uint32_t base, std::uint8_t sel, bool present, std::size_t dpl)
	{
		this->base_low = base & 0xFFFF;
		this->base_high = (base >> 16) & 0xFFFF;
		this->sel = sel;
		this->always0 = 0;
		this->flags = ((present ? 1 : 0) << 7) | (dpl << 5) | 0b01110;
	}
	std::uint16_t base_low;		// The lower 16 bits of the address to jump to when this interrupt fires
	std::uint16_t sel;			// Kernel segment selector
	std::uint8_t always0;		// Always set to 0
	std::uint8_t flags;			// Present and descriptor privilege level
	std::uint16_t base_high;	// The upper 16 bits of the address to jump to
} __attribute__((packed));

struct idt_ptr_t
{
	std::uint16_t limit;
	std::uint32_t base;
} __attribute__((packed));

struct Registers
{
	std::uint32_t gs, fs, es, ds;
	std::uint32_t edi, esi, ebp, ebx, edx, ecx, eax;
	std::uint32_t intNo, code;
	std::uint32_t eip, cs, eflags, useresp, ss;		// useresp and ss only valid on privilege-level change, see 6.12.1
};

IdtEntry idtEntries[256] __attribute__ ((aligned (8)));
idt_ptr_t idt_ptr;

extern "C" void loadIdt();

void irqRemap();

void initIdt()
{
	idt_ptr.base = (std::uint32_t)idtEntries;
	idt_ptr.limit = sizeof(IdtEntry) * 256 - 1;
	std::memset(idtEntries, 0, sizeof(IdtEntry) * 256);
	
	idtEntries[0].setGate((std::uint32_t)isr0, 0x08, true, 3);
	idtEntries[1].setGate((std::uint32_t)isr1, 0x08, true, 3);
	idtEntries[2].setGate((std::uint32_t)isr2, 0x08, true, 3);
	idtEntries[3].setGate((std::uint32_t)isr3, 0x08, true, 3);
	idtEntries[4].setGate((std::uint32_t)isr4, 0x08, true, 3);
	idtEntries[5].setGate((std::uint32_t)isr5, 0x08, true, 3);
	idtEntries[6].setGate((std::uint32_t)isr6, 0x08, true, 3);
	idtEntries[7].setGate((std::uint32_t)isr7, 0x08, true, 3);
	idtEntries[8].setGate((std::uint32_t)isr8, 0x08, true, 3);
	idtEntries[9].setGate((std::uint32_t)isr9, 0x08, true, 3);
	idtEntries[10].setGate((std::uint32_t)isr10, 0x08, true, 3);
	idtEntries[11].setGate((std::uint32_t)isr11, 0x08, true, 3);
	idtEntries[12].setGate((std::uint32_t)isr12, 0x08, true, 3);
	idtEntries[13].setGate((std::uint32_t)isr13, 0x08, true, 3);
	idtEntries[14].setGate((std::uint32_t)isr14, 0x08, true, 3);
	idtEntries[15].setGate((std::uint32_t)isr15, 0x08, true, 3);
	idtEntries[16].setGate((std::uint32_t)isr16, 0x08, true, 3);
	idtEntries[17].setGate((std::uint32_t)isr17, 0x08, true, 3);
	idtEntries[18].setGate((std::uint32_t)isr18, 0x08, true, 3);
	idtEntries[19].setGate((std::uint32_t)isr19, 0x08, true, 3);
	idtEntries[20].setGate((std::uint32_t)isr20, 0x08, true, 3);
	idtEntries[21].setGate((std::uint32_t)isr21, 0x08, true, 3);
	idtEntries[22].setGate((std::uint32_t)isr22, 0x08, true, 3);
	idtEntries[23].setGate((std::uint32_t)isr23, 0x08, true, 3);
	idtEntries[24].setGate((std::uint32_t)isr24, 0x08, true, 3);
	idtEntries[25].setGate((std::uint32_t)isr25, 0x08, true, 3);
	idtEntries[26].setGate((std::uint32_t)isr26, 0x08, true, 3);
	idtEntries[27].setGate((std::uint32_t)isr27, 0x08, true, 3);
	idtEntries[28].setGate((std::uint32_t)isr28, 0x08, true, 3);
	idtEntries[29].setGate((std::uint32_t)isr29, 0x08, true, 3);
	idtEntries[30].setGate((std::uint32_t)isr30, 0x08, true, 3);
	idtEntries[31].setGate((std::uint32_t)isr31, 0x08, true, 3);
	idtEntries[32].setGate((std::uint32_t)isr32, 0x08, true, 3);
	idtEntries[33].setGate((std::uint32_t)isr33, 0x08, true, 3);
	idtEntries[34].setGate((std::uint32_t)isr34, 0x08, true, 3);
	idtEntries[35].setGate((std::uint32_t)isr35, 0x08, true, 3);
	idtEntries[36].setGate((std::uint32_t)isr36, 0x08, true, 3);
	idtEntries[37].setGate((std::uint32_t)isr37, 0x08, true, 3);
	idtEntries[38].setGate((std::uint32_t)isr38, 0x08, true, 3);
	idtEntries[39].setGate((std::uint32_t)isr39, 0x08, true, 3);
	idtEntries[40].setGate((std::uint32_t)isr40, 0x08, true, 3);
	idtEntries[41].setGate((std::uint32_t)isr41, 0x08, true, 3);
	idtEntries[42].setGate((std::uint32_t)isr42, 0x08, true, 3);
	idtEntries[43].setGate((std::uint32_t)isr43, 0x08, true, 3);
	idtEntries[44].setGate((std::uint32_t)isr44, 0x08, true, 3);
	idtEntries[45].setGate((std::uint32_t)isr45, 0x08, true, 3);
	idtEntries[46].setGate((std::uint32_t)isr46, 0x08, true, 3);
	idtEntries[47].setGate((std::uint32_t)isr47, 0x08, true, 3);
	
	loadIdt();
	PIC::init();

	asm volatile("sti");
}

IrqHandler* irqHandlers[16];

extern "C" void isrHandler(const Registers *r)
{
	if(r->intNo < 32)
	{
		bootTerminal::prints("caught interrupt: ");
		bootTerminal::print10(r->intNo);
		if(r->intNo == 14)
		{	bootTerminal::prints(" EIP= ");
			bootTerminal::print10(r->eip);
			bootTerminal::prints(" CR2= ");
			bootTerminal::print10(readCR2());
		}
		bootTerminal::printc('\n');
	}
	
	if(PIC::usingPIC() and r->intNo >= PIC::REMAP_START && r->intNo < PIC::REMAP_START + 16)
	{
		// call irq handler here
		
		PIC::sendEOI(r->intNo);
	}
}

