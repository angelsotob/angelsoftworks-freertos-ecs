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
 * @version 0.0.2
 */

#include "sensors_task.h"
#include "esp_log.h"

#include "i2c.h"       // Custom I2C driver
#include "aht_20.h"    // Custom AHT20 driver

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

    // Keep the last valid data to avoid false triggers at startup
    sensors_data_t data = {
        .temperature = 25.0f,
        .humidity    = 50.0f
    };

    const TickType_t period_ticks = pdMS_TO_TICKS(MS_1000);

    while (1)
    {
        g_task_alive_flags.sensor_alive = true;

        float t = 0.0f, h = 0.0f;
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

        get_timestamp_str(timestamp_str, sizeof(timestamp_str));

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
