/********************************************************************
    USE TAKE COPY FREE. GO ON, WHY WOULD I SUE YOU OVER THIS.
    copyleft George Robertson 2011
********************************************************************/

#include <cstdlib>

namespace std
{
    int abs(int i) /* faster way would be to OR the sign bit */
    {
        if(i<0) return -i;
        return i;
    }
} //namespace std


