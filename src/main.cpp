#include "common.hpp"
#include "cpu.hpp"
#include "memory.hpp"
#include "io.hpp"
#include "loader.hpp"

int main(int argc, const char* argv[])
{


    std::cout << "Mini VM starting up...\n";
    // Initialize memory and CPU
    CPU cpu;

    for (int j = 1; j < argc; ++j)  // start at 1, because argv[0] is program name
    {
        const char* image_path = argv[j];
        if (!read_image(image_path))
        {
            printf("failed to load image: %s\n", argv[j]);
            exit(1);
        }
    }
    signal(SIGINT, handle_interrupt);
    disable_input_buffering();

    // Set initial CPU state
    cpu.reset();

    // Run VM
    cpu.run(memory);

    restore_input_buffering();

    /* since exactly one condition flag should be set at any given time, set the Z flag */
    /* CPU reg;
    reg[R_COND] = FL_ZRO;

    /* set the PC to starting position 
    /* 0x3000 is the default 
    enum { PC_START = 0x3000 };
    reg[R_PC] = PC_START;

    bool running = true;
    while (running)
    {


        // FETCH
        uint16_t instr = memory.mem_read(reg[R_PC]++);
        uint16_t op = instr >> 12;

        switch (op)
        {
            case OP_LD:
                {
                    Register REG = static_cast<Register>((instr >> 9) & 0x7);
                    int16_t IR = static_cast<int16_t>((static_cast<int16_t>(instr << 7)) >> 7);
                    reg[REG] = memory.mem_read(reg[R_PC] + IR);
                    reg.update_flags(reg[REG]);
                    break;
                }
                
        }
    }*/
   return 0;
}