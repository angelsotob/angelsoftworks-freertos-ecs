/**
 * @file sensors_task.c
 * @brief Implementation of simulated sensor functions and task.
 *
 * This module provides fake temperature and humidity readings for testing
 * and logs the simulated data in a FreeRTOS task.
 *
 * @author Ángel Soto Boullosa
 * @date 2025-06-22
 * @version 0.0.1
 */

#include "sensors_task.h"

static const char *TAG = "sensor";

/**
 * @brief FreeRTOS task that logs fake sensor readings every 100 ms.
 *
 * @param[in] pvParameter Unused. Can be NULL.
 */
void sensor_task(void *pvParameter)
{
    (void)pvParameter; // To avoid unused parameter warning

    char timestamp_str[20];

    while (1)
    {
        float temperature = get_fake_temperature();
        float humidity = get_fake_humidity();
        get_timestamp_str(timestamp_str, sizeof(timestamp_str));

        printf("%s: Temperature sensor measures: %.2f ºC\n", timestamp_str, temperature);
        printf("%s: Humidity sensor measures: %.1f %%HR\n", timestamp_str, humidity);
        ESP_LOGI(TAG, "[%s] Temperature: %.2f ºC | Humidity: %.1f %%", timestamp_str, temperature, humidity);

        vTaskDelay(pdMS_TO_TICKS(MS_100));
    }
}
