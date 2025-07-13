/**
 * @file main.c
 * @brief Main source file for the FreeRTOS ESP32 Project.
 *
 * This application demonstrates a simple FreeRTOS with some tasks:
 *
 * * One GPIO that toggles.
 * * SensorTask that gathers information about the environment.
 * * ControlTask that operates in concordance with the environmental information.
 *
 * @author Ángel Soto Boullosa
 * @date 2025-06-22
 * @version 0.0.1
 */

#include "../include/config.h"
#include "network/wifi.h"
#include "tasks/blink_task.h"
#include "tasks/control_task.h"
#include "tasks/sensors_task.h"
#include "tasks/watchdog_task.h"
#include "tasks/logger_task.h"
#include "utils/utils.h"

extern void init_gpio(void);
extern void wifi_init_sta(void);
extern void sync_time(void);

static const char *TAG = "main";

// Private function declarations
static void create_tasks(void);

QueueHandle_t xQueueSensors;
QueueHandle_t xQueueLogger;

/**
 * @brief Creates all FreeRTOS tasks in the application.
 *
 * This function creates the main application tasks: `blink_task`,
 * `sensor_task`, and `control_task`. If any of the tasks fail to
 * be created, an error is logged using `ESP_LOGE`.
 */
static void create_tasks(void)
{
    if (xTaskCreate(&blink_task, "blink_task", 1024, NULL, 5, NULL) != pdPASS)
        ESP_LOGE(TAG, "Failed to create blink_task");

    if (xTaskCreate(&sensor_task, "sensor_task", 2048, NULL, 5, NULL) != pdPASS)
        ESP_LOGE(TAG, "Failed to create sensor_task");

    if (xTaskCreate(&control_task, "control_task", 2048, NULL, 5, NULL) != pdPASS)
        ESP_LOGE(TAG, "Failed to create control_task");

    if (xTaskCreate(&logger_task, "logger_task", 2048, NULL, 5, NULL) != pdPASS)
        ESP_LOGE(TAG, "Failed to create logger_task");

    if (xTaskCreate(&watchdog_task, "watchdog_task", 2048, NULL, 5, NULL) != pdPASS)
        ESP_LOGE(TAG, "Failed to create watchdog_task");
}

/**
 * @brief Main application entry point.
 */
int app_main(void)
{
    srand((unsigned)time(NULL));
    init_gpio();
    wifi_init_sta();
    sync_time();

    xQueueSensors = xQueueCreate(10, sizeof(sensors_data_t));

    if (xQueueSensors == NULL)
    {
        printf("xQueueSensors was not created correctly");
        return -1;
    }

    xQueueLogger = xQueueCreate(1, sizeof(system_state_t));

    if (xQueueLogger == NULL)
    {
        printf("xQueueLogger was not created correctly");
        return -1;
    }

    create_tasks();

    return 0;
}