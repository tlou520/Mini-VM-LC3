#pragma once
#include "memory.hpp"

    enum Register {
        R_R0 = 0,
        R_R1,
        R_R2,
        R_R3,
        R_R4,
        R_R5,
        R_R6,
        R_R7,
        R_PC,    // program counter
        R_COND,  // condition flags
        R_COUNT
    };

    enum Cflags {
        FL_POS = 1 << 0, /* P */
        FL_ZRO = 1 << 1, /* Z */
        FL_NEG = 1 << 2, /* N */
    };

    enum Opcodes {
        OP_BR = 0, /* branch */
        OP_ADD,    /* add  */
        OP_LD,     /* load */
        OP_ST,     /* store */
        OP_JSR,    /* jump register */
        OP_AND,    /* bitwise and */
        OP_LDR,    /* load register */
        OP_STR,    /* store register */
        OP_RTI,    /* unused */
        OP_NOT,    /* bitwise not */
        OP_LDI,    /* load indirect */
        OP_STI,    /* store indirect */
        OP_JMP_RET,    /* jump */
        OP_RES,    /* reserved (unused) */
        OP_LEA,    /* load effective address */
        OP_TRAP    /* execute trap */
    };


   class CPU {
    public:
        CPU();                     // Constructor to initialize registers if needed
        // Allow array-like access to registers
        void reset();
        void step(Memory& mem);
        void run(Memory& mem);

        uint16_t& operator[](Register r);
        const uint16_t& operator[](Register r) const;

        // Update condition flags based on the value in register `r`
        void update_flags(Register r);
        bool check_condition(uint8_t cond);

        uint16_t get_psr() const;
        void set_psr(uint16_t v);

        void raise_exception(const char* reason);

    private:
        std::array<uint16_t, R_COUNT> reg;
        uint16_t psr;  // Program Status Register
    };



