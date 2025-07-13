/**
 * @file control_task.h
 * @brief Control task header for managing GPIO based on sensor readings.
 *
 * This file declares the task responsible for controlling a GPIO pin 
 * based on temperature and humidity values retrieved from simulated sensors.
 *
 * @author Ángel Soto Boullosa
 * @date 2025-06-22
 * @version 0.0.1
 */

#ifndef CONTROL_TASK_H
#define CONTROL_TASK_H

#include "../include/config.h"
#include "sensors.h"
#include "../utils/utils.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <time.h>
#include "logger_task.h"
#include "watchdog_task.h"


/**
 * @brief Task that controls a GPIO pin based on environmental conditions.
 *
 * If the temperature exceeds TEMP_THRESHOLD or humidity drops below
 * HUM_THRESHOLD, the CONTROL_GPIO is set to GPIO_HIGH; otherwise,
 * it is set to GPIO_LOW.
 *
 * @param pvParameter Unused FreeRTOS task parameter.
 */
void control_task(void *pvParameter);

#endif // CONTROL_TASK_H
