#include <stdio.h>
#include "display.h"
#include "ssd1306.h"
#include "gfx.h"

extern I2C_HandleTypeDef hi2c;
extern OLED_Config oled;
extern GFX_Framebuffer gfx;
static uint8_t gfx_buffer[APP_OLED_BUFFER_SIZE];

static int32_t oled_send_i2c(void *user_context, uint8_t i2c_address_7bit,
                             const uint8_t *data, size_t length)
{
    I2C_HandleTypeDef *i2c = (I2C_HandleTypeDef *)user_context;
    if ((i2c == NULL) || (data == NULL) || (length == 0U)){
        return OLED_ERR_INVALID_ARG;
    }

    if (HAL_I2C_Master_Transmit(i2c, (uint16_t)(i2c_address_7bit << 1),
                                (uint8_t *)data, (uint16_t)length,
                                HAL_MAX_DELAY) != HAL_OK)
    {
        return OLED_ERR_IO;
    }

    return OLED_OK;
}


void display_init(void){
    oled = (OLED_Config){
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

    if (OLED_Init(&oled) != OLED_OK){
        Error_Handler();
    }

    gfx = GFX_Init(oled.width, oled.height, gfx_buffer, sizeof(gfx_buffer),
                    OLED_GfxFlushCallback, &oled);

    if (GFX_IsReady(&gfx) == 0U){
        Error_Handler();
    }
}
