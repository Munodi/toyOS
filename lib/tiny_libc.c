#ifdef hnjhvhvgvmnbhgvgbklwjdmkckdmc

/* Tiny implementation of the C library
   These functions are *not* optimised
   Currently supported functions are;
   abs, labs, strlen, memcpy, memset
   (C) George Robertson 2010, freely distribute as long as you give credit where due
*/

#include <stdlib.h>
#include <string.h>

int abs(int i)
{
    if(i<0) return -i;
    return i;
}

long labs(long i)
{
    if(i<0) return -i;
    return i;
}

size_t strlen(const char *s)
{
    size_t length = 0;
    while(s[length] != '\0')
    {
        ++length;
    }
    return length;
}

void *memcpy(void *destination, const void *source, size_t num) /* This will cause a warning */
{
        size_t i;
        char *dest;
        char *src;
        dest = destination;
        src = source;
        for(i = 0; i < num; i++)
        {
                dest[i] = src[i];
        }
        return destination;
}

void *memset(void *ptr, int value, size_t num)
{
	char *cptr = ptr;
	char val = (unsigned char)value;
	size_t i;
	for(i = 0; i < num; i++)
	{
		cptr[i] = val;
	}
	return ptr;
}


#endif
