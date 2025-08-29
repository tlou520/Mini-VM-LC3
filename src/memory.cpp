#include "memory.hpp"
#include "io.hpp"
// Define the global object
Memory memory;

// Function definitions
void Memory::mem_write(uint16_t address, uint16_t val) {
    this->memory[address] = val;
}

uint16_t Memory::mem_read(uint16_t address) {
        if (address == MR_KBSR)
        {
            if (check_key())
            {
                this->memory[MR_KBSR] = (1 << 15);
                this->memory[MR_KBDR] = getchar();
            }
            else
            {
                this->memory[MR_KBSR] = 0;
            }
        }
    return this->memory[address];
}

// data() accessors
uint16_t* Memory::data() { return this->memory.data(); }

