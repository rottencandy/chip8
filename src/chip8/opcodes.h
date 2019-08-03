#pragma once

#include <stdint.h>
#include "memory.h"
#include "cpu.h"
#include "screen.h"

void (*opcode_ptr[16])(chip8reg_t*);
uint8_t chip8_fontset[80];
