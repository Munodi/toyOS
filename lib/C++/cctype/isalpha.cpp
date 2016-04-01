#include <cctype>

int std::isalpha(int c)
{
    return (std::isupper(c) or std::islower(c));
}
