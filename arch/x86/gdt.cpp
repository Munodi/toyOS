#ifdef x86

#include <cstdint>
#include <cstdlib>
#include <toyOS/config.hpp>
#include <toyOS/arch/x86/gdt.hpp>
#include <toyOS/bootTerminal.hpp>

using std::uint32_t;
using std::uint8_t;

// This structure contains the value of one GDT entry.
// We use the attribute 'packed' to tell GCC not to change
// any of the alignment in the structure.
struct gdt_entry_t
{
   std::uint16_t limit_low;           // The lower 16 bits of the limit.
   std::uint16_t base_low;            // The lower 16 bits of the base.
   std::uint8_t  base_middle;         // The next 8 bits of the base.
   std::uint8_t  access;              // Access flags, determine what ring this segment can be used in.
   std::uint8_t  granularity;
   std::uint8_t  base_high;           // The last 8 bits of the base.
} __attribute__((packed));

struct gdt_ptr_t
{
   std::uint16_t limit;               // The upper 16 bits of all selector limits.
   std::uint32_t base;                // The address of the first gdt_entry_t struct.
} __attribute__((packed));

static gdt_entry_t gdt_entries[5];
static gdt_ptr_t gdt_ptr;
static void gdt_set_gate(std::size_t, uint32_t, uint32_t, uint8_t, uint8_t);
extern "C" void gdt_flush (uint32_t);

void gdt::init()
{
    gdt_ptr.limit = sizeof(gdt_entry_t) * 5 - 1;
    gdt_ptr.base  = (std::uint32_t)&gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment
    // Inform the CPU about our GDT.
    gdt_flush ((uint32_t) &gdt_ptr);
}

// Set the value of one GDT entry.
static void gdt_set_gate(std::size_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
   gdt_entries[num].base_low    = (base & 0xFFFF);
   gdt_entries[num].base_middle = (base >> 16) & 0xFF;
   gdt_entries[num].base_high   = (base >> 24) & 0xFF;

   gdt_entries[num].limit_low   = (limit & 0xFFFF);
   gdt_entries[num].granularity = (limit >> 16) & 0x0F;

   gdt_entries[num].granularity |= gran & 0xF0;
   gdt_entries[num].access      = access;
}


#endif // x86
