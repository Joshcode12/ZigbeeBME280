#pragma once

#include "driver/i2c_types.h"

extern i2c_master_bus_handle_t bus_hdl;

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