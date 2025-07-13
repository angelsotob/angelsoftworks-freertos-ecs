/**
 * @file utils.c
 * @brief Implementation of utility functions for the ESP32 application.
 *
 * This source file provides helper functions used by various modules,
 * such as timestamp formatting.
 *
 * @author Ángel Soto Boullosa
 * @date 2025-06-22
 * @version 0.0.1
 */

#include "utils.h"

/**
 * @brief Retrieves the current system time as a formatted string.
 *
 * This function gets the local system time using `time()` and `localtime_r()`
 * and writes it into the provided buffer using the format "%Y-%m-%d %H:%M:%S".
 *
 * @param[out] buffer Pointer to the buffer where the formatted string will be written.
 * @param[in]  len    Size of the buffer.
 */
void get_timestamp_str(char *buffer, size_t len)
{
    time_t now = time(NULL);
    struct tm timeinfo;
    localtime_r(&now, &timeinfo); // Reentrant (thread-safe) version

    strftime(buffer, len, "%Y-%m-%d %H:%M:%S", &timeinfo);
}
