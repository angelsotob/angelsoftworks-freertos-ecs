/**
 * @file aht20.c
 * @brief Minimal AHT20 temperature and humidity driver (I2C).
 *
 * Implements initialization and measurement routines for the AHT20 sensor.
 * Communication is done over I2C using simple read/write transactions.
 * 
 * @author Ángel Soto Boullosa
 * @date 2025-10-12
 * @version 0.0.1
 */

#include "aht_20.h"
#include "i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "AHT20";

/*
 * According to the datasheet:
 *  - Initialization command : 0xBE 0x08 0x00
 *  - Measurement command     : 0xAC 0x33 0x00  (typical conversion time ~80–85 ms)
 *  - Readout: 6 bytes
 *
 *  - Calculation formulas:
 *      Humidity (20 bits): (b1 << 12) | (b2 << 4) | (b3 >> 4)
 *          -> RH% = raw / 2^20 * 100
 *
 *      Temperature (20 bits): ((b3 & 0x0F) << 16) | (b4 << 8) | b5
 *          -> °C = raw / 2^20 * 200 - 50
 */

esp_err_t aht20_init(void)
{
    const uint8_t init_cmd[3] = {0xBE, 0x08, 0x00};
    esp_err_t err = i2c_write_read(AHT20_I2C_ADDR, init_cmd, sizeof(init_cmd), NULL, 0, pdMS_TO_TICKS(50));

    if (err != ESP_OK) {
        ESP_LOGW(TAG, "Initialization write failed: %s", esp_err_to_name(err));
        return err;
    }

    vTaskDelay(pdMS_TO_TICKS(20));
    return ESP_OK;
}

esp_err_t aht20_read(float *temperature_c, float *humidity_rh)
{
    const uint8_t measure_cmd[3] = {0xAC, 0x33, 0x00};
    uint8_t data[6] = {0};

    esp_err_t err = i2c_write_read(AHT20_I2C_ADDR, measure_cmd, sizeof(measure_cmd), NULL, 0, pdMS_TO_TICKS(50));
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "Measurement command failed: %s", esp_err_to_name(err));
        return err;
    }

    vTaskDelay(pdMS_TO_TICKS(85)); // typical conversion time

    err = i2c_read_bytes(AHT20_I2C_ADDR, data, sizeof(data), pdMS_TO_TICKS(50));
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "Data read failed: %s", esp_err_to_name(err));
        return err;
    }

    // Optional: check the BUSY bit (data[0] & 0x80 == 0 means ready)
    uint32_t raw_h = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | (data[3] >> 4);
    uint32_t raw_t = ((uint32_t)(data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | data[5];

    if (humidity_rh) {
        *humidity_rh = (raw_h / 1048576.0f) * 100.0f;
    }
    if (temperature_c) {
        *temperature_c = (raw_t / 1048576.0f) * 200.0f - 50.0f;
    }

    return ESP_OK;
}
