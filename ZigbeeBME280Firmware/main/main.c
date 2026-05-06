#include "bme280.h"
#include "esp_log.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "gpio_helper.h"
#include "i2c_helper.h"
#include "ssd1306.h"
#include "vcnl4010.h"

static const char *TAG = "Zigbee BME280";

static ssd1306_handle_t display_hdl;
static bme280_handle_t bme280_hdl;
static vcnl4010_handle_t vcnl4010_hdl;

void app_main(void) {
  ESP_LOGI(TAG, "Staring...");

  gpio_init();

  i2c_init();

  ssd1306_config_t display_cfg = I2C_SSD1306_128x64_CONFIG_DEFAULT;
  ESP_ERROR_CHECK(ssd1306_init(bus_hdl, &display_cfg, &display_hdl));

  ssd1306_clear_display(display_hdl, false);
  ssd1306_set_contrast(display_hdl, 0xff);
  ssd1306_display_text(display_hdl, 0, "Starting...     ", false);

  bme280_config_t bme280_cfg = {
      .i2c_addr = BME280_I2C_ADDR_ALT,
      .mode = BME280_MODE_FORCED,
      .press_oversampling = BME280_OVERSAMPLING_X1,
      .temp_oversampling = BME280_OVERSAMPLING_X1,
      .hum_oversampling = BME280_OVERSAMPLING_X1,
  };
  ESP_ERROR_CHECK(bme280_init(bus_hdl, &bme280_cfg, &bme280_hdl));

  vcnl4010_config_t vcnl4010_cfg = {
      .enable_proximity = true,
      .enable_als = false,
      .ir_led_current = VCNL4010_IR_LED_120MA,
      .prox_rate = VCNL4010_PROX_RATE_16_63,
      .als =
          {
              .continuous_mode = false,
              .offset_compensation = true,
          },
      .interrupts =
          {
              .enable = true,
              .low_threshold = 0,
              .high_threshold = 2350,
              .count = VCNL4010_INT_COUNT_2,
              .use_als = false,
              .enable_prox_ready = false,
              .enable_als_ready = false,
          },
  };
  ESP_ERROR_CHECK(vcnl4010_init(bus_hdl, &vcnl4010_cfg, &vcnl4010_hdl));

  while (1) {
    vTaskDelay(pdMS_TO_TICKS(500));
  }

  if (display_hdl != NULL) {
    ssd1306_display_text(display_hdl, 0, "Ending...       ", false);
    vTaskDelay(pdMS_TO_TICKS(1000));

    ssd1306_clear_display(display_hdl, false);
    ssd1306_delete(display_hdl);
    display_hdl = NULL;
  }

  if (bme280_hdl != NULL) {
    bme280_deinit(bme280_hdl);
    bme280_hdl = NULL;
  }

  if (vcnl4010_hdl != NULL) {
    vcnl4010_deinit(vcnl4010_hdl);
    vcnl4010_hdl = NULL;
  }

  i2c_deinit();
  ESP_LOGI(TAG, "Ending...");
}
