#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chip8/cpu.h"
#include "chip8/screen.h"
#include "chip8/memory.h"


int main(int argc, char* argv[]) {
    if(argc != 2) {
        printf("Incorrect usage.\n");
        return 1;
    }

    chip8reg_t c;
    cpu_init(&c, 64, 48);
    printf("cpu loaded.\n");
    if(load_rom(argv[1])) {
        printf("ROM loaded.\n");
    }

    for(int i = 0; i < 1; i++)
    {
        emulate_cycle(&c);
    }
    end_cpu(&c);
    return 0;
}
