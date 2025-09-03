#include "instructions.hpp"
#include "cpu.hpp"
#include "memory.hpp"

// trap vector constants (put these in instructions.hpp if you prefer)
constexpr uint8_t TRAP_GETC = 0x20;
constexpr uint8_t TRAP_OUT  = 0x21;
constexpr uint8_t TRAP_PUTS = 0x22;
constexpr uint8_t TRAP_IN   = 0x23;
constexpr uint8_t TRAP_PUTSP= 0x24;
constexpr uint8_t TRAP_HALT = 0x25;

static bool acv_violation(CPU& cpu, uint16_t addr, const char* reason = "ACV")
{
    constexpr uint16_t PRIV_END = 0x2FFF;
    constexpr uint16_t PSR_PRIV = 1u << 15; // bit 15
    if (addr <= PRIV_END && (cpu.get_psr() & PSR_PRIV)) {
        cpu.raise_exception(reason);
        return true;
    }
    return false;
}

// OP_BR   (0)
void instr_br(CPU& reg, Memory& memory, uint16_t instr)
{
    // Get the condition code from the instruction
    uint8_t cond = (instr >> 9) & 0x7;

    // Check if the condition is met
    if (reg.check_condition(cond)) {
        // Get the offset and update the PC
        int16_t offset = static_cast<int16_t>((instr & 0x1FF) << 7) >> 7;
        reg[R_PC] += offset;
    }
}


// OP_ADD (1)
void instr_add(CPU& reg, Memory& memory, uint16_t instr)
{
    Register DEST = static_cast<Register>((instr >> 9) & 0x7);
    Register SRC1 = static_cast<Register>((instr >> 6) & 0x7);

    // Check bit [5] to determine addressing mode
    if ((instr >> 5) & 0x1) {
        // Immediate mode: sign-extend imm5
        int16_t imm5 = static_cast<int16_t>(instr & 0x1F);
        if (imm5 & 0x10) // sign bit of 5-bit immediate
            imm5 |= 0xFFE0;
        reg[DEST] = reg[SRC1] + imm5;
    } else {
        // Register mode
        Register SRC2 = static_cast<Register>(instr & 0x7);
        reg[DEST] = reg[SRC1] + reg[SRC2];
    }

    // Update condition codes after result
    reg.update_flags(DEST);
}

// OP_LD (2)
void instr_ld(CPU& reg, Memory& memory, uint16_t instr)
{
        Register REG = static_cast<Register>((instr >> 9) & 0x7);
        int16_t IR = static_cast<int16_t>((static_cast<int16_t>(instr << 7)) >> 7);
        reg[REG] = memory.mem_read(reg[R_PC] + IR);
        reg.update_flags(REG);
}

// OP_ST (3)
void instr_st(CPU& reg, Memory& memory, uint16_t instr)
{
    Register REG = static_cast<Register>((instr >> 9) & 0x7);
    int16_t IR = static_cast<int16_t>((static_cast<int16_t>(instr << 7)) >> 7);
    uint16_t addr = static_cast<uint16_t>(reg[R_PC] + IR);


    // ACV check
    if (acv_violation(reg, addr)) return;

    memory.mem_write(addr, reg[REG]);
}


// OP_JSR (4)
// Prototype kept in header; implement here when ready
void instr_jsr(CPU& reg, Memory& memory, uint16_t instr)
{
    // Save the return address (next instruction)
    reg[R_R7] = reg[R_PC];

    // Check addressing mode: bit[11] = 1 → JSR (PC-relative), 0 → JSRR (BaseR)
    if ((instr >> 11) & 1) {
        // JSR: sign-extend 11-bit offset
        int16_t offset = static_cast<int16_t>((instr & 0x7FF) << 5) >> 5;
        reg[R_PC] += offset;
    } else {
        // JSRR: bits[8:6] select Base Register
        Register baseR = static_cast<Register>((instr >> 6) & 0x7);
        reg[R_PC] = reg[baseR];
}
}


// OP_AND (5)
void instr_and(CPU& reg, Memory& memory, uint16_t instr)
{
    Register DEST = static_cast<Register>((instr >> 9) & 0x7);
    Register SRC1 = static_cast<Register>((instr >> 6) & 0x7);

    // Check bit [5] to determine addressing mode
    if ((instr >> 5) & 0x1) {
        // Immediate mode: sign-extend imm5
        int16_t imm5 = static_cast<int16_t>(instr & 0x1F);
        if (imm5 & 0x10) // sign bit of 5-bit immediate
            imm5 |= 0xFFE0;
        reg[DEST] = reg[SRC1] & imm5;
    } else {
        // Register mode
        Register SRC2 = static_cast<Register>(instr & 0x7);
        reg[DEST] = reg[SRC1] & reg[SRC2];
    }

    // Update condition codes after result
    reg.update_flags(DEST);
}

// OP_LDR (6)
void instr_ldr(CPU& reg, Memory& memory, uint16_t instr)
{
    Register DEST = static_cast<Register>((instr >> 9) & 0x7);
    Register BASE = static_cast<Register>((instr >> 6) & 0x7);
    std::uint16_t imm6   = instr & 0x3Fu;
    std::int16_t  offset = static_cast<std::int16_t>(
                               (static_cast<std::int16_t>(imm6 << 10)) >> 10);

    // Effective address calculation
    uint16_t addr = reg[BASE] + offset;
    // ACV check
    if (acv_violation(reg, addr)) return;

    reg[DEST] = memory.mem_read(addr);
    // Update condition codes after result
    reg.update_flags(DEST);
}

// OP_STR (7)
void instr_str(CPU& reg, Memory& memory, uint16_t instr)
{
    Register SRC = static_cast<Register>((instr >> 9) & 0x7);
    Register BASE = static_cast<Register>((instr >> 6) & 0x7);
    std::uint16_t imm6 = instr & 0x3Fu;
    std::int16_t offset = static_cast<std::int16_t>(
        (static_cast<std::int16_t>(imm6 << 10)) >> 10);

    // Effective address calculation
    uint16_t addr = reg[BASE] + offset;
    // ACV check
    if (acv_violation(reg, addr)) return;

    memory.mem_write(addr, reg[SRC]);
}


// OP_RTI (8) - usually unused
void instr_rti(CPU& reg, Memory& memory, uint16_t instr);


// OP_NOT (9)
void instr_not(CPU& reg, Memory& memory, uint16_t instr)
{
    Register DEST = static_cast<Register>((instr >> 9) & 0x7);
    Register SRC = static_cast<Register>((instr >> 6) & 0x7);
    reg[DEST] = ~reg[SRC];
    // Update condition codes after result
    reg.update_flags(DEST);
}


// OP_LDI (10)
void instr_ldi(CPU& reg, Memory& memory, uint16_t instr)
{
    Register DEST = static_cast<Register>((instr >> 9) & 0x7);
    int16_t SIGN_EXT = static_cast<int16_t>((static_cast<int16_t>(instr << 7)) >> 7);
    uint16_t addr = reg[R_PC] + SIGN_EXT;

    uint16_t final_addr = memory.mem_read(addr);
    reg[DEST] = memory.mem_read(final_addr);
    // Update condition codes after result
    reg.update_flags(DEST);
}


// OP_STI (11)
void instr_sti(CPU& reg, Memory& memory, uint16_t instr)
{
    Register REG = static_cast<Register>((instr >> 9) & 0x7);
    int16_t IR = static_cast<int16_t>((static_cast<int16_t>(instr << 7)) >> 7);
    uint16_t addr = static_cast<uint16_t>(reg[R_PC] + IR);

    // ACV: computed address in privileged memory range [0x0000..0x2FFF]
    // and PSR[15] == 1 (privileged) -> raise ACV exception
    // ACV check
    if (acv_violation(reg, addr)) return;

    // Indirection: first read pointer from memory
    uint16_t final_addr = memory.mem_read(addr);

    memory.mem_write(final_addr, reg[REG]);
}


// OP_JMP_RET (12)
void instr_jmp_ret(CPU& reg, Memory& memory, uint16_t instr)
{
    if ((instr & 0x01C0) == 0x01C0) {
        // RET instruction
        reg[R_PC] = reg[R_R7];
        return;
    }
    Register BASE = static_cast<Register>((instr >> 6) & 0x7);
    reg[R_R7] = reg[R_PC]; // for RET
    reg[R_PC] = reg[BASE];
}

// OP_RES (13) reserved / noop
// void instr_res(CPU& reg, Memory& memory, uint16_t instr); // reserved / noop


// OP_LEA (14)
void instr_lea(CPU& reg, Memory& memory, uint16_t instr)
{
    Register REG = static_cast<Register>((instr >> 9) & 0x7);
    int16_t IR = static_cast<int16_t>((static_cast<int16_t>(instr << 7)) >> 7);
    reg[REG] = reg[R_PC] + IR;
}


// OP_TRAP (15)
void instr_trap(CPU& reg, Memory& memory, uint16_t instr)
{
    uint8_t trapvect = instr & 0xFF;

    switch (trapvect)
    {
        case TRAP_GETC:
        {
            int input = std::cin.get();
            if (input != EOF) 
            {
                reg[R_R0] = static_cast<uint16_t>(static_cast<unsigned char>(input));
            }
            break;
        }
        case TRAP_OUT:
        {
            char c = static_cast<char>(reg[R_R0] & 0xFF);
            std::cout.put(c).flush();
            break;
        }
        case TRAP_PUTS:
        {
            uint16_t addr = reg[R_R0];
            auto* ch = memory.data() + addr;

            while (*ch) {
                std::cout.put(static_cast<char>(*ch & 0xFF));
                ++ch;
            }

            std::cout.flush();
            break;
        }
        case TRAP_IN:
        {
            std::cout << "Enter a character: ";
            int input = std::cin.get();
            if (input != EOF) {
                char c = static_cast<char>(input);
                std::cout.put(c); // echo
                reg[R_R0] = static_cast<uint16_t>(static_cast<unsigned char>(c));
            }
            break;
        }
        case TRAP_PUTSP:
        {
            uint16_t addr = reg[R_R0];
            uint16_t* ch = memory.data() + addr;

            while (*ch) {
                char char1 = static_cast<char>(*ch & 0xFF);
                std::cout.put(char1);

                char char2 = static_cast<char>((*ch >> 8) & 0xFF);
                if (char2) {
                    std::cout.put(char2);
                }

                ++ch;
            }
            std::cout.flush();
        }
        case TRAP_HALT:
        {
            std::cout << "HALT\n";
            abort();
            break;
        }
    }
}
