idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES esp_event i2c_helper bme280 vcnl4010
                    )
