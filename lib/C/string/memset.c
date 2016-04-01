// Copied from the Public Domain C Library (PDCLib). r519
#include <string.h>

void * memset( void * s, int c, size_t n )
{
    unsigned char * p = (unsigned char *) s;
    while ( n-- )
    {
        *p++ = (unsigned char) c;
    }
    return s;
}

