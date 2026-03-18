#ifndef __DISPLAY_H
#define __DISPLAY_H
#include "main.h"

#define APP_OLED_WIDTH 128U
#define APP_OLED_HEIGHT 64U
#define APP_OLED_BUFFER_SIZE ((APP_OLED_WIDTH * APP_OLED_HEIGHT) / 8U)

#define OLED_CS_GPIO_Port GPIOB
#define OLED_CS_Pin GPIO_PIN_12
#define OLED_DC_GPIO_Port GPIOC
#define OLED_DC_Pin GPIO_PIN_15
#define OLED_RST_GPIO_Port GPIOA
#define OLED_RST_Pin GPIO_PIN_8

void display_init(void);

#endif //__DISPLAY_H
