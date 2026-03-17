#include "main.h"

#include <stdio.h>

#define GPIO_DEBOUNCE_MS 200U

static volatile uint32_t s_last_pc13_tick = 0U;
static volatile uint32_t s_last_pc14_tick = 0U;

void EXTI15_10_IRQHandler(void){
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    uint32_t now = HAL_GetTick();

    if (GPIO_Pin == GPIO_PIN_13){
        if ((now - s_last_pc13_tick) >= GPIO_DEBOUNCE_MS){
            s_last_pc13_tick = now;
            printf("PC13 interrupt\n\r");
        }
    }
    else if (GPIO_Pin == GPIO_PIN_14){
        if ((now - s_last_pc14_tick) >= GPIO_DEBOUNCE_MS){
            s_last_pc14_tick = now;
            printf("PC14 interrupt\n\r");
        }
    }
}
