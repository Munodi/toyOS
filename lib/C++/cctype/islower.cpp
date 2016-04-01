#include <cctype>

int std::islower(int c)
{
    if(c >= 'a' and c <= 'z')
        return true;
    return false;
}
