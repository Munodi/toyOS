/********************************************************************
    This is top secret intellectual property.
    George Robertson 2011
********************************************************************/

#include <cstdlib>

namespace std
{
    long labs(long i) /* faster way would be to OR the sign bit */
    {
        if(i<0) return -i;
        return i;
    }
} // namespace std


