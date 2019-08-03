#pragma once

#include <stdbool.h>
#include <stdint.h>

#define MAX_MEM 4096


bool load_rom(char*);
uint16_t fetch_instruction(uint16_t);
