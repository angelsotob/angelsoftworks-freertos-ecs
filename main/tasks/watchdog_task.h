/**
 * @file watchdog_task.h
 * @brief Interface for the watchdog monitoring system in FreeRTOS.
 *
 * Declares the `watchdog_task` and the shared `task_alive_flags_t`
 * structure used to detect stalled tasks and trigger system resets.
 *
 * Each monitored task should periodically set its corresponding
 * `*_alive` flag to true. The watchdog task clears the flags and
 * checks them again after a defined interval (5 seconds).
 *
 * If any flag remains false, the system is assumed to be unresponsive
 * and is restarted.
 *
 * Usage:
 *  - Include this header in each task.
 *  - Set `g_task_alive_flags.<task>_alive = true` inside the loop.
 *  - Create the `watchdog_task` at startup.
 *
 * @author Ángel Soto Boullosa
 * @date 2025-06-22
 * @version 0.0.1
 */

#ifndef WATCHDOG_TASK_H
#define WATCHDOG_TASK_H

#include <stdbool.h>
#include "../include/config.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <time.h>
#include "watchdog_task.h" 


typedef struct {
    bool sensor_alive;
    bool control_alive;
    bool logger_alive;
} task_alive_flags_t;

extern task_alive_flags_t g_task_alive_flags;

/**
 * @brief FreeRTOS task that monitors the health of system tasks.
 *
 * This task checks periodically if all critical tasks (sensor, control, logger)
 * have reported activity. If any of them fails to set its alive flag within the 
 * check interval, the system is considered unresponsive and is restarted via `esp_restart()`.
 *
 * The flags `sensor_alive`, `control_alive`, and `logger_alive` must be set to `true`
 * by each respective task in every cycle.
 *
 * @param[in] pvParameter Unused. Can be NULL.
 */
void watchdog_task(void *pvParameter);


#endif // WATCHDOG_TASK_H