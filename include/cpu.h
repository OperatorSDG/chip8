#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdbool.h>

// CHIP-8 CPU Constants
#define MEMORY_SIZE 4096
#define REGISTER_COUNT 16
#define STACK_SIZE 16
#define KEYPAD_SIZE 16
#define FONTSET_START_ADDRESS 0x50
#define PROGRAM_START_ADDRESS 0x200

// CPU Structure
typedef struct{
    uint8_t memory[MEMORY_SIZE];        // 4KB
    uint8_t V[REGISTER_COUNT];          // 16x 8-bit registers
    uint16_t I;                         // Index Register
    uint16_t pc;                        // Program Counter
    uint8_t delay_timer;                //
    uint8_t sound_timer;                //
    uint16_t stack[STACK_SIZE];          // Stack for subroutine calls
    uint8_t sp;
    uint8_t keypad[KEYPAD_SIZE];
} Chip8;

extern Chip8 chip8;

// CPU Functions
void cpu_init(void);
void cpu_load_rom(const char* path);
void cpu_cycle(void);


#endif