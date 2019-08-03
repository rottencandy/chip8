#include <stdlib.h>
#include <stdio.h>

#include "memory.h"
#include "cpu.h"
#include "opcodes.h"
#include "screen.h"

#define START_LOC 512

void cpu_init(chip8reg_t* c, uint8_t width, uint8_t height)
{
    // Index reg
    c->I = 0;
    // Program counter
    c->PC = START_LOC;
    c->delay_timer = 0;
    c->sound_timer = 0;
    c->SP = 0;
    c->buffer = init_screen(width, height);
}

void emulate_cycle(chip8reg_t* c)
{
    c->opcode = fetch_instruction(c->PC);
    printf("\nReading instruction\n");
    printf("Instr = %x\n", c->opcode);
    uint8_t op = c->opcode >> 12;
    opcode_ptr[op](c);
    clear_buffer(c->buffer);
}

void end_cpu(chip8reg_t* c)
{
    end_screen(c->buffer);
}
