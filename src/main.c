#include "../include/display.h"
#include "../include/input.h"
#include "../include/cpu.h"
#include "../include/audio.h"

#include <stdio.h>
#include <SDL2/SDL.h>

#define CYCLE_PER_FRAME 8

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <ROM file>\n", argv[0]);
        return 1;
    }

    // Initialize Systems
    if (!display_init()) {
        return 1;
    }
    if (!audio_init()) {
        return 1;
    }
    cpu_init();
    cpu_load_rom(argv[1]);
    bool running = true;

    // CPU Cycling
    uint32_t tick_time = SDL_GetTicks();
    while (running) {
        // Event Handler
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            input_handle_event(&event);
        }
        
        // Simulate CPU Cycles
        for (int cycle_no = 0; cycle_no < CYCLE_PER_FRAME; cycle_no++) {
            cpu_cycle();
        }

        // Timer Updates
        if (SDL_GetTicks() - tick_time >= 16) { // 60hz ~= 16ms
            if (chip8.delay_timer > 0) {
                chip8.delay_timer--;
            }
            if (chip8.sound_timer > 0) {
                audio_beep_on();
                chip8.sound_timer--;
                if (chip8.sound_timer == 0) {
                    audio_beep_off();
                }
            } else {
                audio_beep_off();
            }
            tick_time = SDL_GetTicks();
        }
        SDL_Delay(1);
    }

    display_destroy();
    return 0;
}