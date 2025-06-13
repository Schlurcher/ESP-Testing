#include <stdio.h>

#include "aht20.h"
#include "esp_lcd_panel_dev.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "driver/i2c_types.h"
#include "esp_err.h"
#include "i2c_utils.h"
#include "sdkconfig.h"

static const char* TAG = "aht20 test";

static i2c_master_bus_handle_t i2c_bus_0_handle = NULL;
static i2c_master_bus_handle_t i2c_bus_1_handle = NULL;
static aht20_dev_handle_t aht20_0_handle = NULL;
static aht20_dev_handle_t aht20_1_handle = NULL;

/**
 * @brief Main application entry point
 */
void app_main(void)
{
    ESP_ERROR_CHECK(i2c_master_init(CONFIG_I2C_MASTER_0_SDA, CONFIG_I2C_MASTER_0_SCL, 0, &i2c_bus_0_handle));
    ESP_ERROR_CHECK(i2c_master_init(CONFIG_I2C_MASTER_1_SDA, CONFIG_I2C_MASTER_1_SCL, 1, &i2c_bus_1_handle));

    const i2c_aht20_config_t aht20_i2c_config = {
        .i2c_config.device_address = AHT20_ADDRESS_0,
        .i2c_config.scl_speed_hz = CONFIG_I2C_MASTER_FREQ_HZ,
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
