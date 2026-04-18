#include "esp_log.h"
#include "gpio_helper.h"
#include "i2c_helper.h"

static const char *TAG = "Zigbee BME280";

void app_main(void) {
  ESP_LOGI(TAG, "Staring...");

  gpio_init();

  i2c_init();

  ESP_LOGI(TAG, "Ending...");

  i2c_deinit();
}
