#include "i2c_utils.h"
#include "driver/i2c_master.h"
#include "driver/i2c_types.h"
#include "esp_err.h"

#define I2C_DEFAULT_GLITCH_FILTER 7

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
esp_err_t i2c_master_init(int sda_io_num, int scl_io_num, int bus_num, i2c_master_bus_handle_t* i2c_handle)
{
    i2c_master_bus_config_t i2c_bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = bus_num,
        .scl_io_num = scl_io_num,
        .sda_io_num = sda_io_num,
        .glitch_ignore_cnt = I2C_DEFAULT_GLITCH_FILTER,
        .flags.enable_internal_pullup = true,
    };

    return i2c_new_master_bus(&i2c_bus_config, i2c_handle);
}
