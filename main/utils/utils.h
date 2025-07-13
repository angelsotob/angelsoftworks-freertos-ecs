/**
 * @file utils.h
 * @brief Utility functions for time formatting and general-purpose helpers.
 *
 * This header declares general-purpose utility functions used throughout
 * the ESP32 FreeRTOS application.
 *
 * @author Ángel Soto Boullosa
 * @date 2025-06-22
 * @version 0.0.1
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <time.h>

/**
 * @brief Retrieves the current system time as a formatted string.
 *
 * This function gets the local system time and formats it as
 * a human-readable string (e.g., "2025-07-08 22:45:00").
 *
 * @param[out] buffer Pointer to the destination buffer.
 * @param[in] len     Length of the buffer in bytes.
 */
void get_timestamp_str(char *buffer, size_t len);

#endif // UTILS_H
