#include <stdio.h>

#include "aht20.h"
#include "esp_lcd_panel_dev.h"
#include "esp_lcd_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "driver/i2c_types.h"
#include "esp_err.h"

static const char* TAG = "aht20 test";

#define I2C_MASTER_FREQ_HZ  100000          /*!< I2C master clock frequency */


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
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    return i2c_new_master_bus(&i2c_bus_config, i2c_handle);
}

static i2c_master_bus_handle_t i2c_bus_0_handle = NULL;
static i2c_master_bus_handle_t i2c_bus_1_handle = NULL;
static aht20_dev_handle_t aht20_0_handle = NULL;
static aht20_dev_handle_t aht20_1_handle = NULL;

void app_main(void)
{
    ESP_ERROR_CHECK(i2c_master_init(CONFIG_I2C_MASTER_0_SDA, CONFIG_I2C_MASTER_0_SCL, 0, &i2c_bus_0_handle));
    ESP_ERROR_CHECK(i2c_master_init(CONFIG_I2C_MASTER_1_SDA, CONFIG_I2C_MASTER_1_SCL, 1, &i2c_bus_1_handle));

    i2c_aht20_config_t aht20_i2c_config = {
        .i2c_config.device_address = AHT20_ADDRESS_0,
        .i2c_config.scl_speed_hz = I2C_MASTER_FREQ_HZ,
        .i2c_timeout = 100,
    };

    ESP_ERROR_CHECK(i2c_master_probe(i2c_bus_0_handle, AHT20_ADDRESS_0, 1000));
    ESP_ERROR_CHECK(i2c_master_probe(i2c_bus_1_handle, AHT20_ADDRESS_0, 1000));

    aht20_new_sensor(i2c_bus_0_handle, &aht20_i2c_config, &aht20_0_handle);
    aht20_new_sensor(i2c_bus_1_handle, &aht20_i2c_config, &aht20_1_handle);

    float temp0, hum0, temp1, hum1;
    bool led_on = false;

    while (1)
    {
        led_on = !led_on;
        aht20_read_float(aht20_0_handle, &temp0, &hum0);
        aht20_read_float(aht20_1_handle, &temp1, &hum1);
        ESP_LOGI(TAG, "Sensor 0      : %2.2f %% %2.2f °C", hum0, temp0);
        ESP_LOGI(TAG, "Sensor 1      : %2.2f %% %2.2f °C", hum1, temp1);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
