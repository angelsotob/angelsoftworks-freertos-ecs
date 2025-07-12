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

/**
 * @brief GPIO pin connected to the LED.
 */
#define BLINK_GPIO GPIO_NUM_2

/**
 * @brief Logic level to turn the LED on.
 */
#define GPIO_HIGH 1

/**
 * @brief Logic level to turn the LED off.
 */
#define GPIO_LOW 0

/**
 * @brief LED on-time in milliseconds.
 */
#define HIGH_TIME_MS 500

/**
 * @brief LED off-time in milliseconds.
 */
#define LOW_TIME_MS 500

#define MS_100 100

#define WIFI_SSID      "2_4_wifi"
#define WIFI_PASS      "1234567909765421"

#endif // CONFIG_H

