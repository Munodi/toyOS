/* 
 * Copyright (c) 2011 George Robertson
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE. 
 */


#ifndef _STDINT_H_
#define _STDINT_H_


/* This, if __INT*_TYPE__ is undefined, it is defined with something like signed short */

/* defining __INT8_TYPE__ & __UINT8_TYPE__ */
#ifndef __INT8_TYPE__
        #define __INT8_TYPE__ signed char
#endif

#ifndef __UINT8_TYPE__
        #define __UINT8_TYPE__ unsigned char
#endif

/* defining __INT16_TYPE__ & __UINT16_TYPE__ */
#ifndef __INT16_TYPE__
        #if defined(__SIZEOF_SHORT__)
                #if __SIZEOF_SHORT__ == 2
                        #define __INT16_TYPE__ signed short
                #endif
        #endif
#endif

#ifndef __UINT16_TYPE__
        #if defined(__SIZEOF_SHORT__)
                #if __SIZEOF_SHORT__ == 2
                        #define __UINT16_TYPE__ unsigned short
                #endif
        #endif
#endif

/* defining __INT32_TYPE__ & __UINT32_TYPE__ */
#ifndef __INT32_TYPE__
        #if defined(__SIZEOF_INT__)
                #if __SIZEOF_INT__ == 4
                        #define __INT32_TYPE__ signed int
                #endif
        #endif
#endif

#ifndef __UINT32_TYPE__
        #if defined(__SIZEOF_INT__)
                #if __SIZEOF_INT__ == 4
                        #define __UINT32_TYPE__ unsigned int
                #endif
        #endif
#endif

/* defining __INT64_TYPE__ & __UINT64_TYPE__ */
#ifndef __INT64_TYPE__
        #if defined(__SIZEOF_LONG_LONG__)
                #if __SIZEOF_LONG_LONG__ == 8
                        #define __INT64_TYPE__ signed long long
                #endif
        #endif
#endif

#ifndef __UINT64_TYPE__
        #if defined(__SIZEOF_LONG_LONG__)
                #if __SIZEOF_LONG_LONG__ == 8
                        #define __UINT64_TYPE__ unsigned long long
                #endif
        #endif
#endif

#if defined(__INT8_TYPE__) && defined(__INT16_TYPE__) && defined(__INT32_TYPE__) && defined(__INT64_TYPE__)
    typedef __INT8_TYPE__  int8_t;
    typedef __INT16_TYPE__ int16_t;
    typedef __INT32_TYPE__ int32_t;
    typedef __INT64_TYPE__ int64_t;
#else
    #error "__INT8_TYPE__ or __INT16_TYPE__ or __INT32_TYPE__ or __INT64_TYPE__ is not defined"
#endif

#if defined(__UINT8_TYPE__) && defined(__UINT16_TYPE__) && defined(__UINT32_TYPE__) && defined(__UINT64_TYPE__)
    typedef __UINT8_TYPE__  uint8_t;
    typedef __UINT16_TYPE__ uint16_t;
    typedef __UINT32_TYPE__ uint32_t;
    typedef __UINT64_TYPE__ uint64_t;
#else
    #error "__UINT8_TYPE__ or __UINT16_TYPE__ or __UINT32_TYPE__ or __UINT64_TYPE__ is not defined"
#endif

#if defined(__UINTPTR_TYPE__) && defined(__INTPTR_TYPE__)
    typedef __UINTPTR_TYPE__ uintptr_t;
    typedef __INTPTR_TYPE__  intptr_t;
#else
    #error "__UINTPTR_TYPE__ or __INTPTR_TYPE__ is not defined"
#endif

#endif /* _STDINT_H_ */
