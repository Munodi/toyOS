#include <cctype>

int std::isdigit(int c)
{
    if(c >= '0' and c <= '9')
        return true;
    return false;
}
