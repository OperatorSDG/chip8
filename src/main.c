#include "../include/display.h"
#include "../include/cpu.h"
#include <stdio.h>
#include <SDL2/SDL.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <ROM file>\n", argv[0]);
        return 1;
    }

    cpu_init();
    cpu_load_rom(argv[1]);

    // Print first 10 Bytes of ROM
    printf("Program Counter: 0x%03X\n", chip8.pc);
    printf("First 10 bytes of ROM in memory at 0x200: \n");
    for (int i = 0; i < 10; i++) {
        int curr_mem_loc = 0x200 + i;
        printf("0x%03X: 0x%02X\n",curr_mem_loc ,chip8.memory[curr_mem_loc]);
    }
    printf("\n");

    return 0;
}