#include "../include/display.h"
#include "../include/cpu.h"
#include <stdio.h>
#include <SDL2/SDL.h>

#define CYCLE_COUNT 40

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

    // Cycle through Test ROM
    for (int i = 0; i < CYCLE_COUNT; i++){
        cpu_cycle();
        printf("Cycle %d: PC = 0x%03X\n", i+1, chip8.pc);
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