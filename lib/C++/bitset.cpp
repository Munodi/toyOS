#include <bitset>
#include <cstdint>
#include <cstring>
#include <cstddef>

/*
template <std::size_t N>
std::bitset<N>::bitset()
{
    memset(_array, 0, (N/32) + 1);
}
*/

/*
template <std::size_t N>
std::bitset<N>& std::bitset<N>::set ( std::size_t pos)
{
    std::uint32_t elementNumber = pos / 32;
    std::uint32_t bitNumber = pos % 32;
    _array[elementNumber] = (_array[elementNumber] |  (1 << bitNumber));
    return *this;
}

*/
