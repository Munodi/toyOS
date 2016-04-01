#include <cctype>

int std::isalnum(int c)
{
    return (std::isalpha(c) or std::isdigit(c));
}
