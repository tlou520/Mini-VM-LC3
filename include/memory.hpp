#pragma once
#ifndef MEMORY_HPP
#define MEMORY_HPP



// Global type alias, visible to all files that include memory.hpp
constexpr std::size_t MEMORY_MAX = 1 << 16;
using MemArray = std::array<uint16_t, MEMORY_MAX>;

enum MMIORegisters {
    MR_KBSR = 0xFE00,  /* keyboard status */
    MR_KBDR = 0xFE02   /* keyboard data */
};

class Memory {
public:
    void mem_write(uint16_t address, uint16_t val);
    uint16_t mem_read(uint16_t address);

    // Provide access to the underlying memory buffer:
    // - uint16_t* data() returns a mutable pointer for callers that will modify memory.
    uint16_t* data();

    private:
        MemArray memory{};  // the actual memory storage
};

// Declare the global object
extern Memory memory;

#endif
