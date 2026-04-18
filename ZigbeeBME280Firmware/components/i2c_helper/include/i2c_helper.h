/**
 * @brief Initialize the I2C bus and adds all the devices.
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