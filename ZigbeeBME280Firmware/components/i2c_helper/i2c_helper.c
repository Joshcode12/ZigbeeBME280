#include "i2c_helper.h"
#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "esp_log.h"
#include "sdkconfig.h"

static const char *TAG = "I2C Helper";

i2c_master_bus_handle_t bus_hdl;

void i2c_init() {
  ESP_LOGI(TAG, "I2C initialize starting...");

  const i2c_master_bus_config_t i2c_mst_config = {
      .clk_source = I2C_CLK_SRC_DEFAULT,
      .i2c_port = CONFIG_I2C_NUM,
      .scl_io_num = CONFIG_I2C_MASTER_SCL,
      .sda_io_num = CONFIG_I2C_MASTER_SDA,
      .glitch_ignore_cnt = 7,
      .flags.enable_internal_pullup = false,
  };
  ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_hdl));

  ESP_LOGI(TAG, "I2C initialize done...");
}

void i2c_deinit() {
  ESP_LOGI(TAG, "I2C deinitialize starting...");

  if (bus_hdl != NULL) {
    ESP_ERROR_CHECK(i2c_del_master_bus(bus_hdl));
    bus_hdl = NULL;
  }

  gpio_reset_pin(CONFIG_I2C_MASTER_SDA);
  gpio_reset_pin(CONFIG_I2C_MASTER_SCL);

  ESP_LOGI(TAG, "I2C deinitialize done...");
}

void i2c_bus_scan() {
  ESP_LOGI(TAG, "Starting I2C scan...");

  for (int addr = 1; addr < 127; addr++) {
    if (i2c_master_probe(bus_hdl, addr, -1) == ESP_OK) {
      ESP_LOGI(TAG, "Found device at address: 0x%02x", addr);
    }
  }

  ESP_LOGI(TAG, "Scan complete.");
}