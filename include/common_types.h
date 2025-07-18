/**
 * @file common_types.h
 * @brief Common type definitions for system-wide data structures.
 *
 * This file defines shared data types used across multiple modules
 * in the FreeRTOS-based embedded system, such as sensor data,
 * system state representation, and task liveness flags.
 *
 * By centralizing these structures, we ensure consistency and
 * modularity across components like logger, control, and sensor tasks.
 * 
 * @author Ángel Soto Boullosa
 * @date 2025-07-13
 * @version 0.0.1
 */

#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <stdbool.h>

/**
 * @brief Structure containing sensor data (temperature and humidity).
 */
typedef struct
{
    float temperature; /**< Temperature value in degrees Celsius */
    float humidity;    /**< Humidity percentage (0–100%) */
} sensors_data_t;

/**
 * @brief Structure representing system-wide state data.
 */
typedef struct
{
    sensors_data_t sensors; /**< Last acquired sensor data */
    bool alarm;       /**< Current state of the relay */
} system_state_t;

/**
 * @brief Flags indicating whether each task is alive.
 */
typedef struct
{
    bool sensor_alive;   /**< True if the sensors task is running */
    bool control_alive;  /**< True if the control task is running */
    bool logger_alive;   /**< True if the logger task is running */
} task_alive_flags_t;

#endif // COMMON_TYPES_H