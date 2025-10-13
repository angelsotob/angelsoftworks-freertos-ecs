/**
 * @file gpio.c
 * @brief GPIO configuration implementation for the application.
 *
 * Sets up output pins used for LED blinking and control signals.
 *
 * @author Ángel Soto Boullosa
 * @date 2025-06-22
 * @version 0.0.1
 */

#include "gpio.h"
#include "../include/config.h"

/**
 * @brief Initializes the GPIO pins used for LED indication and control output.
 *
 * This function configures the GPIOs defined by `BLINK_GPIO` and `CONTROL_GPIO`
 * as standard digital outputs. Both pins are initialized with:
 * - No internal pull-up or pull-down resistors.
 * - Output mode enabled.
 * - Interrupts disabled.
 *
 * Typical usage:
 * - `BLINK_GPIO`: status LED or heartbeat indicator.
 * - `CONTROL_GPIO`: alarm output or system control pin.
 *
 * @note The specific GPIO numbers must be defined elsewhere in the project
 *       (e.g., in `config.h` or `main.h`).
 *
 * @return void
 */
void init_gpio(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = ((1ULL << BLINK_GPIO) | (1ULL << CONTROL_GPIO)),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE};

    gpio_config(&io_conf);
}
