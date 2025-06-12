#pragma once

#include "driver/i2c_master.h"
#include "esp_err.h"

/**
 * @brief Initialize I2C master bus
 * @param sda_io_num GPIO number for I2C SDA signal
 * @param scl_io_num GPIO number for I2C SCL signal 
 * @param bus_num I2C port number
 * @param[out] i2c_handle Pointer to store the I2C bus handle
 * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG Parameter error
 *     - ESP_FAIL Driver install error
 */
esp_err_t i2c_master_init(int sda_io_num, int scl_io_num, int bus_num, i2c_master_bus_handle_t* i2c_handle);
