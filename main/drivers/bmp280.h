#pragma once
/**
 * @file bmp280.h
 * @brief Minimal BMP280 pressure/temperature driver over I2C.
 *
 * Implements forced-mode measurement with x1 oversampling for both temperature and pressure.
 * The calibration constants are read and stored at initialization (`bmp280_init()`), and
 * applied during compensation in `bmp280_read()`.
 *
 * @par I2C Address
 * - Default: `0x77` (some boards use `0x76`).
 * - Change via `-DBMP280_I2C_ADDR=0x76` in compile flags or define before including this header.
 *
 * @note Blocking I2C access, safe for FreeRTOS task context.
 */

#include "esp_err.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BMP280_I2C_ADDR
#  define BMP280_I2C_ADDR  0x77   /**< Default I2C address (0x76 on some boards). */
#endif

/**
 * @brief Initialize the BMP280 and load its calibration constants.
 *
 * - Reads CHIP_ID (expected 0x58). If different, logs a warning but continues.
 * - Reads the calibration block (T1..T3, P1..P9) and stores it internally.
 *
 * @retval ESP_OK          Successfully initialized.
 * @retval ESP_ERR_TIMEOUT I2C timeout.
 * @retval ESP_FAIL        ID or calibration read failed.
 */
esp_err_t bmp280_init(void);

/**
 * @brief Read temperature (°C) and pressure (Pa) in forced mode (x1 oversampling).
 *
 * Sequence:
 *  1. Write `CTRL_MEAS` with `osrs_t=1`, `osrs_p=1`, `mode=forced`.
 *  2. Wait until conversion completes (`STATUS.measuring` = 0).
 *  3. Read raw 20-bit values and apply Bosch compensation formulas.
 *
 * @param[out] temperature_c  Temperature in °C (nullable).
 * @param[out] pressure_pa    Pressure in Pascals (nullable).
 *
 * @retval ESP_OK          Read and compensation successful.
 * @retval ESP_ERR_TIMEOUT I2C or conversion timeout.
 * @retval ESP_FAIL        Invalid raw data or division by zero during compensation.
 *
 * @note Blocking call (~10–20 ms typical with x1 oversampling).
 * @warning Requires successful `bmp280_init()` before the first read.
 */
esp_err_t bmp280_read(float *temperature_c, float *pressure_pa);

#ifdef __cplusplus
}
#endif
