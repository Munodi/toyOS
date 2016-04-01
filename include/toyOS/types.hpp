/**
 *  toyOS/types.hpp defines types:
 *   physical_uintptr_t: an unsigned integer large enough to hold any physical address or physical maximum memory
 *
 */

#ifndef _TOYOS_TYPES_HPP_
#define _TOYOS_TYPES_HPP_
#include <cstdint>

#if defined(x86)
    typedef std::uint64_t physical_uintptr_t;
#elif defined(x64)
    typedef std::uint64_t physical_uintptr_t;
#else
    #error "unrecognised architecture, error in include/toyOS/typeshpp"
#endif

#endif // _TOYOS_TYPES_HPP_
