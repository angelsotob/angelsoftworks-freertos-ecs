/**
 * @file time_sync.c
 * @brief SNTP-based system time synchronization for ESP32.
 *
 * This module configures the SNTP client to synchronize the ESP32's
 * system clock with an internet time server. It retries up to 10 times
 * if the time is not yet set, with a delay of 2 seconds between attempts.
 *
 * @author Ángel Soto Boullosa
 * @date 2025-06-22
 * @version 0.0.1
 */

#include "time_sync.h"
#include <esp_log.h>
#include <esp_sntp.h>
#include <time.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

/**
 * @brief Synchronizes the system time using SNTP.
 *
 * This function configures and starts the SNTP client to obtain the current
 * time from an NTP server. It retries the synchronization up to 10 times,
 * waiting 2 seconds between each attempt. Once the time is set (tm_year >= 2023),
 * the function exits.
 */
void sync_time(void)
{
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "pool.ntp.org");
    esp_sntp_init();

    time_t now = 0;
    struct tm timeinfo = {0};
    int retry = 0;
    const int retry_count = 10;

    while (retry < retry_count)
    {
        time(&now);
        localtime_r(&now, &timeinfo);

        if (timeinfo.tm_year >= (2023 - 1900)) {
            break;
        }

        ESP_LOGI("sntp", "Waiting for system time to be set... (%d)", ++retry);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
