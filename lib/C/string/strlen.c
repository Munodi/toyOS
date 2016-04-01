// Copied from the Public Domain C Library (PDCLib). r519
#include <string.h>

size_t strlen( const char * s )
{
    size_t rc = 0;
    while ( s[rc] )
    {
        ++rc;
    }
    return rc;
}
