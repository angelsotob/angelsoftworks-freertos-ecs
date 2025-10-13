// main/drivers/i2c.h
#pragma once

/**
 * @file i2c.h
 * @brief I2C master utility interface for ESP-IDF (ESP32).
 *
 * Provides simple initialization and combined write/read operations for I2C devices.
 * 
 * @author Ángel Soto Boullosa
 * @date 2025-10-12
 * @version 0.0.1
 */

#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include <stddef.h>
#include <stdint.h>

/** @brief I2C port to use (override at compile time if needed). */
#ifndef I2C_PORT
#  define I2C_PORT        I2C_NUM_0
#endif

/** @brief Default GPIO for SDA (override at compile time if needed). */
#ifndef I2C_SDA_GPIO
#  define I2C_SDA_GPIO    21
#endif

/** @brief Default GPIO for SCL (override at compile time if needed). */
#ifndef I2C_SCL_GPIO
#  define I2C_SCL_GPIO    22
#endif

/** @brief I2C bus frequency in Hz (override at compile time if needed). */
#ifndef I2C_CLK_HZ
#  define I2C_CLK_HZ      400000
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the I2C master bus (idempotent).
 *
 * Configures the selected I2C port with the given pins and clock speed.
 * Safe to call multiple times; subsequent calls return ESP_OK without reinitializing.
 *
 * @return
 *  - ESP_OK on success
 *  - An ESP_ERR_* code if configuration or driver installation fails
 */
esp_err_t i2c_bus_init(void);

/**
 * @brief Perform a combined I2C write and/or read transaction.
 *
 * If @p wn > 0, a write is performed. If @p rn > 0, a read is performed.
 * Both operations occur within a single transaction on the same slave address.
 *
 * @param[in]  addr          7-bit I2C device address
 * @param[in]  w             Pointer to data to write (NULL if no write)
 * @param[in]  wn            Number of bytes to write
 * @param[out] r             Pointer to buffer for read data (NULL if no read)
 * @param[in]  rn            Number of bytes to read
 * @param[in]  timeout_ticks FreeRTOS ticks to wait for the transaction
 *
 * @return
 *  - ESP_OK on success
 *  - ESP_ERR_TIMEOUT on bus timeout
 *  - Other ESP_ERR_* codes on failure
 */
esp_err_t i2c_write_read(uint8_t addr,
                         const uint8_t *w, size_t wn,
                         uint8_t *r, size_t rn,
                         TickType_t timeout_ticks);

/**
 * @brief Convenience wrapper: write-only I2C transaction.
 */
static inline esp_err_t i2c_write_bytes(uint8_t addr, const uint8_t *w, size_t wn, TickType_t to_ticks)
{
    return i2c_write_read(addr, w, wn, NULL, 0, to_ticks);
}

/**
 * @brief Convenience wrapper: read-only I2C transaction.
 */
static inline esp_err_t i2c_read_bytes(uint8_t addr, uint8_t *r, size_t rn, TickType_t to_ticks)
{
    return i2c_write_read(addr, NULL, 0, r, rn, to_ticks);
}

#ifdef __cplusplus
}
#endif
