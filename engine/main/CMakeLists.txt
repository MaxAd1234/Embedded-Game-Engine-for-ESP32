idf_component_register(SRCS "main.cpp"
                       PRIV_REQUIRES esp_driver_gpio esp_timer Input esp_lcd World esp_driver_ledc audio
                       INCLUDE_DIRS "")
