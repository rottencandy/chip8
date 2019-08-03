#pragma once

#include <stdbool.h>
#include <stdint.h>

#define MAX_STACK 16

typedef struct {
    uint8_t V[16];

    uint16_t opcode;

    uint16_t I;
    uint16_t PC;

    uint8_t delay_timer;
    uint8_t sound_timer;

    uint8_t SP;
    uint16_t stack[MAX_STACK];

    uint8_t* buffer;
} chip8reg_t;


void cpu_init(chip8reg_t*, uint8_t, uint8_t);
void emulate_cycle(chip8reg_t*);
void end_cpu(chip8reg_t*);
