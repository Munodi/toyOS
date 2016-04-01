#ifndef _TOYOS_ARCH_X86_PAGING_HPP_
#define _TOYOS_ARCH_X86_PAGING_HPP_

// defines what page tale entry bits mean, x means it doesn't matter what the bit is
#define UNALLOCATED_PAGE    "00000000000000000000000000000000" // 32 0 bits
#define MARKED_AS_BSS       "xxxxxxxxxxxxxxxxxxxxxx1xxxxxxxx0" // not present, ignored set to 1
#define COPY_ON_WRITE       "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx01" // only copy-on-write in a writable section


#endif // _TOYOS_ARCH_X86_PAGING_HPP_
