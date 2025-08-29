#pragma once

class CPU;
class Memory;

// Instruction prototypes in opcode order (OP_BR = 0 .. OP_TRAP = 15).
// Uncomment the prototypes you want to implement.

 // OP_BR   (0)
 void instr_br(CPU& reg, Memory& memory, uint16_t instr);

 // OP_ADD  (1)
 void instr_add(CPU& reg, Memory& memory, uint16_t instr);

 // OP_LD   (2) -- implemented
void instr_ld(CPU& reg, Memory& memory, uint16_t instr);

 // OP_ST   (3)
 void instr_st(CPU& reg, Memory& memory, uint16_t instr);

 // OP_JSR  (4) -- prototype kept (implement if not already)
void instr_jsr(CPU& reg, Memory& memory, uint16_t instr);

 // OP_AND  (5)
 void instr_and(CPU& reg, Memory& memory, uint16_t instr);

 // OP_LDR  (6)
 void instr_ldr(CPU& reg, Memory& memory, uint16_t instr);

 // OP_STR  (7)
 void instr_str(CPU& reg, Memory& memory, uint16_t instr);

 // OP_RTI  (8) - usually unused
void instr_rti(CPU& reg, Memory& memory, uint16_t instr); // usually unused

 // OP_NOT  (9)
 void instr_not(CPU& reg, Memory& memory, uint16_t instr);

 // OP_LDI  (10)
 void instr_ldi(CPU& reg, Memory& memory, uint16_t instr);

 // OP_STI  (11)
 void instr_sti(CPU& reg, Memory& memory, uint16_t instr);

 // OP_JMP  (12)
 void instr_jmp_ret(CPU& reg, Memory& memory, uint16_t instr);

 // OP_RES  (13)
 // void instr_res(CPU& reg, Memory& memory, uint16_t instr); // reserved / noop

 // OP_LEA  (14) -- implemented
void instr_lea(CPU& reg, Memory& memory, uint16_t instr);

 // OP_TRAP (15) -- implemented
void instr_trap(CPU& reg, Memory& memory, uint16_t instr);

