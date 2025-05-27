#include "../include/display.h"
#include "../include/cpu.h"
#include <stdio.h>
#include <SDL2/SDL.h>

#define CYCLE_COUNT 950

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <ROM file>\n", argv[0]);
        return 1;
    }

    // Initialize Systems
    if(!display_init()) {
        return 1;
    }
    cpu_init();
    cpu_load_rom(argv[1]);
    printf("0x200: 0x%02X%02X\n", chip8.memory[0x200], chip8.memory[0x201]); // Should print 0x12 08


    // Cycle through Test ROM
    for (int i = 0; i < CYCLE_COUNT; i++){
        cpu_cycle();
        printf("Cycle %d: PC = 0x%03X : Code = 0x%02X%02X\n", i+1, chip8.pc - 2, chip8.memory[chip8.pc - 2],chip8.memory[chip8.pc - 1]);
        
        // Print Registers
        for (int r = 0; r < 16; r++) {
            printf("%d ", chip8.V[r]);
            if ((r + 1) % 8 == 0) printf("\n");  // 8 per line
        }

        printf("\n");
    }

    SDL_Event event;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        SDL_Delay(100);
    }

    display_destroy();
    return 0;
}