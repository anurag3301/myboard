#include <stm32f1xx.h>
#include "main.h"
#include "gfx.h"
#include "ssd1306.h"
#include <stdio.h>

UART_HandleTypeDef huart;
I2C_HandleTypeDef hi2c;
OLED_Config oled;
GFX_Framebuffer gfx;

int main(){
    HAL_Init();
    SystemClock_Config();
    enable_gpio();
    setup_gpio();
    setup_uart();
    setup_i2c();

    GFX_Clear(&gfx, 0U);
    GFX_DrawRectBorder(&gfx, 10, 8, 108, 48, 2U, 1U);

    if (GFX_Present(&gfx) != 0){
        Error_Handler();
    }

    while(1){
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
        printf("OLED rectangle displayed\n\r");
        HAL_Delay(1000);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
        HAL_Delay(1000);
    }
}

