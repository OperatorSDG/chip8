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

    // Default increment; will be skipped if instruction sets PC manually
    uint16_t next_pc = chip8.pc + 2;

    // Instruction Decode and Execute
    switch (opcode & 0xF000) {
        case 0x0000: { // 00**
            switch (opcode) {
                case 0x00E0: { // CLS
                    display_clear(); 
                    break;
                }
                case 0x00EE: { // RET
                    chip8.sp--;
                    next_pc = chip8.stack[chip8.sp];
                    break;
                }
                default:
                    printf("Unknown 0x opcode: 0x%04X\n", opcode);
                    break;
            }
            break;
        }

        case 0x1000: { // 1nnn - JP addr
            next_pc = opcode & 0x0FFF; 
            break;
        }

        case 0x2000: { // 2nnn - CALL addr
            chip8.stack[chip8.sp++] = chip8.pc + 2;
            next_pc = opcode & 0x0FFF;
            break;
        }

        case 0x3000: { // 3xkk - SE Vx, byte(kk)
            uint8_t x = (opcode & 0x0F00) >> 8;
            if (chip8.V[x] == (opcode & 0x00FF)) next_pc += 2;
            break;
        }

        case 0x4000: { // 4xkk - SNE Vx, byte(kk)
            uint8_t x = (opcode & 0x0F00) >> 8;
            if (chip8.V[x] != (opcode & 0x00FF)) next_pc += 2;
            break;
        }

        case 0x5000: { // 5xy0 - SE Vx, Vy
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4;
            if (chip8.V[x] == chip8.V[y]) next_pc += 2;
            break;
        }

        case 0x6000: { // 6xkk - LD Vx, byte(kk)
            uint8_t x = (opcode & 0x0F00) >> 8;
            chip8.V[x] = opcode & 0x00FF;
            break;
        }

        case 0x7000: { // 7xkk - ADD Vx, byte(kk)
            uint8_t x = (opcode & 0x0F00) >> 8;
            chip8.V[x] += opcode & 0x00FF;
            break;
        }

        case 0x8000: { // 8xy* 
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4;
            uint8_t vx = chip8.V[x];
            uint8_t vy = chip8.V[y];
            switch (opcode & 0x000F) {
                case 0x0: { // 8xy0 - LD Vx, Vy
                    chip8.V[x] = chip8.V[y]; 
                    break;
                }
                case 0x1: { // 8xy1 - OR Vx, Vy
                    chip8.V[x] |= chip8.V[y];
                    chip8.V[0xF] = 0; 
                    break;
                }
                case 0x2: { // 8xy2 - AND Vx, Vy
                    chip8.V[x] &= chip8.V[y]; 
                    chip8.V[0xF] = 0;
                    break;
                }
                case 0x3: { // 8xy3 - XOR Vx, Vy
                    chip8.V[x] ^= chip8.V[y];
                    chip8.V[0xF] = 0; 
                    break;
                }
                case 0x4: { // 8xy4 - ADD Vx, Vy
                    uint16_t sum = vx + vy;
                    chip8.V[x] = sum & 0xFF;
                    chip8.V[0xF] = sum > 0xFF;
                    break;
                }
                case 0x5: { // 8xy5 - SUB Vx, Vy
                    chip8.V[x] -= chip8.V[y];
                    chip8.V[0xF] = vx >= vy;
                    break;
                }
                case 0x6: { // 8xy6 - SHR Vx{, Vy}
                    chip8.V[x] = chip8.V[y];
                    chip8.V[x] >>= 1;
                    chip8.V[0xF] = vy & 0x1;
                    break;
                }
                case 0x7: { // 8xy7 - SUBN Vx, Vy
                    chip8.V[x] = chip8.V[y] - chip8.V[x];
                    chip8.V[0xF] = vy >= vx;
                    break;
                }
                case 0xE: { // 8xyE - SHL Vx{, Vy}
                    chip8.V[x] = chip8.V[y];
                    chip8.V[x] <<= 1;
                    chip8.V[0xF] = (vy & 0x80) >> 7;
                    break;
                }
                default:
                    printf("Unknown 8x opcode: 0x%04X\n", opcode);
                    break;
            }
            break;
        }

        case 0x9000: { // 9xy0 - SNE Vx, Vy
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4;
            if (chip8.V[x] != chip8.V[y]) next_pc += 2;
            break;
        }

        case 0xA000: { // Annn - LD I, addr
            chip8.I = opcode & 0x0FFF;
            break;
        }

        case 0xB000: { // Bnnn - JP V0, addr
            next_pc = chip8.V[0] + (opcode & 0x0FFF);
            break;
        }

        case 0xC000: { // Cxkk - RND Vx, byte(kk)
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t kk = opcode & 0x00FF;
            chip8.V[x] = (rand() % 256) & kk;
            break; 
        }

        case 0xD000: { // Dxyn - DRW Vx, Vy, nibble(n)
            uint8_t x = chip8.V[(opcode & 0x0F00) >> 8] % DISPLAY_WIDTH;
            uint8_t y = chip8.V[(opcode & 0x00F0) >> 4] % DISPLAY_HEIGHT;
            uint8_t height = opcode & 0x000F;
            chip8.V[0xF] = 0;

            for (int row = 0; row < height; row++) {
                if (y + row >= DISPLAY_HEIGHT) break; // clip at bottom
                uint8_t sprite = chip8.memory[chip8.I + row];
                for (int col = 0; col < 8; col++) {
                    if (x + col >= DISPLAY_WIDTH) continue; // clip at right
                    uint8_t pixel = (sprite >> (7 - col)) & 1;
                    uint8_t* screen_pixel = &display[y + row][x + col];
                    if (pixel && *screen_pixel) chip8.V[0xF] = 1;
                    *screen_pixel ^= pixel;
                }
            }
            break;
        }

        case 0xE000: { // Ex**
            uint8_t x = (opcode & 0x0F00) >> 8;
            switch (opcode & 0x00FF) {
                case 0x9E: { // Ex9E - SKP Vx
                    if (chip8.keypad[chip8.V[x]]) {
                        next_pc += 2;
                    }
                    break;
                }
                case 0xA1: { // ExA1 - SKNP Vx
                    if (!chip8.keypad[chip8.V[x]]) {
                        next_pc += 2;
                    }
                    break;
                }
                default:
                    printf("Unknown Ex opcode: 0x%04X\n", opcode);
                    break;
            }
            break;
        }

        case 0xF000: { // Fx** 
            uint8_t x = (opcode & 0x0F00) >> 8;
            switch (opcode & 0x00FF) {
                case 0x07: { // Fx07 - LD Vx, DT
                    chip8.V[x] = chip8.delay_timer;
                    break;
                }
                case 0x0A: { // Fx0A - LD Vx, Key
                    static int waiting = 0;
                    static int key_down = -1;

                    if(!waiting) {
                        waiting = 1;
                        key_down = -1;
                        next_pc = chip8.pc;
                        break;
                    }

                    if (key_down == -1) {
                        for (int i = 0; i < 16; i++) {
                            if (chip8.keypad[i]) {
                                key_down = i;
                                break;
                            }
                        }
                        next_pc = chip8.pc;
                        break;
                    } else {
                        if (!chip8.keypad[key_down]) {
                            chip8.V[x] = key_down;
                            waiting = 0;
                            key_down = -1;
                        } else {
                            next_pc = chip8.pc;
                        }
                        break;
                    }
                }
                case 0x15: { // Fx15 - LD DT, Vx
                    chip8.delay_timer = chip8.V[x];
                    break;
                }
                case 0x18: { // Fx18 - LD ST, Vx
                    chip8.sound_timer = chip8.V[x];
                    break;
                }
                case 0x1E: { // Fx1E - ADD I, Vx
                    chip8.I += chip8.V[x]; 
                    break;
                }
                case 0x29: { // Fx29 - LD LF, Vx
                    chip8.I = 0x50 + (chip8.V[x] & 0x0F) * 5;
                    break;
                }
                case 0x33: { // Fx33 - LD B, Vx
                    chip8.memory[chip8.I] = chip8.V[x] / 100;
                    chip8.memory[chip8.I + 1] = (chip8.V[x] / 10) % 10;
                    chip8.memory[chip8.I + 2] = chip8.V[x] % 10;
                    break;
                }
                case 0x55: { // Fx55 - LD [I], Vx
                    for (int i = 0; i <= x; i++) {
                        chip8.memory[chip8.I++] = chip8.V[i]; // Legacy code : I increments to [I + x + 1]
                    }
                    break;
                }
                case 0x65: { // Fx65 - LD Vx, [I]
                    for (int i = 0; i <= x; i++) {
                        chip8.V[i] = chip8.memory[chip8.I++]; // Legacy code : I increments to [I + x + 1]
                    }
                    break;
                }
                default:
                    printf("Unknown Fx opcode: 0x%04X\n", opcode);
                    break;
            }
            break;
        }

        default:
            printf("Unknown opcode: 0x%04X\n", opcode);
            break;
    }

    // Update PC & Display
    display_render();
    chip8.pc = next_pc;
}
