#include "../include/display.h"
#include "../include/cpu.h"
#include <stdio.h>
#include <SDL2/SDL.h>

int main() {
    if (!display_init()) {
        printf("Failed to Initialize Display!\n");
        return 1;
    }

    // 'S'
    display[10][9] = 1;
    display[10][10] = 1;
    display[10][11] = 1;
    display[11][9] = 1;
    display[12][9] = 1;
    display[12][10] = 1;
    display[12][11] = 1;
    display[13][11] = 1;
    display[14][9] = 1;
    display[14][10] = 1;
    display[14][11] = 1;

    // 'D'
    display[10][13] = 1;
    display[10][14] = 1;
    display[11][15] = 1;
    display[11][13] = 1;
    display[12][13] = 1;
    display[12][15] = 1;
    display[13][13] = 1;
    display[14][13] = 1;
    display[14][14] = 1;
    display[13][15] = 1;

    // 'G'
    display[10][17] = 1;
    display[10][18] = 1;
    display[10][19] = 1;
    display[11][17] = 1;
    display[12][17] = 1;
    display[12][19] = 1;
    display[13][17] = 1;
    display[14][17] = 1;
    display[14][18] = 1;
    display[14][19] = 1;
    display[13][19] = 1;

    display_render();
    SDL_Delay(10000);
    
    display_destroy();
    return 0;
}