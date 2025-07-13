/**
 * @file gpio.h
 * @brief GPIO initialization utilities for the application.
 *
 * This file declares the function used to configure the GPIO pins
 * required by the application, such as LED blinking or control signals.
 *
 * @author Ángel Soto Boullosa
 * @date 2025-06-22
 * @version 0.0.1
 */

#ifndef GPIO_H
#define GPIO_H

#include <driver/gpio.h>

/**
 * @brief Initializes GPIO output pins.
 *
 * Configures the GPIO pins defined by `BLINK_GPIO` and `CONTROL_GPIO`
 * as outputs with no pull-up or pull-down resistors and no interrupts.
 */
void init_gpio(void);

#endif // GPIO_H