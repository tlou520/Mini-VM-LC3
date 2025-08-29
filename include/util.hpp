#pragma once
#ifndef UTIL_HPP
#define UTIL_HPP

// Swaps byte order of a 16-bit unsigned integer (big ↔ little endian)
constexpr uint16_t swap16(uint16_t x)
{
    return (x << 8) | (x >> 8);
}

#endif // UTIL_HPP
