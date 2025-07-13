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
        float temperature = get_fake_temperature();
        float humidity = get_fake_humidity();
        get_timestamp_str(timestamp_str, sizeof(timestamp_str));

        if (temperature > TEMP_THRESHOLD || humidity < HUM_THRESHOLD) {
            gpio_set_level(CONTROL_GPIO, GPIO_HIGH);
        } else {
            gpio_set_level(CONTROL_GPIO, GPIO_LOW);
        }

        ESP_LOGI("control", "[%s] Control GPIO updated (Temp: %.2f ºC, Hum: %.1f %%HR)",
                 timestamp_str, temperature, humidity);

        vTaskDelay(pdMS_TO_TICKS(MS_100));
    }
}

