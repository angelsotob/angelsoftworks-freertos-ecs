/**
 * @file sensors.c
 * @brief Simulated sensor driver implementation.
 *
 * Implements functions that return random values emulating
 * temperature and humidity sensors for test environments.
 *
 * @author Ángel Soto Boullosa
 * @date 2025-06-22
 * @version 0.0.1
 */

#include "sensors.h"

/**
 * @brief Generates a fake temperature value.
 *
 * @return Simulated temperature in degrees Celsius between 20.00 and 29.99.
 */
float get_fake_temperature(void)
{
    return 20.0f + (rand() % 1000) / 100.0f;
}

/**
 * @brief Generates a fake humidity value.
 *
 * @return Simulated relative humidity between 40.0% and 99.9%.
 */
float get_fake_humidity(void)
{
    return 40.0f + (rand() % 600) / 10.0f;
}