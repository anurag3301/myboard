#include <stm32f1xx.h>
#include "main.h"
#include "gfx.h"
#include "display.h"
#include "ssd1306.h"
#include "event.h"
#include <stdio.h>

UART_HandleTypeDef huart;
I2C_HandleTypeDef hi2c;
SPI_HandleTypeDef hspi;
OLED_Config oled;
GFX_Framebuffer gfx;

int main(){
    EventType event;

    HAL_Init();
    SystemClock_Config();
    enable_gpio();
    EventQueue_Init();
    setup_gpio();
    setup_uart();
    setup_i2c();
    setup_spi();
    display_init();

    uint8_t num = 0;

    char numbuf[20];

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);


    while(1){
        GPIOA->BSRR = (0xFF << 16);
        GPIOA->BSRR = num;
        GFX_Clear(&gfx, 0U);
        GFX_DrawRectBorder(&gfx, 0, 0, 120, 60, 5U, 1U);

        // if (EventQueue_Pop(&event) != 0U){
        //     if (event == EVENT_LEFT){
        //         x -= 10;
        //     }
        //     else if (event == EVENT_RIGHT){
        //         x += 10;
        //     }
        // }

        snprintf(numbuf, 20, "%ud", num++);
        GFX_DrawStr(&gfx, numbuf, 35, 20, 20, 1);
        
        if (GFX_Present(&gfx) != 0){
            Error_Handler();
        }

        HAL_Delay(50);
    }
}
