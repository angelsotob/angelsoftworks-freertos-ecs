/**
 * @file sensors_task.h
 * @brief Header file for the simulated sensor FreeRTOS task.
 *
 * Declares the FreeRTOS task that generates and logs fake sensor data.
 *
 * @author Ángel Soto Boullosa
 * @date 2025-06-22
 * @version 0.0.1
 */

#ifndef SENSORS_TASK_H
#define SENSORS_TASK_H

#include "../include/config.h"
#include "../utils/utils.h"
#include "../drivers/sensors.h"
#include <stdio.h>
#include <stdlib.h>
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <time.h>
#include "logger_task.h"
#include "watchdog_task.h"

/**
 * @brief FreeRTOS task that periodically reads sensor values.
 *
 * This task simulates temperature and humidity readings and logs them
 * every 100 ms using both printf and ESP_LOGI.
 *
 * @param[in] pvParameter Unused. Can be NULL.
 */
void sensor_task(void *pvParameter);

#endif // SENSORS_TASK_H