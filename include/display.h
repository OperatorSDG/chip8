#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DISPLAY_SCALE 10

extern uint8_t display[DISPLAY_HEIGHT][DISPLAY_WIDTH];

bool display_init(void);
void display_clear(void);
void display_render(void);
void display_destroy(void);

#endif