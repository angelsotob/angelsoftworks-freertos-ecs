/**
 * @file logger_task.c
 * @brief Implementation of the system logger task for ESP32 FreeRTOS.
 *
 * This module defines a FreeRTOS task that receives system status updates
 * through a queue (`xQueueLogger`) and logs them periodically over UART
 * and ESP-IDF's logging system. It centralizes logging from both the
 * sensor and control tasks, providing formatted output with timestamps.
 *
 * The logger task expects to receive a `system_state_t` structure containing:
 * - The latest temperature and humidity readings.
 * - The current alarm state based on thresholds.
 *
 * The task runs continuously with a delay of 1 second between log entries.
 *
 * @author Ángel Soto Boullosa
 * @date 2025-06-22
 * @version 0.0.1
 */

#include "logger_task.h"

static const char *TAG = "logger";

extern QueueHandle_t xQueueLogger;

/**
 * @brief FreeRTOS task that logs sensor data and alarm state.
 *
 * This task runs in an infinite loop, receiving system status updates
 * from a queue and logging the temperature, humidity, and alarm status.
 * It does not control any GPIOs.
 *
 * @param[in] pvParameter Unused. Can be NULL.
 */
void logger_task(void *pvParameter)
{
    (void)pvParameter;
    system_state_t systemData;
    char timestamp_str[20];

    while (1)
    {
        g_task_alive_flags.logger_alive = true;
        if ((xQueueLogger != NULL) &&
            (xQueueReceive(xQueueLogger, &systemData, pdMS_TO_TICKS(10))))
        {
            get_timestamp_str(timestamp_str, sizeof(timestamp_str));

            const char *alarm;
            if (systemData.alarm_triggered)
            {
                alarm = "ON";
            }
            else
            {
                alarm = "OFF";
            }

            ESP_LOGI(TAG, "[%s] LOG: Temp: %.2f ºC | Hum: %.1f %% | Alarm: %s",
                     timestamp_str, systemData.last_sensors.temperature, systemData.last_sensors.humidity, alarm);
        }

        vTaskDelay(pdMS_TO_TICKS(MS_1000));
    }
}
