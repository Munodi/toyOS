/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    partial stddef.h

    George Robertson
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/



#ifndef _STDDEF_H_
#define _STDDEF_H_

#if defined(__SIZE_TYPE__)
    typedef __SIZE_TYPE__ size_t;
#else
    #warning "__SIZE_TYPE__ is not defined, defaulting size_t to unsigned int"
    typedef unsigned int size_t;
#endif

#if __SIZEOF_SIZE_T__ == __SIZEOF_INT__
    typedef signed int ssize_t;
#elif __SIZEOF_SIZE_T__ == __SIZEOF_LONG__
    typedef signed long ssize_t;
#elif __SIZEOF_SIZE_T__ == __SIZEOF_SHORT__
    typedef signed short ssize_t;
#else
    #error "unable to typedef ssize_t"
#endif

#endif /* _STDDEF_H_ */
