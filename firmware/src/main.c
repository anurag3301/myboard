#include <stm32f1xx.h>
#include "main.h"
#include "gfx.h"
#include "display.h"
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
    display_init();

    char c[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 
        'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    GFX_Clear(&gfx, 0U);
    GFX_DrawRectBorder(&gfx, 10, 8, 108, 48, 2U, 1U);
    GFX_DrawChar(&gfx, '7', 88, 18, 14U, 1U);  /* Scaled with fixed aspect ratio */

    if (GFX_Present(&gfx) != 0){
        Error_Handler();
    }

    while(1){
        for(int i=0; i<36; i++){
            GFX_Clear(&gfx, 0U);
            GFX_DrawRectBorder(&gfx, 10, 8, 108, 48, 2U, 1U);
            GFX_DrawChar(&gfx, c[i], 60, 25, 14U, 1U);
            
            if (GFX_Present(&gfx) != 0){
                Error_Handler();
            }
            HAL_Delay(100);
        }
    }
}
