#pragma once

#include <cstdint>

static inline
void outb( unsigned short port, std::uint8_t val )
{
    asm volatile( "outb %0, %1"
                  : : "a"(val), "Nd"(port) );
}

static inline
void out8( unsigned short port, std::uint8_t val )
{
    asm volatile( "outb %0, %1"
                  : : "a"(val), "Nd"(port) );
}

static inline
unsigned char inb( unsigned short port )
{
    unsigned char ret;
    asm volatile( "inb %1, %0"
                  : "=a"(ret) : "Nd"(port) );
    return ret;
}

static inline
unsigned char in8( unsigned short port )
{
    unsigned char ret;
    asm volatile( "inb %1, %0"
                  : "=a"(ret) : "Nd"(port) );
    return ret;
}

static inline
void io_wait( void )
{
    // port 0x80 is used for 'checkpoints' during POST.
    // The Linux kernel seems to think it is free for use :-/
    asm volatile( "outb %%al, $0x80"
                  : : "a"(0) );
}

