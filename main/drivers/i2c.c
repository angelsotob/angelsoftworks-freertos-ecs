/**
 * @file i2c.c
 * @brief I2C master utility functions for ESP-IDF (ESP32).
 *
 * Provides simple initialization and combined write/read operations for I2C sensors.
 * Designed for use with FreeRTOS-based applications using the ESP-IDF framework.
 * 
 * @author Ángel Soto Boullosa
 * @date 2025-10-12
 * @version 0.0.1
 */

#include "i2c.h"
#include "driver/i2c.h"
#include "esp_check.h"

/**
 * @brief Initializes the I2C master bus if not already initialized.
 *
 * Configures the specified I2C port, GPIOs, and clock speed.
 * This function is safe to call multiple times — initialization
 * will only occur once.
 *
 * @return
 *  - ESP_OK on success  
 *  - ESP_FAIL or ESP_ERR_* code on driver or configuration error
 */
esp_err_t i2c_bus_init(void)
{
    static bool s_inited = false;
    if (s_inited) return ESP_OK;

    i2c_config_t cfg = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA_GPIO,
        .scl_io_num = I2C_SCL_GPIO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_CLK_HZ,
    };

    ESP_RETURN_ON_ERROR(i2c_param_config(I2C_PORT, &cfg), "I2C", "Parameter configuration failed");
    ESP_RETURN_ON_ERROR(i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0), "I2C", "Driver installation failed");

    s_inited = true;
    return ESP_OK;
}

/**
 * @brief Performs a combined I2C write and/or read operation.
 *
 * Writes data to a slave device, optionally followed by a read from the same device.
 * Both operations are executed within a single I2C transaction.
 *
 * @param[in]  addr           7-bit I2C device address
 * @param[in]  w              Pointer to data to write (can be NULL if no write)
 * @param[in]  wn             Number of bytes to write
 * @param[out] r              Pointer to buffer for read data (can be NULL if no read)
 * @param[in]  rn             Number of bytes to read
 * @param[in]  timeout_ticks  FreeRTOS ticks to wait for the transaction
 *
 * @return
 *  - ESP_OK on success  
 *  - ESP_ERR_TIMEOUT, ESP_FAIL, or other ESP_ERR_* codes on failure
 */
esp_err_t i2c_write_read(uint8_t addr,
                         const uint8_t *w, size_t wn,
                         uint8_t *r, size_t rn,
                         TickType_t timeout_ticks)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    if (!cmd) return ESP_ERR_NO_MEM;

    if (wn) {
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true);
        i2c_master_write(cmd, (uint8_t *)w, wn, true);
    }

    if (rn) {
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_READ, true);
        i2c_master_read(cmd, r, rn, I2C_MASTER_LAST_NACK);
    }

    i2c_master_stop(cmd);
    esp_err_t err = i2c_master_cmd_begin(I2C_PORT, cmd, timeout_ticks);
    i2c_cmd_link_delete(cmd);

    return err;
}
