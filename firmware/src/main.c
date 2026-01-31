#include <stm32f1xx.h>
#include "main.h"
#include <stdio.h>

uint8_t c;
UART_HandleTypeDef huart;
volatile uint32_t last_press = 0;

void setup_gpio();
void setup_uart();


int main(){
    char buf[2] = {'a', '\0'};
    HAL_Init();
    SystemClock_Config();
    enable_gpio();
    setup_gpio();
    setup_uart();
    while(1){
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
        HAL_UART_Transmit(&huart, &buf, 1, HAL_MAX_DELAY);
        HAL_Delay(1000);
        HAL_UART_Transmit(&huart, &buf, 1, HAL_MAX_DELAY);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
        HAL_Delay(1000);
    }
}

void setup_gpio(){
    GPIO_InitTypeDef init = {.Pin = GPIO_PIN_13, .Mode = GPIO_MODE_OUTPUT_PP,
                             .Pull = GPIO_NOPULL, .Speed = GPIO_SPEED_FREQ_LOW};
    HAL_GPIO_Init(GPIOC, &init);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
}

void setup_uart(){
    __HAL_RCC_USART1_CLK_ENABLE();

    GPIO_InitTypeDef gpio_init = {0};
    gpio_init.Pin = GPIO_PIN_9;
    gpio_init.Mode = GPIO_MODE_AF_PP;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &gpio_init);
    gpio_init.Pin = GPIO_PIN_10;
    gpio_init.Mode = GPIO_MODE_INPUT;
    gpio_init.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &gpio_init);

    huart.Instance = USART1;
    huart.Init = (UART_InitTypeDef){
        .BaudRate = 115200, .WordLength = UART_WORDLENGTH_8B, 
        .StopBits = UART_STOPBITS_1, .Parity = UART_PARITY_NONE, 
        .Mode = UART_MODE_TX_RX, .HwFlowCtl = UART_HWCONTROL_NONE, 
        .OverSampling = UART_OVERSAMPLING_16};

    if (HAL_UART_Init(&huart) != HAL_OK){
        Error_Handler();
    }
}
