#include <stdlib.h>
#include <stdio.h>

#include "opcodes.h"

// Single instruction composition(1 byte):
// 0123 4 5 67
// ADDR X Y KK
// ---- - - -N

#define ADDR(c) (c->opcode & 0x0FFF)
#define X(c) (c->opcode & 0x0F00 >> 8)
#define Y(c) (c->opcode & 0x00F0 >> 4)
#define N(c) (c->opcode & 0x000F)
#define K(c) (c->opcode & 0x00FF)

static void _0xxx(chip8reg_t* c)
{
    // Ignore operation
    if(K(c) == 0x00)
    {
        c->PC += 2;
    }// Clear screen
    else if(K(c) == 0xE0)
    {
        clear_buffer(c->buffer);
    }
    // Return from subroutine
    else if(K(c) == 0xEE)
    {
        // TODO handle empty stack
        c->SP --;
    }
}

// Jump to address xxx
static void _1xxx(chip8reg_t* c)
{
    c->PC = ADDR(c);
}

// Call subroutine at xxx
static void _2xxx(chip8reg_t* c)
{
    c->stack[c->SP++] = c->PC;
    c->PC = ADDR(c);
}

// Skip following instruction if Vx value is equal to n
static void _3xxx(chip8reg_t* c)
{
    if(c->V[X(c)] == K(c))
    {
        c->PC += 4;
    }
    else
        c->PC += 2;
}

// Skip following instruction if Vx value is not equal
static void _4xxx(chip8reg_t* c)
{
    if(c->V[X(c)] != K(c))
    {
        c->PC += 4;
    }
    else
        c->PC += 2;
}

// Skip following instruction if Vx == Vy
static void _5xxx(chip8reg_t* c)
{
    if(c->V[X(c)] == c->V[Y(c)])
    {
        c->PC += 4;
    }
    else
        c->PC += 2;
}

// Store value in reg Vx
static void _6xxx(chip8reg_t* c)
{
    printf("executing 6\n");
    c->V[X(c)] = K(c);
    c->PC += 2;
}

// Add value to Vx
static void _7xxx(chip8reg_t* c)
{
    c->V[X(c)] += K(c);
    c->PC += 2;
}

static void _8xxx(chip8reg_t* c)
{
    // Copy Vy to Vx
    if(N(c) == 0x000)
    {
        c->V[X(c)] = c->V[Y(c)];
    }

    // Vx = Vx OR Vy
    else if(N(c) == 0x001)
    {
        c->V[X(c)] |= c->V[Y(c)];
    }

    // Vx = Vx AND Vy
    else if(N(c) == 0x002)
    {
        c->V[X(c)] &= c->V[Y(c)];
    }

    // Vx = Vx XOR Vy
    else if(N(c) == 0x003)
    {
        c->V[X(c)] ^= c->V[Y(c)];
    }

    // Vx = Vx + Vy. Set carry to VF
    else if(N(c) == 0x004)
    {
        c->V[15] = (c->V[Y(c)] > 0xFF - c->V[X(c)]) ? 1:0;
        c->V[X(c)] += c->V[Y(c)];
    }

    // Vx = Vx - Vy. VF = 1 if NO borrow
    else if(N(c) == 0x005)
    {
        c->V[15] = (c->V[Y(c)] > c->V[X(c)]) ? 1:0;
        c->V[X(c)] -= c->V[Y(c)];
    }

    // Vx = Vy >> 1. VF = MSB before shift
    else if(N(c) == 0x006)
    {
        c->V[15] = (c->V[X(c)] & 0x01)? 1:0;
        c->V[X(c)] = c->V[Y(c)] >> 1;
    }

    // Vx = Vy - Vx. VF = 1 if No borrow
    else if(N(c) == 0x007)
    {
        c->V[15] = (c->V[X(c)] > c->V[Y(c)]) ? 1:0;
        c->V[X(c)] = c->V[Y(c)] - c->V[X(c)];
    }

    // Vx == Vy << 1. VF = MSB before shift
    else if(N(c) == 0x00E)
    {
        c->V[15] = (c->V[X(c)] & 0x80)? 1:0;
        c->V[X(c)] = c->V[Y(c)] << 1;
    }
    // TODO handle unknown instructions.
    c->PC += 2;
}

// Skip next instruction if Vx != Vy
static void _9xxx(chip8reg_t* c)
{
    if(c->V[X(c)] != c->V[X(c)])
    {
        c->PC += 4;
    }
    else
        c->PC += 2;
}

// Store xxx in reg I
static void _Axxx(chip8reg_t* c)
{
    c->I = ADDR(c);
    c->PC += 2;
}

// Jump to xxx + V0
static void _Bxxx(chip8reg_t* c)
{
    c->PC = c->V[0] + ADDR(c);
}

// Set Vx to random number with mask nn
static void _Cxxx(chip8reg_t* c)
{
    c->V[X(c)] = (rand() % 0xFF) & K(c);
}

// Draw sprite at (Vx,Vy) and n  sprites stored at address in I.
// VF = 01 if any set pixels are changed to unset.(collision detection)
static void _Dxxx(chip8reg_t* c)
{
}

static void _Exxx(chip8reg_t* c)
{
// Skip instruction if key in Vx is pressed
    if(K(c) == 0x9E)
    {
    }
// Skip instruction if key in Vx is not pressed
    else if(K(c) == 0xA1)
    {
    }
}

static void _Fxxx(chip8reg_t* c)
{
    // Store delay timer value in Vx
    if(K(c) == 0x07)
    {
        c->V[V(c)] = c->delay_timer;
    }

    // Wait for keypress and store value in Vx
    else if(K(c) == 0x0A)
    {
    }

    // Set delay timer to Vx
    else if(K(c) == 0x15)
    {
        c->delay_timer = c->V[V(c)];
    }

    // Set sound timer to Vx
    else if(K(c) == 0x18)
    {
        c->sound_timer = c->V[V(c)];
    }

    // Add Vx to I
    else if(K(c) == 0x1E)
    {
    }

    // Set I to location of sprite in Vx
    else if(K(c) == 0x29)
    {
    }

    // Store BCD of Vx value to I, I+1, I+2
    else if(K(c) == 0x33)
    {
    }

    // Store V0-Vx in I-...
    else if(K(c) == 0x55)
    {
    }

    // Copy I-... to V0-Vx
    else if(K(c) == 0x65)
    {
    }
    else
    {
    //TODO handle invalid instruction.
    }
}

void (*opcode_ptr[16])(chip8reg_t* c) = {
    _0xxx, _1xxx, _2xxx, _3xxx,
    _4xxx, _5xxx, _6xxx, _7xxx,
    _8xxx, _9xxx, _Axxx, _Bxxx,
    _Cxxx, _Dxxx, _Exxx, _Fxxx
};

uint8_t chip8_fontset[80] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};
