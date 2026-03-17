#ifndef __DISPLAY_H
#define __DISPLAY_H
#include "main.h"

#define APP_OLED_WIDTH 128U
#define APP_OLED_HEIGHT 64U
#define APP_OLED_BUFFER_SIZE ((APP_OLED_WIDTH * APP_OLED_HEIGHT) / 8U)

void display_init(void);

#endif //__DISPLAY_H
