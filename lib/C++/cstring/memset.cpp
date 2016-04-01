// Copied from the Public Domain C Library (PDCLib). r519
#include <cstring>

namespace std
{
    void * memset( void * s, int c, std::size_t n )
    {
        unsigned char * p = (unsigned char *) s;
        while ( n-- )
        {
            *p++ = (unsigned char) c;
        }
        return s;
    }
} //namespace std


