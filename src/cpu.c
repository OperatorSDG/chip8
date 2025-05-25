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
    memcpy(&chip8.memory[FONTSET_START_ADDRESS], chip8_fontset, sizeof(chip8_fontset));
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

void cpu_cycle(void) {
    // Instruction Fetch
    uint16_t opcode = (chip8.memory[chip8.pc] << 8 | chip8.memory[chip8.pc + 1]);
    chip8.pc += 2;

    // Instruction Decode and Execute
    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode) {
                case 0x00E0: { // CLS - Clear Screen
                    display_clear();
                    break;
                }
                case 0x00EE: { // RET - Return drom subroutine
                    chip8.sp--;
                    chip8.pc = chip8.stack[chip8.sp];
                    break;
                }
            }
            break;

        case 0x1000: { // 1nnn - JP addr
            chip8.pc = opcode & 0x0FFF;
            break;
        }
        
        case 0x2000: { // 2nnn - CALL addr
            chip8.stack[chip8.sp] = chip8.pc;
            chip8.sp++;
            chip8.pc = opcode & 0x0FFF;
            break;
        }

        case 0x3000: { // 3xkk - SE Vx, kk
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t byte = (opcode & 0x00FF);
            if (chip8.V[x] == byte) {
                chip8.pc += 2;
            } 
            break;
        }

        case 0x4000: { // 3xkk - SE Vx, kk
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t byte = (opcode & 0x00FF);
            if (chip8.V[x] != byte) {
                chip8.pc += 2;
            } 
            break;
        }

        case 0x5000: { // 5xy0 - SE Vx, Vy
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4;
            if (chip8.V[x] == chip8.V[y]) {
                chip8.pc += 2;
            }
            break;
        }

        case 0x6000: { // 6xkk - LD Vx, kk
            uint8_t x = (opcode & 0x0F00) >> 8;
            chip8.V[x] = opcode & 0x00FF;
            break;
        }

        case 0x7000: { // 7xkk - ADD Vx, kk
            uint8_t x = (opcode & 0x0F00) >> 8;
            chip8.V[x] += opcode & 0x0FFF;
            break;
        }

        case 0x8000: {
            switch (opcode & 0xF00F)
            {
            case 0x8000:
                break;
            
            default:
                break;
            }
        }

        case 0xA000: { // ANNN - LD i, addr
            chip8.I = opcode & 0x0FFF;
            break;
        }

        case 0xD000: { //Dxyn - DRW Vx, Vy, nibble
            uint8_t x = chip8.V[(opcode & 0x0F00) >> 8];
            uint8_t y = chip8.V[(opcode & 0x00F0) >> 4];
            uint8_t height = opcode & 0x000F;

            chip8.V[0xF] = 0;

            for(int row = 0; row < height; row++) {
                uint8_t sprite_byte = chip8.memory[chip8.I + row];
                for(int col = 0; col < 8; col++) {
                    uint8_t sprite_pixel = (sprite_byte >> (7 - col)) & 1;
                    uint8_t* screen_pixel = &display[(y + row) % DISPLAY_HEIGHT][(x + col) % DISPLAY_WIDTH];

                    if (sprite_pixel) {
                        if (*screen_pixel == 1) {
                            chip8.V[0xF] = 1;
                        }
                        *screen_pixel ^= 1;
                    }
                }
            }
            
            display_render();
            break;
        }

        default:
            printf("Unknown opcode: 0x%04X\n", opcode);
            break;
    }

    // Update Timers
    if (chip8.delay_timer > 0) chip8.delay_timer--;
    if (chip8.sound_timer > 0) chip8.sound_timer--;
}