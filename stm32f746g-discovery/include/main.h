#ifndef MAIN_H
#define MAIN_H

// Here to provide LCD colors in application.c
#include "stm32746g_discovery_lcd.h"

void error_handler(const char *function, char *file, int line);

void set_screen_color(uint32_t color);

void on_screen_touch(void);

void display_message(char *message, uint32_t color);

#endif
