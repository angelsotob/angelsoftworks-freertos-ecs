#pragma once
/**
 * @file aht20.h
 * @brief Minimal AHT20 temperature and humidity driver (I2C).
 *
 * Provides initialization and measurement functions for the AHT20 sensor.
 * 
 * @author Ángel Soto Boullosa
 * @date 2025-10-12
 * @version 0.0.1
 */

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef AHT20_I2C_ADDR
#  define AHT20_I2C_ADDR  0x38
#endif

/**
 * @brief Initializes the AHT20 sensor (soft initialization and basic calibration).
 *
 * @note The I2C bus must be initialized beforehand by calling `i2c_bus_init()`.
 *
 * @return
 *  - ESP_OK on success  
 *  - An appropriate ESP_ERR_* code on failure
 */
esp_err_t aht20_init(void);

/**
 * @brief Reads temperature (°C) and relative humidity (%RH) from the AHT20 sensor.
 *
 * @param[out] temperature_c Pointer to store the temperature in °C (can be NULL if not needed).
 * @param[out] humidity_rh   Pointer to store the relative humidity in %RH (can be NULL if not needed).
 *
 * @return
 *  - ESP_OK on success  
 *  - An appropriate ESP_ERR_* code on failure
 */
esp_err_t aht20_read(float *temperature_c, float *humidity_rh);

#ifdef __cplusplus
}
#endif
