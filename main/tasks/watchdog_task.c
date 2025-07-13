/**
 * @file watchdog_task.c
 * @brief Implementation of a simple software watchdog for FreeRTOS.
 *
 * This module defines a task that periodically checks whether critical
 * system tasks (sensor, control, logger) are alive. Each monitored task
 * must set its corresponding flag (`*_alive`) to true during each loop.
 *
 * If any of the flags remain false after a period (5 seconds), the
 * watchdog assumes the task has stalled and triggers a system restart
 * using `esp_restart()`.
 *
 * The mechanism provides a basic fault recovery strategy to maintain
 * system stability in unattended deployments.
 *
 * @author Ángel Soto Boullosa
 * @date 2025-06-22
 * @version 0.0.1
 */

#include "watchdog_task.h"

static const char *TAG = "watchdog";

task_alive_flags_t g_task_alive_flags = {0};

void watchdog_task(void *pvParameter)
{
    (void)pvParameter;

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(MS_5000)); // Check each 5 s

        if (!g_task_alive_flags.sensor_alive ||
            !g_task_alive_flags.control_alive ||
            !g_task_alive_flags.logger_alive)
        {
            ESP_LOGE(TAG, "Watchdog detected a dead task. Restarting system.");
            vTaskDelay(pdMS_TO_TICKS(MS_1000)); // Time to log
            esp_restart();
        }

        // Reset flags
        g_task_alive_flags.sensor_alive = false;
        g_task_alive_flags.control_alive = false;
        g_task_alive_flags.logger_alive = false;
    }
}