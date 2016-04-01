#ifndef _TOYOS_ARCH_X86_IRQ_HPP_
#define _TOYOS_ARCH_X86_IRQ_HPP_

/*
    On the x86 there are 3 types on interrupts, IRQs are what hardware generate
    to signal it has finished something. When an IRQ occurs a message should be
    sent to the appropiate driver


    0 	Programmable Interrupt Timer Interrupt
    1 	Keyboard Interrupt
    2 	Cascade (used internally by the two PICs. never raised)
    3 	COM2 (if enabled)
    4 	COM1 (if enabled)
    6 	Floppy Disk
    7 	Unreliable "spurious" interrupt (usually)
    8 	CMOS real-time clock (if enabled)
    14 	Primary ATA Hard Disk
    15 	Secondary ATA Hard Disk

*/

#endif // _TOYOS_ARCH_X86_IRQ_HPP_
