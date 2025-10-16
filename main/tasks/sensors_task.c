/**
 * @file sensors_task.c
 * @brief Reads data from the AHT20 sensor over I2C and publishes it to xQueueSensors.
 *
 * Wiring (ESP32-WROOM-32 on breadboard):
 *   SDA -> GPIO21
 *   SCL -> GPIO22
 *   VCC -> 3V3
 *   GND -> GND
 *
 * This task periodically reads temperature and humidity from the AHT20 sensor
 * and sends the data to xQueueSensors for further processing by other tasks.
 * If a read fails, it keeps the last valid data to avoid invalid zero readings.
 *
 * @author Ángel Soto Boullosa
 * @date 2025-10-12
 * @version 0.0.3
 */

#include "sensors_task.h"
#include "esp_log.h"

#include "i2c.h"       // I2C helpers
#include "aht_20.h"    // AHT20 driver
#include "bmp280.h"    // BMP280 driver (pressure) - log only

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// NEW: expose live readings to the web UI via /api/data
#include "data_snapshot.h"   // data_snapshot_update(...)
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

static const char *TAG = "sensor";

extern QueueHandle_t xQueueSensors;
extern task_alive_flags_t g_task_alive_flags;

/**
 * @brief FreeRTOS task that reads AHT20 data over I2C and publishes it to xQueueSensors.
 *
 * Sampling rate: 1 second (aligned with logger).
 * For finer granularity, you may reduce it to 100 ms.
 *
 * @param[in] pvParameter Unused. Can be NULL.
 */
void sensor_task(void *pvParameter)
{
    (void)pvParameter; // Prevent unused parameter warning
    char timestamp_str[20];

    // --- Hardware initialization (run once) ---
    ESP_ERROR_CHECK(i2c_bus_init());
    ESP_ERROR_CHECK(aht20_init());

    // Initialize BMP280 (pressure + temp). If your board uses 0x77,
    // define BMP280_I2C_ADDR=0x77 (compile flag or config header).
    esp_err_t bmp_ok = bmp280_init();

    for (int i = 0; i < 5; ++i)
    {
        vTaskDelay(pdMS_TO_TICKS(10)); // power-up settle
        float t_bmp, p_pa;
        esp_err_t e = bmp280_read(&t_bmp, &p_pa);
        if (e == ESP_OK)
            break;
        ESP_LOGW(TAG, "BMP280 first read retry %d: %s", i + 1, esp_err_to_name(e));
    }

    if (bmp_ok != ESP_OK) {
        ESP_LOGW(TAG, "BMP280 init failed: %s", esp_err_to_name(bmp_ok));
    }

    // Keep the last valid data to avoid false triggers at startup
    sensors_data_t data = {
        .temperature = 25.0f,
        .humidity    = 50.0f,
        .pressure_hpa = 1013.25f
    };

    const TickType_t period_ticks = pdMS_TO_TICKS(MS_1000);

    while (1)
    {
        g_task_alive_flags.sensor_alive = true;

        // --- AHT20 (temp + humidity) -> app data path via queue ---
        float t = 0.0f;
        float h = 0.0f;
        esp_err_t err = aht20_read(&t, &h);

        if (err == ESP_OK)
        {
            // Defensive clamp in case the sensor returns out-of-range values
            if (h < 0.0f)   h = 0.0f;
            if (h > 100.0f) h = 100.0f;

            data.temperature = t;
            data.humidity    = h;
        }
        else
        {
            ESP_LOGW(TAG, "AHT20 read failed: %s", esp_err_to_name(err));
            // Keep the last valid reading to avoid 0%/0°C outputs
        }

        // --- BMP280 (pressure + temp) -> log only, no queues/struct changes ---
        if (bmp_ok == ESP_OK) {
            float t_bmp = 0.0f;
            float p_pa = 0.0f;
            esp_err_t e = bmp280_read(&t_bmp, &p_pa);
            if (e == ESP_OK) {
                data.pressure_hpa = p_pa / 100.0f; // store in hPa
            } else {
                ESP_LOGW(TAG, "BMP280 read failed: %s", esp_err_to_name(e));
            }
        }

        get_timestamp_str(timestamp_str, sizeof(timestamp_str));

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        // NEW: update the live data snapshot for /api/data
        // For now we set alarm_on=false here; the real alarm flag can be
        // updated from control_task via another data_snapshot_update(...)
        data_snapshot_update(
            data.temperature,
            data.humidity,
            data.pressure_hpa,
            false /* alarm_on placeholder; control_task should update */,
            0     /* let the module timestamp with esp_timer_get_time() */
        );
        // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

        // Always publish the latest valid data (new or previous)
        if (xQueueSensors != NULL)
        {
            if (xQueueSend(xQueueSensors, (void *)&data, pdMS_TO_TICKS(10)) != pdPASS)
            {
                ESP_LOGW(TAG, "Failed to send to xQueueSensors");
            }
        }

        vTaskDelay(period_ticks);
    }
}
