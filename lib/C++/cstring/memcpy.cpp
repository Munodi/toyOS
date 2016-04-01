/* note: taken from PDCLIB, thanks for this great library */

#include <cstring>
namespace std
{
	void * memcpy( void * s1, const void * s2, size_t n )
	{
	    char * dest = (char *) s1;
	    const char * src = (const char *) s2;
	    while ( n-- )
	    {
	        *dest++ = *src++;
	    }
	    return s1;
	}
}
