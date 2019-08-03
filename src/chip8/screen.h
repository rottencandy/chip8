#pragma once

#include <stdint.h>

#define PIXEL_SIZE 10


uint8_t* init_screen(uint8_t, uint8_t);
void display_draw(uint8_t*);
void clear_buffer(uint8_t*);
void end_screen(uint8_t*);
