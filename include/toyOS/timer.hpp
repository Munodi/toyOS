#pragma once

#include <cstdlib>
#include <cstdint>
#include <toyOS/irqHandler.hpp>
//std::uint64_t unixTime;
std::uint64_t millis = 0;

class Timer : public IrqHandler
{
};
