/**
 * @file control_task.c
 * @brief Implementation of the control task logic.
 *
 * This task checks simulated temperature and humidity values and activates
 * a GPIO output based on defined thresholds.
 *
 * @author Ángel Soto Boullosa
 * @date 2025-06-22
 * @version 0.0.1
 */

#include "control_task.h"

sensors_data_t received;
extern QueueHandle_t xQueueSensors;

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
void control_task(void *pvParameter)
{
    (void)pvParameter;

    char timestamp_str[20];

    while (1)
    {
        if ((xQueueSensors != NULL) && 
            (xQueueReceive(xQueueSensors, &received, pdMS_TO_TICKS(10))))
        {
            if (received.temperature > TEMP_THRESHOLD || received.humidity < HUM_THRESHOLD)
            {
                gpio_set_level(CONTROL_GPIO, GPIO_HIGH);
                get_timestamp_str(timestamp_str, sizeof(timestamp_str));
                ESP_LOGW("control", "[%s] ALARM TRIGGERED! Temp: %.2f ºC | Hum: %.1f %%", 
                         timestamp_str, received.temperature, received.humidity);
            }
            else
            {
                gpio_set_level(CONTROL_GPIO, GPIO_LOW);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(MS_100));
    }
}

