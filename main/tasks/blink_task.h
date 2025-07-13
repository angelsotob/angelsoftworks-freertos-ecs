/**
 * @file blink_task.h
 * @brief Header file for the blinking LED task.
 *
 * Declares the FreeRTOS task that toggles an output GPIO to simulate
 * a blinking LED.
 *
 * @author Ángel Soto Boullosa
 * @date 2025-06-22
 * @version 0.0.1
 */

#ifndef BLINK_TASK_H
#define BLINK_TASK_H

#include "../include/config.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <time.h>
#include "watchdog_task.h" 

/**
 * @brief FreeRTOS task that toggles an LED GPIO.
 *
 * The task periodically sets the BLINK_GPIO pin high and low,
 * using the timing constants defined in config.h.
 *
 * @param pvParameter Optional parameter (unused).
 */
void blink_task(void *pvParameter);

#endif // BLINK_TASK_H
