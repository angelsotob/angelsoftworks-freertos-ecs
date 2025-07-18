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
extern QueueHandle_t xQueueLogger;
extern task_alive_flags_t g_task_alive_flags; 

/**
 * @brief FreeRTOS task that logs fake sensor readings every 100 ms.
 *
 * @param[in] pvParameter Unused. Can be NULL.
 */
void sensor_task(void *pvParameter)
{
    (void)pvParameter; // To avoid unused parameter warning
    system_state_t state;
    char timestamp_str[20];

    while (1)
    {
        g_task_alive_flags.sensor_alive = true;
        sensors_data_t data;
        data.temperature = get_fake_temperature();
        data.humidity = get_fake_humidity();
        get_timestamp_str(timestamp_str, sizeof(timestamp_str));

        if (xQueueSensors != 0)
        {
            /* Send an unsigned long. Wait for 10 ticks for space to become
               available if necessary. */
            if (xQueueSend(xQueueSensors, (void *)&data, pdMS_TO_TICKS(10)) != pdPASS)
            {
                ESP_LOGW(TAG, "Failed to send to xQueueSensors");
            }
        }

        if (xQueueLogger != 0)
        {
            state.sensors = data;
            state.alarm = false;
            /* Send an unsigned long. Wait for 10 ticks for space to become
               available if necessary. */
            xQueueOverwrite(xQueueLogger, (void *)&state);
        }

        vTaskDelay(pdMS_TO_TICKS(MS_100));
    }
}
