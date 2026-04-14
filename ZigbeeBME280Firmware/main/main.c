#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "ssd1306.h"

static const char *TAG = "ZigbeeBME280";

void app_main(void) {
  ESP_LOGI(TAG, "Staring...");

  gpio_config_t io_conf = {
      .pin_bit_mask = BIT64(CONFIG_OLED_DISABLE_GPIO),
      .mode = GPIO_MODE_OUTPUT,
      .pull_down_en = false,
      .pull_up_en = false,
      .intr_type = GPIO_INTR_DISABLE,
  };

  gpio_config(&io_conf);

  gpio_set_level(CONFIG_OLED_DISABLE_GPIO, 1);

  i2c_master_bus_config_t i2c_mst_config = {
      .clk_source = I2C_CLK_SRC_DEFAULT,
      .i2c_port = CONFIG_I2C_NUM,
      .scl_io_num = CONFIG_I2C_MASTER_SCL,
      .sda_io_num = CONFIG_I2C_MASTER_SDA,
      .glitch_ignore_cnt = 7,
      .flags.enable_internal_pullup = false,
  };

  i2c_master_bus_handle_t bus_handle;
  ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));

  ssd1306_config_t display_cfg = I2C_SSD1306_128x64_CONFIG_DEFAULT;
  ssd1306_handle_t display_hdl;

  ssd1306_init(bus_handle, &display_cfg, &display_hdl);

  if (display_hdl == NULL) {
    ESP_LOGE(TAG, "ssd1306 handle init failed");
    assert(display_hdl);
  }

  ssd1306_clear_display(display_hdl, false);
  ssd1306_set_contrast(display_hdl, 0xff);
  ssd1306_display_text(display_hdl, 0, "SSD1306 128x64", false);
  ssd1306_display_text(display_hdl, 1, "Hello World!!", false);
  ssd1306_display_text(display_hdl, 2, "SSD1306 128x64", true);
  ssd1306_display_text(display_hdl, 3, "Hello World!!", true);

  ESP_LOGI(TAG, "Ending...");
}
