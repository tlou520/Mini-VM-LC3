#include "cpu.hpp"
#include "instructions.hpp"


    CPU::CPU() {
    }

    void CPU::reset()
    {
        reg.fill(0);
        reg[R_COND] = FL_ZRO;
        reg[R_PC] = 0x3000; // default start
    }

    void CPU::step(Memory& mem)
    {
        uint16_t instr = mem.mem_read(reg[R_PC]++);
        uint16_t op = instr >> 12;

        switch (op)
        {
            case OP_BR: instr_br(*this, mem, instr); break;
            case OP_ADD: instr_add(*this, mem, instr); break;
            case OP_LD: instr_ld(*this, mem, instr); break;
            case OP_ST: instr_st(*this, mem, instr); break;
            case OP_JSR: instr_jsr(*this, mem, instr); break;
            case OP_AND: instr_and(*this, mem, instr); break;
            case OP_LDR: instr_ldr(*this, mem, instr); break;
            case OP_STR: instr_str(*this, mem, instr); break;
            case OP_NOT: instr_not(*this, mem, instr); break;
            case OP_LDI: instr_ldi(*this, mem, instr); break;
            case OP_STI: instr_sti(*this, mem, instr); break;
            case OP_JMP_RET: instr_jmp_ret(*this, mem, instr); break;
            case OP_LEA: instr_lea(*this, mem, instr); break;
            case OP_TRAP: instr_trap(*this, mem, instr); break;
            case OP_RTI: break;
            case OP_RES: break;
            default: std::cout << "wrong opcode" << std::endl; /* handle bad opcode */ break;
        }
    }

    void CPU::run(Memory& mem)
    {
        bool running = true;
        while (running)
        {
            step(mem);
            // you can add break condition if HALT instruction is executed
        }
    }



    uint16_t& CPU::operator[](Register r) {
        return reg[r];
    }

    const uint16_t& CPU::operator[](Register r) const {
        return reg[r];
    }

    // Update condition flags based on register value
    void CPU::update_flags(Register r) {
        if (this->reg[r] == 0) {
            this->reg[R_COND] = FL_ZRO;
        } else if (this->reg[r] >> 15) { // negative if top bit set
            this->reg[R_COND] = FL_NEG;
        } else {
            this->reg[R_COND] = FL_POS;
        }
    }

    bool CPU::check_condition(uint8_t cond) {
        if (cond & 0x1 && this->reg[R_COND] == FL_POS) return true; // Positive
        if (cond & 0x2 && this->reg[R_COND] == FL_ZRO) return true; // Zero
        if (cond & 0x4 && this->reg[R_COND] == FL_NEG) return true; // Negative
        return false;
    }

    uint16_t CPU::get_psr() const {
        return psr;
    }

    void CPU::set_psr(uint16_t v) {
        psr = v;
    }

    void CPU::raise_exception(const char* reason) {
        // placeholder: log and stop. Replace with real vector/exception flow later.
        std::cerr << "Exception raised: " << reason << "\n";
        // In a full implementation you'd:
        //  - save state (PC, PSR) onto supervisor stack
        //  - set PSR[15]=1 (enter privileged mode)
        //  - load PC from the appropriate vector table entry
        // For now abort so the problem is visible.
        std::exit(EXIT_FAILURE);
    }