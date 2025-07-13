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

void init_gpio(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = ((1ULL << BLINK_GPIO) | (1ULL << CONTROL_GPIO)),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&io_conf);
}
