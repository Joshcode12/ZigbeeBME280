idf_component_register(SRCS "bme280_hal.c"
    PRIV_REQUIRES bme280_sensorapi esp_driver_i2c
    INCLUDE_DIRS "include")
