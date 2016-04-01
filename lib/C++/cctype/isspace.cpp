#include <cctype>

int std::isspace(int c)
{
    switch(c)
    {
        case ' ':
        case '\t':
        case '\n':
        case '\v':
        case '\f':
        case '\r': return true;
        default: return false;
    }
}
