/**
 *
 * Typical macros are
 *  USERSPACE_START
 *  USERSPACE_END
 *  KERNEL_IMAGE_START
 *  KERNEL_IMAGE_END
 *  KERNEL_HEAP_START
 *  KERNEL_HEAP_END

  note that the *END macros imply a limit and don't need to be be filled up to
*/



#ifndef _TOYOS_MEMORY_LAYOUT_HPP_
#define _TOYOS_MEMORY_LAYOUT_HPP_

#if defined(x86)
    #include <toyOS/arch/x86/memoryLayout.hpp>
#elif defined(x64)
    #include <toyOS/arch/x64/memoryLayout.hpp>
#else
    #error "error in target architecture in _TOYOS_MEMORY_LAYOUT_HPP_"
#endif

#endif // _TOYOS_MEMORY_LAYOUT_HPP_
