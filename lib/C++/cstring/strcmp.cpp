/* note: taken from PDCLIB, thanks for this great library */

#include <cstring>
namespace std
{
	int strcmp( const char * s1, const char * s2 )
	{
	    while ( ( *s1 ) && ( *s1 == *s2 ) )
	    {
	        ++s1;
	        ++s2;
	    }
	    return ( *(unsigned char *)s1 - *(unsigned char *)s2 );
	}
}
