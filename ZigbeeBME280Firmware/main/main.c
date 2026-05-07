#include "bme280.h"
#include "esp_check.h"
#include "esp_err.h"
#include "esp_event.h"
#include "esp_log.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "i2c_helper.h"
#include "sdkconfig.h"
#include "ssd1306.h"
#include "vcnl4010.h"

static const char *TAG = "Zigbee BME280";

static TaskHandle_t ui_task_hdl = NULL;
static uint32_t screen_on_time_ms = 10000;

static ssd1306_handle_t display_hdl;
static bme280_handle_t bme280_hdl;
static vcnl4010_handle_t vcnl4010_hdl;

static esp_err_t add_i2c_devices(void);

static void vcnl_event_handler(void *handler_args, esp_event_base_t base,
                               int32_t id, void *event_data);

static void ui_application_task(void *pvParameters);

void app_main(void) {
  ESP_LOGI(TAG, "Staring...");

  i2c_init();

  esp_event_loop_create_default();

  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      VCNL4010_EVENTS, ESP_EVENT_ANY_ID, &vcnl_event_handler, NULL, NULL));

  ESP_ERROR_CHECK(add_i2c_devices());

  xTaskCreate(ui_application_task, "ui_task", 4096, NULL, 5, &ui_task_hdl);

  ESP_ERROR_CHECK(
      vcnl4010_interrupt_init(vcnl4010_hdl, CONFIG_VCNL_INTERRUPT_GPIO, false));
}

static esp_err_t add_i2c_devices(void) {
  ssd1306_config_t display_cfg = I2C_SSD1306_128x64_CONFIG_DEFAULT;
  ESP_RETURN_ON_ERROR(ssd1306_init(bus_hdl, &display_cfg, &display_hdl), TAG,
                      "Failed to add ssd1306");

  ESP_RETURN_ON_ERROR(ssd1306_clear_display(display_hdl, false), TAG,
                      "Failed to clear the screen");
  ESP_RETURN_ON_ERROR(ssd1306_set_contrast(display_hdl, 0xff), TAG,
                      "Failed to set the screen contrast");
  ESP_RETURN_ON_ERROR(ssd1306_disable_display(display_hdl), TAG,
                      "Failed sleep the ssd1306 display");

  bme280_config_t bme280_cfg = {
      .i2c_addr = BME280_I2C_ADDR_ALT,
      .mode = BME280_MODE_FORCED,
      .press_oversampling = BME280_OVERSAMPLING_X2,
      .temp_oversampling = BME280_OVERSAMPLING_X2,
      .hum_oversampling = BME280_OVERSAMPLING_X1,
  };
  ESP_RETURN_ON_ERROR(bme280_init(bus_hdl, &bme280_cfg, &bme280_hdl), TAG,
                      "Failed too add ssd1306");

  vcnl4010_config_t vcnl4010_cfg = {
      .enable_proximity = true,
      .enable_als = false,
      .ir_led_current = VCNL4010_IR_LED_150MA,
      .prox_rate = VCNL4010_PROX_RATE_31_25,
      .als =
          {
              .continuous_mode = false,
              .offset_compensation = true,
          },
      .interrupts =
          {
              .enable = true,
              .low_threshold = 0,
              .high_threshold = 2300,
              .count = VCNL4010_INT_COUNT_2,
              .use_als = false,
              .enable_prox_ready = false,
              .enable_als_ready = false,
          },
  };
  ESP_RETURN_ON_ERROR(vcnl4010_init(bus_hdl, &vcnl4010_cfg, &vcnl4010_hdl), TAG,
                      "Failed too add vcnl4010");
  return ESP_OK;
}

static void vcnl_event_handler(void *handler_args, esp_event_base_t base,
                               int32_t id, void *event_data) {
  switch (id) {
  case VCNL4010_EVENT_THR_HI:
    if (ui_task_hdl != NULL) {
      xTaskNotifyGive(ui_task_hdl);
    }
    break;
  }
}

static void ui_application_task(void *pvParameters) {
  while (1) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    ESP_LOGI(TAG, "Waking up display...");
    ssd1306_enable_display(display_hdl);

    const uint32_t update_interval_ms = 250;
    uint32_t elapsed_ms = 0;

    while (elapsed_ms < screen_on_time_ms) {
      float temp, press, hum;
      if (bme280_read(bme280_hdl, &temp, &press, &hum) == ESP_OK) {
        char buf[17];
        (void)ssd1306_display_text(display_hdl, 1, "Current Readings", false);

        (void)snprintf(buf, sizeof(buf), "Temp: %.1fC", temp);
        (void)ssd1306_display_text(display_hdl, 2, buf, false);

        (void)snprintf(buf, sizeof(buf), "Hum:  %.1f%%", hum);
        (void)ssd1306_display_text(display_hdl, 3, buf, false);

        (void)snprintf(buf, sizeof(buf), "Pres: %.2fkPa", press / 1000);
        (void)ssd1306_display_text(display_hdl, 4, buf, false);
      }

      vTaskDelay(pdMS_TO_TICKS(update_interval_ms));
      elapsed_ms += update_interval_ms;

      if (ulTaskNotifyTake(pdTRUE, 0) > 0) {
        elapsed_ms = 0;
        ESP_LOGD(TAG, "Screen timer reset due to new proximity trigger");
      }
    }

    ESP_LOGI(TAG, "Sleeping display...");
    ssd1306_clear_display(display_hdl, false);
    ssd1306_disable_display(display_hdl);

    ulTaskNotifyTake(pdTRUE, 0);
  }
}