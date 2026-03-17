#include <stm32f1xx.h>
#include "main.h"
#include "gfx.h"
#include "display.h"
#include "ssd1306.h"
#include "event.h"
#include <stdio.h>

UART_HandleTypeDef huart;
I2C_HandleTypeDef hi2c;
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
    display_init();

    static const uint8_t hi_bitmap_data[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC6, 0x00, 0xC6, 0x00, 0xC6, 0x60,
        0xC6, 0x60, 0xFE, 0x00, 0xFE, 0x60, 0xC6, 0x60, 0xC6, 0x60, 0xC6, 0x60,
        0xC6, 0x60, 0xC6, 0x60, 0x00, 0x00, 0x00, 0x00
    };

    GFX_Bitmap hi_bitmap = { hi_bitmap_data, sizeof(hi_bitmap_data), 16U, 16U };

    int x = 60, y = 25;

    while(1){
        GFX_Clear(&gfx, 0U);
        GFX_DrawRectBorder(&gfx, 10, 8, 108, 48, 2U, 1U);

        if (EventQueue_Pop(&event) != 0U){
            if (event == EVENT_LEFT){
                x -= 10;
            }
            else if (event == EVENT_RIGHT){
                x += 10;
            }
        }

        GFX_DrawBitmap(&gfx, &hi_bitmap, x, y);
        
        if (GFX_Present(&gfx) != 0){
            Error_Handler();
        }

        HAL_Delay(10);
    }
}
