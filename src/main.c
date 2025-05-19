#include "../include/display.h"
#include <stdio.h>
#include <SDL2/SDL.h>

int main() {
    if (!display_init()) {
        return 1;
    }

    display[10][9] = 1;
    display[10][10] = 1;
    display[10][11] = 1;
    display[11][10] = 1;
    display[12][10] = 1;
    display[13][10] = 1;
    display[14][10] = 1;
    display[14][9] = 1;
    display[14][11] = 1;

    display_render();
    SDL_Delay(3000);
    
    display_destroy();
    return 0;
}