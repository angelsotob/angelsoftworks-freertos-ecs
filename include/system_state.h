/**
 * @file system_state.h
 * @brief Shared system state structure for FreeRTOS tasks.
 *
 * Defines the `system_state_t` structure, used to communicate
 * sensor data and alarm status between tasks via queues.
 *
 * @author Ángel Soto Boullosa
 * @date 2025-07-13
 * @version 0.0.1
 */

#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H

#include "sensors.h"
#include <stdbool.h>

/**
 * @brief Struct holding the latest sensor values and alarm flag.
 */
typedef struct {
    sensors_data_t last_sensors;
    bool alarm_triggered;
} system_state_t;

#endif // SYSTEM_STATE_H