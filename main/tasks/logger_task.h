/**
 * @file logger_task.h
 * @brief 
 *
 * @author Ángel Soto Boullosa
 * @date 2025-06-22
 * @version 0.0.1
 */

#ifndef LOGGER_TASK_H
#define LOGGER_TASK_H

#include "../include/config.h"
#include "sensors.h"
#include "../utils/utils.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <time.h>

typedef struct{
    sensors_data_t last_sensors;
    bool alarm_triggered;
} system_state_t;


/**
 * @brief Task that controls a GPIO pin based on environmental conditions.
 *
 * If the temperature exceeds TEMP_THRESHOLD or humidity drops below
 * HUM_THRESHOLD, the CONTROL_GPIO is set to GPIO_HIGH; otherwise,
 * it is set to GPIO_LOW.
 *
 * @param pvParameter Unused FreeRTOS task parameter.
 */
void logger_task(void *pvParameter);

#endif // LOGGER_TASK_H
