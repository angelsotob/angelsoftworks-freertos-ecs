/**
 * @file config.h
 * @brief Configuration constants for the blink application.
 *
 * This file defines hardware-specific and timing constants used
 * by the FreeRTOS blink example application.
 *
 * @author Ángel Soto Boullosa
 * @date 2025-06-22
 * @version 0.0.1
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "wifi_config.h"

/** @name GPIO Configuration
 *  GPIO-related pin assignments and levels
 *  @{
 */
#define BLINK_GPIO GPIO_NUM_2   ///< GPIO pin number 2.
#define CONTROL_GPIO GPIO_NUM_4 ///< GPIO pin number 4.
#define GPIO_HIGH 1 // Digital HIGH level (3.3V)
#define GPIO_LOW  0 // Digital LOW level (0V)
/** @} */

/** @name Time Configuration
 *  @{
 */
#define HIGH_TIME_MS 500 // LED on-time in milliseconds.
#define LOW_TIME_MS 500  // LED off-time in milliseconds.
#define MS_100 100       // Macro for 100 milliseconds.
#define MS_1000 1000     // Macro for 1000 milliseconds.
#define MS_5000 5000     // Macro for 5000 milliseconds.
/** @} */

/** @name Sensor Thresholds
 *  @{
 */
#define TEMP_THRESHOLD 25.0 // Trigger control if temperature > 25.0 ºC
#define HUM_THRESHOLD 50.0  // Trigger control if humidity < 50.0 %
/** @} */

#endif // CONFIG_H
