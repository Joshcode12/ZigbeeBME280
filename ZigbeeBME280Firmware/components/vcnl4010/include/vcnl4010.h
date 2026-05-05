#pragma once

#include "driver/i2c_types.h"
#include "esp_err.h"
#include <stdint.h>

#define VCNL4010_I2C_ADDR 0x13

#define VCNL4010_CHIP_ID 0x21

#define VCNL4010_REG_COMMAND 0x80

#define VCNL4010_REG_PRODUCT_ID 0x81

#define VCNL4010_REG_PROX_RATE 0x82

#define VCNL4010_REG_IR_LED_CURRENT 0x83

#define VCNL4010_REG_ALS_PARAM 0x84

#define VCNL4010_REG_ALS_RESULT_H 0x85
#define VCNL4010_REG_ALS_RESULT_L 0x86

#define VCNL4010_REG_PROX_RESULT_H 0x87
#define VCNL4010_REG_PROX_RESULT_L 0x88

#define VCNL4010_REG_INT_CTRL 0x89

#define VCNL4010_REG_LOW_THRESH_H 0x8A
#define VCNL4010_REG_LOW_THRESH_L 0x8B

#define VCNL4010_REG_HIGH_THRESH_H 0x8C
#define VCNL4010_REG_HIGH_THRESH_L 0x8D

#define VCNL4010_REG_INT_STATUS 0x8E

#define VCNL4010_REG_PROX_TIMING 0x8F

// --- Command register bits (0x80) ---
typedef enum vcnl4010_cmd {
  VCNL4010_CMD_SELFTIMED_EN = (1 << 0),  // enable self-timed measurements
  VCNL4010_CMD_PROX_EN = (1 << 1),       // enable proximity in self-timed mode
  VCNL4010_CMD_ALS_EN = (1 << 2),        // enable ALS in self-timed mode
  VCNL4010_CMD_PROX_OD = (1 << 3),       // trigger single proximity on-demand
  VCNL4010_CMD_ALS_OD = (1 << 4),        // trigger single ALS on-demand
  VCNL4010_CMD_PROX_DATA_RDY = (1 << 5), // proximity result ready (read-only)
  VCNL4010_CMD_ALS_DATA_RDY = (1 << 6),  // ALS result ready (read-only)
} vcnl4010_cmd_t;

// --- Interrupt status bits (0x8D) ---
typedef enum vcnl4010_int_status {
  VCNL4010_INT_THRESH_HI = (1 << 0), // proximity above high threshold
  VCNL4010_INT_THRESH_LO = (1 << 1), // proximity below low threshold
  VCNL4010_INT_ALS_RDY = (1 << 2),   // ALS data ready
  VCNL4010_INT_PROX_RDY = (1 << 3),  // proximity data ready
} vcnl4010_int_status_t;

// --- IR LED current (0x82), 10mA per step, 0–200mA ---
typedef enum vcnl4010_ir_led_current {
  VCNL4010_IR_LED_0MA = 0,
  VCNL4010_IR_LED_10MA = 1,
  VCNL4010_IR_LED_20MA = 2,
  VCNL4010_IR_LED_30MA = 3,
  VCNL4010_IR_LED_40MA = 4,
  VCNL4010_IR_LED_50MA = 5,
  VCNL4010_IR_LED_60MA = 6,
  VCNL4010_IR_LED_70MA = 7,
  VCNL4010_IR_LED_80MA = 8,
  VCNL4010_IR_LED_90MA = 9,
  VCNL4010_IR_LED_100MA = 10,
  VCNL4010_IR_LED_110MA = 11,
  VCNL4010_IR_LED_120MA = 12,
  VCNL4010_IR_LED_130MA = 13,
  VCNL4010_IR_LED_140MA = 14,
  VCNL4010_IR_LED_150MA = 15,
  VCNL4010_IR_LED_160MA = 16,
  VCNL4010_IR_LED_170MA = 17,
  VCNL4010_IR_LED_180MA = 18,
  VCNL4010_IR_LED_190MA = 19,
  VCNL4010_IR_LED_200MA = 20,
} vcnl4010_ir_led_current_t;

// --- ALS averaging (0x83 bits [2:0]) ---
typedef enum vcnl4010_als_averaging {
  VCNL4010_ALS_AVG_1 = 0,
  VCNL4010_ALS_AVG_2 = 1,
  VCNL4010_ALS_AVG_4 = 2,
  VCNL4010_ALS_AVG_8 = 3,
  VCNL4010_ALS_AVG_16 = 4,
  VCNL4010_ALS_AVG_32 = 5,
  VCNL4010_ALS_AVG_64 = 6,
  VCNL4010_ALS_AVG_128 = 7,
} vcnl4010_als_averaging_t;

// --- ALS measurement rate (0x83 bits [6:4]), samples/s ---
typedef enum vcnl4010_als_rate {
  VCNL4010_ALS_RATE_1 = 0,
  VCNL4010_ALS_RATE_2 = 1,
  VCNL4010_ALS_RATE_3 = 2,
  VCNL4010_ALS_RATE_4 = 3,
  VCNL4010_ALS_RATE_5 = 4,
  VCNL4010_ALS_RATE_6 = 5,
  VCNL4010_ALS_RATE_8 = 6,
  VCNL4010_ALS_RATE_10 = 7,
} vcnl4010_als_rate_t;

// --- Proximity measurement rate (0x8E bits [2:0]), measurements/s ---
typedef enum vcnl4010_prox_rate {
  VCNL4010_PROX_RATE_1_95 = 0,
  VCNL4010_PROX_RATE_3_91 = 1,
  VCNL4010_PROX_RATE_7_81 = 2,
  VCNL4010_PROX_RATE_16_63 = 3,
  VCNL4010_PROX_RATE_31_25 = 4,
  VCNL4010_PROX_RATE_62_5 = 5,
  VCNL4010_PROX_RATE_125 = 6,
  VCNL4010_PROX_RATE_250 = 7,
} vcnl4010_prox_rate_t;

// --- Proximity interrupt count (0x8E bits [6:4]) ---
typedef enum vcnl4010_int_count {
  VCNL4010_INT_COUNT_1 = 0,
  VCNL4010_INT_COUNT_2 = 1,
  VCNL4010_INT_COUNT_4 = 2,
  VCNL4010_INT_COUNT_8 = 3,
} vcnl4010_int_count_t;

// --- Threshold selection ---
typedef enum vcnl4010_threshold_selection {
  VNCL4010_THRESH_SEL_PROX = 0,
  VNCL4010_THRESH_SEL_ALS = 1,
} vcnl4010_threshold_selection_t;

// --- Configuration struct ---
typedef struct vcnl4010_config {
  vcnl4010_cmd_t command;
  vcnl4010_ir_led_current_t ir_led_current;
  vcnl4010_prox_rate_t prox_rate;
  bool als_continuous;
  vcnl4010_als_rate_t als_rate;
  bool offset_comp;
  vcnl4010_als_averaging_t als_averaging;
  vcnl4010_int_count_t int_count;
  bool prox_en;
  bool als_en;
  bool threshold_en;
  vcnl4010_threshold_selection_t threshold_sel;
  uint16_t low_threshold;
  uint16_t high_threshold;
} vcnl4010_config_t;

// --- Context struct ---
struct vcnl4010_context;

// --- Handle for VCNL4010 ---
typedef struct vcnl4010_context *vcnl4010_handle_t;

/**
 * @brief  VCNL4010 initialization
 * @param  bus: I2C bus handle
 * @param  *config: VCNL4010 configuration
 * @param  *handle: VCNL4010 handle
 * @retval ESP Error code
 */
esp_err_t vcnl4010_init(i2c_master_bus_handle_t bus, vcnl4010_config_t *config,
                        vcnl4010_handle_t *handle);

/**
 * @brief  VCNL4010 deinitialize   
 * @param  handle: VCNL4010 handle
 * @retval ESP Error code
 */
esp_err_t vcnl4010_deinit(vcnl4010_handle_t handle);