#include <stm32f1xx.h>
#include "main.h"
#include <stdio.h>

UART_HandleTypeDef huart;
I2C_HandleTypeDef hi2c;


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


        Error_Handler();
    }

        {
            printf("Device found at 0x%02X\n", addr);
        }
    }



        Error_Handler();
    }

}
