/**
 * @file time_sync.h
 * @brief Header for SNTP time synchronization.
 *
 * Declares the function responsible for synchronizing the system clock
 * using SNTP. This is typically used to ensure accurate timestamps
 * in ESP32-based applications.
 *
 * @author Ángel Soto Boullosa
 * @date 2025-06-22
 * @version 0.0.1
 */

#ifndef TIME_SYNC_H
#define TIME_SYNC_H

/**
 * @brief Synchronizes the system time using SNTP.
 *
 * Sets the system time by polling an NTP server until a valid
 * timestamp is obtained or a timeout occurs.
 */
void sync_time(void);

#endif // TIME_SYNC_H
