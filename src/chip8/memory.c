#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "memory.h"


static uint8_t memory[MAX_MEM];

bool load_rom(char* filename) {
    FILE *fp;
    char ch;

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror("Error while reading file.\n");
        return false;
    }

    int i = 512;
    while ((ch = fgetc(fp)) != EOF) {
        memory[i++] = (uint8_t)ch;
    }

    fclose(fp);
    return true;
}

uint16_t fetch_instruction(uint16_t addr)
{
    uint16_t opcode;

    opcode = memory[addr] << 8 | memory[addr + 1];
    return opcode;
}
