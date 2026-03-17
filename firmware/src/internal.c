#include "main.h"
#include <stdio.h>

extern UART_HandleTypeDef huart;
extern I2C_HandleTypeDef hi2c;

void enable_gpio(void){
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();
}

void Error_Handler(void){
  __disable_irq();
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
  while (1)
  {
  }
}

void SystemClock_Config(void){
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9; // 8 MHz * 9 = 72 MHz

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType =
      RCC_CLOCKTYPE_HCLK |
      RCC_CLOCKTYPE_SYSCLK |
      RCC_CLOCKTYPE_PCLK1 |
      RCC_CLOCKTYPE_PCLK2;

  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;   // 72 MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;     // 36 MHz (limit)
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;     // 72 MHz

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
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

void SysTick_Handler(void){
    HAL_IncTick();
}
