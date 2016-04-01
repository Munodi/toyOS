/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    partial string.h
    
    George Robertson
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/



#ifndef _STRING_H_
#define _STRING_H_

#include <stdlib.h>


extern size_t   strlen(const char *s);
extern int      strcmp(const char *, const char *);
extern void     *memset(void *, int, size_t);
extern void     *memcpy(void *dest, const void *src, size_t n);

#endif /* _STDLIB_H_ */
