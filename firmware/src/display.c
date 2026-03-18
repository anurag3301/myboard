#include <stdio.h>
#include "display.h"
#include "ssd1306.h"
#include "gfx.h"

extern OLED_Config oled;
extern GFX_Framebuffer gfx;
extern SPI_HandleTypeDef hspi;
static uint8_t gfx_buffer[APP_OLED_BUFFER_SIZE];


static int32_t oled_send_spi(void *user_context, uint8_t is_data, 
                             const uint8_t *data, size_t length)
{
    SPI_HandleTypeDef *spi = (SPI_HandleTypeDef *)user_context;

    if ((spi == NULL) || (data == NULL) || (length == 0U))
    {
        return OLED_ERR_INVALID_ARG;
    }

    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, (is_data != 0U) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET);

    if (HAL_SPI_Transmit(spi, (uint8_t *)data, (uint16_t)length, HAL_MAX_DELAY) != HAL_OK){
        HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);
        return OLED_ERR_IO;
    }

    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);
    return OLED_OK;
}


void display_init(void){

    //PB12     ------> OLED_CS
    //PB15     ------> OLED_DC
    //PA8      ------> OLED_RES
    GPIO_InitTypeDef gpio_init = {0};
    gpio_init.Pin = OLED_CS_Pin;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gpio_init);

    gpio_init.Pin = OLED_DC_Pin;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &gpio_init);

    gpio_init.Pin = OLED_RST_Pin;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(OLED_RST_GPIO_Port, &gpio_init);

    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(100);

    // RESET the OLED display
    HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(5);
    HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(10);

    oled = (OLED_Config){
        .bus_type = OLED_BUS_SPI,
        .width = APP_OLED_WIDTH,
        .height = APP_OLED_HEIGHT,
        .user_context = &hspi,
        .transport = {
            .spi = {
                .send_fn = oled_send_spi
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
