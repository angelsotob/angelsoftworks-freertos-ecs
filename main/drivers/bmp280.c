/**
 * @file bmp280.c
 * @brief Minimal BMP280 pressure/temperature driver (I2C).
 *
 * Forced-mode, x1 oversampling. Reads calibration at init and compensates raw data.
 */

#include "bmp280.h"
#include "i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <string.h>
#include "esp_check.h" 


static const char *TAG = "BMP280";

/* Registers */
#define REG_CHIP_ID     0xD0
#define REG_RESET       0xE0
#define REG_STATUS      0xF3
#define REG_CTRL_MEAS   0xF4
#define REG_CONFIG      0xF5
#define REG_PRESS_MSB   0xF7  // F7..F9
#define REG_TEMP_MSB    0xFA  // FA..FC

/* Calibration start */
#define REG_CALIB_START 0x88
#define CALIB_LEN       24    // T1..T3 (6B) + P1..P9 (18B)

#define CHIP_ID_EXPECTED 0x58

/* Status bits */
#define STATUS_MEASURING (1 << 3)

/* CTRL_MEAS fields */
#define OSRS_T_x1   (1 << 5)  // 001 << 5
#define OSRS_P_x1   (1 << 2)  // 001 << 2
#define MODE_FORCED 0x01      // forced mode (01)

/* CONFIG: optional IIR/filter, standby not relevant in forced mode */
#define CONFIG_IIR_OFF  (0 << 2)

typedef struct {
    uint16_t T1; int16_t T2; int16_t T3;
    uint16_t P1; int16_t P2; int16_t P3; int16_t P4; int16_t P5;
    int16_t  P6; int16_t P7; int16_t P8; int16_t P9;
} bmp280_calib_t;

static bmp280_calib_t s_cal;
static bool s_inited = false;

/* Helpers */
static inline esp_err_t wr8(uint8_t reg, uint8_t val, TickType_t to)
{
    uint8_t buf[2] = {reg, val};
    return i2c_write_bytes(BMP280_I2C_ADDR, buf, sizeof(buf), to);
}

static inline esp_err_t rd(uint8_t reg, uint8_t *dst, size_t n, TickType_t to)
{
    return i2c_write_read(BMP280_I2C_ADDR, &reg, 1, dst, n, to);
}

esp_err_t bmp280_init(void)
{
    if (s_inited) return ESP_OK;

    /* Read chip id */
    uint8_t id = 0;
    ESP_RETURN_ON_ERROR(rd(REG_CHIP_ID, &id, 1, pdMS_TO_TICKS(50)), TAG, "chip id read failed");
    if (id != CHIP_ID_EXPECTED) {
        ESP_LOGW(TAG, "Unexpected CHIP_ID=0x%02X (expected 0x58). Continuing anyway.", id);
    }

    /* Read calibration data */
    uint8_t calib[CALIB_LEN] = {0};
    ESP_RETURN_ON_ERROR(rd(REG_CALIB_START, calib, sizeof(calib), pdMS_TO_TICKS(50)), TAG, "calib read failed");

    s_cal.T1 = (uint16_t)(calib[1]  << 8 | calib[0]);
    s_cal.T2 = (int16_t) (calib[3]  << 8 | calib[2]);
    s_cal.T3 = (int16_t) (calib[5]  << 8 | calib[4]);
    s_cal.P1 = (uint16_t)(calib[7]  << 8 | calib[6]);
    s_cal.P2 = (int16_t) (calib[9]  << 8 | calib[8]);
    s_cal.P3 = (int16_t) (calib[11] << 8 | calib[10]);
    s_cal.P4 = (int16_t) (calib[13] << 8 | calib[12]);
    s_cal.P5 = (int16_t) (calib[15] << 8 | calib[14]);
    s_cal.P6 = (int16_t) (calib[17] << 8 | calib[16]);
    s_cal.P7 = (int16_t) (calib[19] << 8 | calib[18]);
    s_cal.P8 = (int16_t) (calib[21] << 8 | calib[20]);
    s_cal.P9 = (int16_t) (calib[23] << 8 | calib[22]);

    /* Optional: CONFIG (IIR off) */
    ESP_RETURN_ON_ERROR(wr8(REG_CONFIG, CONFIG_IIR_OFF, pdMS_TO_TICKS(50)), TAG, "config failed");

    s_inited = true;
    return ESP_OK;
}

esp_err_t bmp280_read(float *temperature_c, float *pressure_pa)
{
    if (!s_inited) {
        esp_err_t e = bmp280_init();
        if (e != ESP_OK) return e;
    }

    /* Trigger forced measurement: osrs_t=1, osrs_p=1, mode=forced */
    ESP_RETURN_ON_ERROR(wr8(REG_CTRL_MEAS, OSRS_T_x1 | OSRS_P_x1 | MODE_FORCED, pdMS_TO_TICKS(50)), TAG, "ctrl_meas failed");

    /* Wait while measuring */
    uint8_t status = STATUS_MEASURING;
    TickType_t t0 = xTaskGetTickCount();
    const TickType_t timeout = pdMS_TO_TICKS(100);
    do {
        ESP_RETURN_ON_ERROR(rd(REG_STATUS, &status, 1, pdMS_TO_TICKS(20)), TAG, "status read failed");
        if (!(status & STATUS_MEASURING)) break;
        vTaskDelay(pdMS_TO_TICKS(5));
    } while ((xTaskGetTickCount() - t0) < timeout);

    /* Read raw pressure (20-bit) and temperature (20-bit) */
    uint8_t raw[6] = {0};
    ESP_RETURN_ON_ERROR(rd(REG_PRESS_MSB, raw, sizeof(raw), pdMS_TO_TICKS(50)), TAG, "raw read failed");

    int32_t adc_P = ((int32_t)raw[0] << 12) | ((int32_t)raw[1] << 4) | (raw[2] >> 4);
    int32_t adc_T = ((int32_t)raw[3] << 12) | ((int32_t)raw[4] << 4) | (raw[5] >> 4);

    if (adc_T == 0x80000 || adc_P == 0x80000) return ESP_FAIL; // invalid

    /* Temperature compensation (Bosch formula) */
    int32_t var1 = ((((adc_T >> 3) - ((int32_t)s_cal.T1 << 1))) * (int32_t)s_cal.T2) >> 11;
    int32_t var2 = (((((adc_T >> 4) - (int32_t)s_cal.T1) * ((adc_T >> 4) - (int32_t)s_cal.T1)) >> 12) * (int32_t)s_cal.T3) >> 14;
    int32_t t_fine = var1 + var2;
    int32_t T = (t_fine * 5 + 128) >> 8;  // 0.01 °C

    if (temperature_c) *temperature_c = (float)T / 100.0f;

    /* Pressure compensation (Bosch formula, 64-bit intermediates) */
    int64_t var1_p = ((int64_t)t_fine) - 128000;
    int64_t var2_p = var1_p * var1_p * (int64_t)s_cal.P6;
    var2_p += ((var1_p * (int64_t)s_cal.P5) << 17);
    var2_p += (((int64_t)s_cal.P4) << 35);
    var1_p = (((var1_p * var1_p * (int64_t)s_cal.P3) >> 8) + ((var1_p * (int64_t)s_cal.P2) << 12));
    var1_p = (((((int64_t)1) << 47) + var1_p) * (int64_t)s_cal.P1) >> 33;

    if (var1_p == 0) {
        if (pressure_pa) *pressure_pa = 0.0f;
        return ESP_FAIL; // avoid division by zero
    }

    int64_t p = 1048576 - adc_P;
    p = (((p << 31) - var2_p) * 3125) / var1_p;
    var1_p = (((int64_t)s_cal.P9) * (p >> 13) * (p >> 13)) >> 25;
    var2_p = (((int64_t)s_cal.P8) * p) >> 19;
    p = ((p + var1_p + var2_p) >> 8) + (((int64_t)s_cal.P7) << 4);

    if (pressure_pa) *pressure_pa = (float)p / 256.0f;  // Pa

    return ESP_OK;
}
