#include <stm32f1xx.h>
#include "main.h"
#include <stdio.h>

uint8_t c;
UART_HandleTypeDef huart;
I2C_HandleTypeDef hi2c;
volatile uint32_t last_press = 0;

void setup_gpio();
void setup_uart();
void setup_i2c();
void i2c_scan();

int main(){
    HAL_Init();
    SystemClock_Config();
    enable_gpio();
    setup_gpio();
    setup_uart();
    setup_i2c();
    i2c_scan();
    while(1){
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
        printf("ON\n\r");
        HAL_Delay(1000);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
        printf("OFF\n\r");
        HAL_Delay(1000);
    }
}

void setup_gpio(){
    GPIO_InitTypeDef init = {.Pin = GPIO_PIN_13, .Mode = GPIO_MODE_OUTPUT_PP,
                             .Pull = GPIO_NOPULL, .Speed = GPIO_SPEED_FREQ_LOW};
    HAL_GPIO_Init(GPIOC, &init);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
}

void setup_i2c(){
    __HAL_RCC_I2C2_CLK_ENABLE();

    GPIO_InitTypeDef gpio_init = {0};
    gpio_init.Pin = GPIO_PIN_10 | GPIO_PIN_11;
    gpio_init.Mode = GPIO_MODE_AF_OD;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gpio_init);

    hi2c.Instance = I2C2;
    hi2c.Init.ClockSpeed = 100000;
    hi2c.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c.Init.OwnAddress1 = 0;
    hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c.Init.OwnAddress2 = 0;
    hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c) != HAL_OK){
        Error_Handler();
    }
}

void i2c_scan()
{
    printf("Scanning I2C bus...\n");

    for(uint8_t addr = 1; addr < 128; addr++)
    {
        if(HAL_I2C_IsDeviceReady(&hi2c, addr << 1, 1, 10) == HAL_OK)
        {
            printf("Device found at 0x%02X\n", addr);
        }
    }

    printf("Scan complete\n");
}

void setup_uart(){
    __HAL_RCC_USART1_CLK_ENABLE();

    GPIO_InitTypeDef gpio_init = {0};
    gpio_init.Pin = GPIO_PIN_6;
    gpio_init.Mode = GPIO_MODE_AF_PP;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gpio_init);
    gpio_init.Pin = GPIO_PIN_7;
    gpio_init.Mode = GPIO_MODE_INPUT;
    gpio_init.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &gpio_init);

    __HAL_AFIO_REMAP_USART1_ENABLE();

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

int _write(int file, char *ptr, int len)
{
    HAL_UART_Transmit(&huart, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    return len;
}
