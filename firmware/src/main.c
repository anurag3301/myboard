#include <stm32f1xx.h>
#include "main.h"
#include "gfx.h"
#include "ssd1306.h"
#include <stdio.h>

UART_HandleTypeDef huart;
I2C_HandleTypeDef hi2c;

#define APP_OLED_WIDTH 128U
#define APP_OLED_HEIGHT 64U
#define APP_OLED_BUFFER_SIZE ((APP_OLED_WIDTH * APP_OLED_HEIGHT) / 8U)

int main(){
    static uint8_t gfx_buffer[APP_OLED_BUFFER_SIZE];
    OLED_Config oled = {
        .bus_type = OLED_BUS_I2C,
        .width = APP_OLED_WIDTH,
        .height = APP_OLED_HEIGHT,
        .user_context = &hi2c,
        .transport = {
            .i2c = {
                .i2c_address_7bit = 0x3CU,
                .send_fn = oled_send_i2c
            }
        }
    };

    HAL_Init();
    SystemClock_Config();
    enable_gpio();
    setup_gpio();
    setup_uart();
    setup_i2c();
    i2c_scan();

    if (OLED_Init(&oled) != OLED_OK){
        Error_Handler();
    }

    GFX_Framebuffer gfx = GFX_Init(oled.width,
                                   oled.height,
                                   gfx_buffer,
                                   sizeof(gfx_buffer),
                                   OLED_GfxFlushCallback,
                                   &oled);
    if (GFX_IsReady(&gfx) == 0U){
        Error_Handler();
    }

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



static int32_t oled_send_i2c(void *user_context,
                             uint8_t i2c_address_7bit,
                             const uint8_t *data,
                             size_t length)
{
    I2C_HandleTypeDef *i2c = (I2C_HandleTypeDef *)user_context;
    if ((i2c == NULL) || (data == NULL) || (length == 0U)){
        return OLED_ERR_INVALID_ARG;
    }

    if (HAL_I2C_Master_Transmit(i2c,
                                (uint16_t)(i2c_address_7bit << 1),
                                (uint8_t *)data,
                                (uint16_t)length,
                                HAL_MAX_DELAY) != HAL_OK)
    {
        return OLED_ERR_IO;
    }

    return OLED_OK;
}

