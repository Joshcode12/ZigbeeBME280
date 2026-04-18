#include "gpio_helper.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

void gpio_init() {
  const gpio_config_t output_cfg = {
      .pin_bit_mask = BIT64(CONFIG_OLED_DISABLE_GPIO),
      .mode = GPIO_MODE_OUTPUT,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .pull_up_en = GPIO_PULLUP_DISABLE,
      .intr_type = GPIO_INTR_DISABLE,
  };

  ESP_ERROR_CHECK(gpio_config(&output_cfg));

  const gpio_config_t input_cfg = {
      .pin_bit_mask = BIT64(CONFIG_VCNL_INTERRUPT_GPIO),
      .mode = GPIO_MODE_INPUT,
      .pull_down_en = GPIO_PULLDOWN_ENABLE,
      .pull_up_en = GPIO_PULLUP_DISABLE,
      .intr_type = GPIO_INTR_POSEDGE,
  };

  ESP_ERROR_CHECK(gpio_config(&input_cfg));
}
