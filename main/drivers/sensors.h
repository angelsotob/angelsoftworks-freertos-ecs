/**
 * @file sensors.h
 * @brief Simulated sensor driver header.
 *
 * This module provides declarations for fake temperature and humidity
 * readings, useful for development and testing without real hardware.
 *
 *
 * @author Ángel Soto Boullosa
 * @date 2025-06-22
 * @version 0.0.1
 */

#ifndef SENSORS_H
#define SENSORS_H

#include <stdlib.h>

/**
 * @brief Generates a fake temperature value.
 *
 * @return Simulated temperature in degrees Celsius between 20.00 and 29.99.
 */
float get_fake_temperature(void);

/**
 * @brief Generates a fake humidity value.
 *
 * @return Simulated relative humidity between 40.0% and 99.9%.
 */
float get_fake_humidity(void);

#endif // SENSORS_H
