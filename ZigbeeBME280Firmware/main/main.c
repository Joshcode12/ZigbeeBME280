#include "esp_log.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "gpio_helper.h"
#include "i2c_helper.h"

static const char *TAG = "Zigbee BME280";

void app_main(void) {
  ESP_LOGI(TAG, "Staring...");

  gpio_init();

  i2c_init();

  vTaskDelay(pdMS_TO_TICKS(10));

  float temp, press, hum;
  bme280_read(bme280_hdl, &temp, &press, &hum);
  ESP_LOGI(TAG, "Temp: %0.2f, Press: %0.2f, Hum: %0.2f", temp,
           (float)press / 1000.0f, hum);

  while (1) {
    vTaskDelay(pdMS_TO_TICKS(10));
  }

  i2c_deinit();
  ESP_LOGI(TAG, "Ending...");
}
