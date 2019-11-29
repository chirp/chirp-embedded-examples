#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>

// Here to provide LCD colors in application.c
#include "stm32469i_discovery_lcd.h"

bool init_audio_input(void);

bool init_audio_output(void);

void error_handler(const char *function, char *file, int line);

void set_screen_color(uint32_t color);

void on_screen_touch(void);

void display_message(char *message, uint32_t color);

#endif
