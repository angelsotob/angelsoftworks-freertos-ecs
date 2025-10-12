#pragma once
/**
 * @file bmp280.h
 * @brief Minimal BMP280 pressure/temperature driver (I2C).
 */

#include "esp_err.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BMP280_I2C_ADDR
#  define BMP280_I2C_ADDR  0x77   // Many boards use 0x76; some use 0x77
#endif

esp_err_t bmp280_init(void);

/**
 * @brief Read temperature (°C) and pressure (Pa) from BMP280 (forced mode, x1 oversampling).
 *
 * @param[out] temperature_c  Temperature in °C (can be NULL).
 * @param[out] pressure_pa    Pressure in Pascals (can be NULL).
 */
esp_err_t bmp280_read(float *temperature_c, float *pressure_pa);

#ifdef __cplusplus
}
#endif
