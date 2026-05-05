#pragma once

#include "bme280.h"
#include "ssd1306.h"
#include "vcnl4010.h"

extern ssd1306_handle_t display_hdl;
extern bme280_handle_t bme280_hdl;
extern vcnl4010_handle_t vcnl4010_hdl;

/**
 * @brief  Initialize the I2C bus.
 */
void i2c_init(void);

/**
 * @brief Deinitialize the I2C bus and all the devices.
 */
void i2c_deinit(void);

/**
 * @brief Scans the i2c bus, and displays all the addresses.
 */
void i2c_bus_scan(void);