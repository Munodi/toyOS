#include <cctype>

int std::toupper(int c)
{
    if(std::islower(c))
        return c - 'a' + 'A';
    return c;
}
