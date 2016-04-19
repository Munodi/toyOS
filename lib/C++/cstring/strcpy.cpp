
#include <cstring>
namespace std
{
	char *strcpy(char *dest, const char *src)
	{
		char *ret = dest;
		while((*dest++ = *src++));
		return ret;
	}
}
