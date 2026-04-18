idf_component_register(SRCS "i2c_helper.c"
    PRIV_REQUIRES esp_driver_gpio esp_driver_i2c k0i05__esp_ssd1306
    INCLUDE_DIRS "include")
