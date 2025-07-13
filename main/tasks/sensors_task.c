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
extern QueueHandle_t xQueueSensors;

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
        sensors_data_t data;
        data.temperature = get_fake_temperature();
        data.humidity = get_fake_humidity();
        get_timestamp_str(timestamp_str, sizeof(timestamp_str));

        if (xQueueSensors != 0)
        {
            /* Send an unsigned long. Wait for 10 ticks for space to become
               available if necessary. */
            if (xQueueSend(xQueueSensors,
                           (void *)&data,
                           pdMS_TO_TICKS(10)))
            {
                /* Failed to post the message, even after 10 ticks. */
            }
        }

        printf("%s: Temperature sensor measures: %.2f ºC\n", timestamp_str, data.temperature);
        printf("%s: Humidity sensor measures: %.1f %%HR\n", timestamp_str, data.humidity);
        ESP_LOGI(TAG, "[%s] Temperature: %.2f ºC | Humidity: %.1f %%", timestamp_str, data.temperature, data.humidity);

        vTaskDelay(pdMS_TO_TICKS(MS_100));
    }
}
