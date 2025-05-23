#include "../include/cpu.h"
#include "../include/display.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Chip8 chip8;

static const uint8_t chip8_fontset[80] = {
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

void cpu_init(void) {
    // Clear CPU State
    memset(&chip8, 0, sizeof(chip8));

    // Set program counter to start of program designation
    chip8.pc = PROGRAM_START_ADDRESS;

    // Load fontset into memory at 0x50
    memcpy(FONTSET_START_ADDRESS, chip8_fontset, sizeof(chip8_fontset));
    display_clear();
}

void cpu_load_rom(const char* path) {
    // Reading the ROM from path
    FILE* rom = fopen(path, "rb");
    if (!rom) {
        fprintf(stderr, "Failed to open ROM: %s\n", path);
        exit(EXIT_FAILURE);
    }

    // Aquire ROM size & handle if exceeds
    fseek(rom, 0, SEEK_END);
    long rom_size = ftell(rom); // Reads ROM size in bytes
    rewind(rom);

    if (rom_size > (MEMORY_SIZE - PROGRAM_START_ADDRESS)) {
        fprintf(stderr, "ROM too large to fit in memory.\n");
        fclose(rom);
        exit(EXIT_FAILURE);
    }

    // Load ROM into memory
    fread(&chip8.memory[PROGRAM_START_ADDRESS],sizeof(uint8_t), rom_size, rom);

    fclose(rom);
}