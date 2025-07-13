/**
 * @file blink_task.c
 * @brief Implementation of the blinking LED task.
 *
 * This file contains the implementation of a FreeRTOS task that toggles
 * a GPIO pin to simulate a blinking LED at a defined frequency.
 *
 * @author Ángel Soto Boullosa
 * @date 2025-06-22
 * @version 0.0.1
 */

#include "blink_task.h"

/**
 * @brief Task that blinks the LED on and off at regular intervals.
 *
 * The LED connected to the pin defined by BLINK_GPIO toggles every
 * HIGH_TIME_MS and LOW_TIME_MS milliseconds.
 *
 * @param pvParameter Unused parameter (can be NULL).
 */
void blink_task(void *pvParameter)
{
    (void)pvParameter; // To avoid unused parameter warning

    while (1)
    {
        gpio_set_level(BLINK_GPIO, GPIO_HIGH);
        vTaskDelay(pdMS_TO_TICKS(HIGH_TIME_MS));
        gpio_set_level(BLINK_GPIO, GPIO_LOW);
        vTaskDelay(pdMS_TO_TICKS(LOW_TIME_MS));
    }
}
