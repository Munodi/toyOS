#include <cctype>

int std::isupper(int c)
{
    if(c >= 'A' and c <= 'Z')
        return true;
    return false;
}
