#include "bme280.h"
#include "driver/i2c_master.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/idf_additions.h"

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static const char *TAG = "BME280";

static inline esp_err_t bme280_read_reg(bme280_handle_t handle, uint8_t reg,
                                        uint8_t *data, size_t len) {
  if (handle == NULL || data == NULL || len == 0)
    return ESP_ERR_INVALID_ARG;

  return i2c_master_transmit_receive(handle->i2c_dev, &reg, 1, data, len, -1);
}

static inline esp_err_t bme280_write_reg(bme280_handle_t handle, uint8_t reg,
                                         uint8_t *data, size_t len) {
  if (handle == NULL || data == NULL || len == 0)
    return ESP_ERR_INVALID_ARG;

  uint8_t write_buffer[len + 1];
  write_buffer[0] = reg;
  memcpy(&write_buffer[1], data, len);

  return i2c_master_transmit(handle->i2c_dev, write_buffer,
                             sizeof(write_buffer), -1);
}

static inline uint8_t bme280_status(bme280_handle_t handle) {
  uint8_t status;
  ESP_ERROR_CHECK(bme280_read_reg(handle, BME280_REG_STATUS, &status, 1));
  return status;
}

static esp_err_t bme280_reset(bme280_handle_t handle) {
  if (handle == NULL) {
    return ESP_ERR_INVALID_ARG;
  }

  uint8_t reset_cmd = BME280_SOFT_RESET;
  esp_err_t err = bme280_write_reg(handle, BME280_REG_RESET, &reset_cmd, 1);
  if (err != ESP_OK) {
    return err;
  }

  int timeout = 10;
  while (timeout-- > 0) {
    vTaskDelay(pdMS_TO_TICKS(5));

    uint8_t status = bme280_status(handle);

    if ((status & BME280_STATUS_IM_UPDATE) == 0) {
      return ESP_OK;
    }
  }

  return ESP_ERR_TIMEOUT;
}

esp_err_t bme280_init(i2c_master_bus_handle_t bus, bme280_config_t *config,
                      bme280_handle_t *handle) {
  esp_err_t err = ESP_OK;
  ESP_LOGI(TAG, "Initializing BME280...");

  struct bme280_context *dev = calloc(1, sizeof(struct bme280_context));
  if (dev == NULL)
    return ESP_ERR_NO_MEM;

  const i2c_device_config_t i2c_dev_config = {
      .dev_addr_length = I2C_ADDR_BIT_LEN_7,
      .device_address = config->i2c_addr,
      .scl_speed_hz = 100000,
  };

  err = i2c_master_bus_add_device(bus, &i2c_dev_config, &dev->i2c_dev);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to add I2C device to bus: %d", err);
    goto fail_alloc;
  }

  err = bme280_reset(dev);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to reset BME280: %d", err);
    goto fail_device;
  }

  uint8_t id;
  err = bme280_read_reg(dev, BME280_REG_ID, &id, 1);
  if (err != ESP_OK || id != BME280_CHIP_ID) {
    ESP_LOGE(TAG, "Failed to read BME280 ID: %d", err);
    goto fail_device;
  }

  *handle = (bme280_handle_t)dev;
  ESP_LOGI(TAG, "BME280 initialized.");
  return ESP_OK;

fail_device:
  i2c_master_bus_rm_device(dev->i2c_dev);
fail_alloc:
  free(dev);
  return err;
}

esp_err_t bme280_deinit(bme280_handle_t handle) {
  return i2c_master_bus_rm_device(handle->i2c_dev);
}