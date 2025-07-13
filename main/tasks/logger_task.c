/**
 * @file logger_task.c
 * @brief
 *
 * @author Ángel Soto Boullosa
 * @date 2025-06-22
 * @version 0.0.1
 */

#include "logger_task.h"

static const char *TAG = "logger";

extern QueueHandle_t xQueueLogger;

/**
 * @brief FreeRTOS task that controls a GPIO based on environmental thresholds.
 *
 * This task runs in an infinite loop, simulating the reading of temperature
 * and humidity sensors. If the temperature exceeds TEMP_THRESHOLD or
 * the humidity falls below HUM_THRESHOLD, it sets the CONTROL_GPIO
 * pin to HIGH. Otherwise, it sets the pin to LOW.
 *
 * It also logs the timestamp and sensor values to the console.
 *
 * @param[in] pvParameter Unused task parameter. Can be NULL.
 */
void logger_task(void *pvParameter)
{
    (void)pvParameter;
    system_state_t systemData;
    char timestamp_str[20];

    while (1)
    {
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

            printf("%s: Temperature sensor measures: %.2f ºC\n", timestamp_str, systemData.last_sensors.temperature);
            printf("%s: Humidity sensor measures: %.1f %%HR\n", timestamp_str, systemData.last_sensors.humidity);
            ESP_LOGI(TAG, "[%s] Temperature: %.2f ºC | Humidity: %.1f %%", timestamp_str, systemData.last_sensors.temperature, systemData.last_sensors.humidity);

            ESP_LOGI("logger", "[%s] LOG: Temp: %.2f ºC | Hum: %.1f %% | Alarm: %s",
                     timestamp_str, systemData.last_sensors.temperature, systemData.last_sensors.humidity, alarm);
        }

        vTaskDelay(pdMS_TO_TICKS(MS_1000));
    }
}
