#include <stm32f1xx_hal.h>

void Error_Handler(void);
void SystemClock_Config(void);
void enable_gpio(void);
void setup_gpio();
void setup_uart();
void setup_i2c();
void i2c_scan();
static int32_t oled_send_i2c(void *user_context, uint8_t i2c_address_7bit, const uint8_t *data, size_t length);
