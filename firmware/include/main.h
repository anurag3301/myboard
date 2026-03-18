#include <stm32f1xx_hal.h>

void Error_Handler(void);
void SystemClock_Config(void);
void enable_gpio(void);
void setup_gpio();
void setup_uart();
void setup_i2c();
void i2c_scan();
void setup_spi();
