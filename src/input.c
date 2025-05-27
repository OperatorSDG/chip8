#include "../include/input.h"
#include <stdbool.h>

const SDL_KeyCode chip8_keymap[16] = {
    SDLK_x, SDLK_1, SDLK_2, SDLK_3,
    SDLK_q, SDLK_w, SDLK_e, SDLK_a,
    SDLK_s, SDLK_d, SDLK_z, SDLK_c,
    SDLK_4, SDLK_r, SDLK_f, SDLK_v      
};

void input_handle_event(const SDL_Event *event) {
    if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) {
        bool pressed = (event->type == SDL_KEYDOWN);
        for (int i = 0; i < 16; i++) {
            if (event->key.keysym.sym == chip8_keymap[i]) {
                chip8.keypad[i] = pressed ? 1 : 0;
                // Input debug
                // printf("CHIP-8 key %X %s\n", i, pressed ? "pressed" : "released");
            }
        }
    }
}