/**
 * @file logger_task.h
 * @brief Logger task interface for centralized system logging in FreeRTOS.
 *
 * Declares the `logger_task` function and the `system_state_t` structure,
 * used for centralized logging of sensor data and alarm states.
 *
 * The logger receives data from other tasks (e.g., sensor and control)
 * via `xQueueLogger`. It prints formatted logs to UART and the ESP-IDF
 * logging system, including timestamps and environmental status.
 *
 * The queue should be created with `xQueueCreate(1, sizeof(system_state_t))`
 * and updated using `xQueueOverwrite()` by producers.
 *
 * This task is intended to run at low priority with 1-second periodic logging.
 *
 * @author Ángel Soto Boullosa
 * @date 2025-06-22
 * @version 0.0.1
 */

#ifndef LOGGER_TASK_H
#define LOGGER_TASK_H

#include "../include/config.h"
#include "../include/system_state.h"
#include "sensors.h"
#include "../utils/utils.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <time.h>
#include "watchdog_task.h"

/**
 * @brief FreeRTOS task that logs sensor data and alarm state.
 *
 * This task runs in an infinite loop, receiving system status updates
 * from a queue and logging the temperature, humidity, and alarm status.
 * It does not control any GPIOs.
 *
 * @param[in] pvParameter Unused. Can be NULL.
 */
void logger_task(void *pvParameter);

#endif // LOGGER_TASK_H
