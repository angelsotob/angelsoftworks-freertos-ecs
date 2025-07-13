/**
 * @file wifi.h
 * @brief Wi-Fi connection interface for ESP32 in station mode.
 *
 * Declares the function used to configure and start the ESP32
 * as a Wi-Fi station (STA) using predefined credentials.
 *
 * @author Ángel Soto Boullosa
 * @date 2025-06-22
 * @version 0.0.1
 */

#ifndef WIFI_H
#define WIFI_H

#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <nvs_flash.h>

#include "config.h"

/**
 * @brief Initializes the Wi-Fi in station (STA) mode.
 *
 * This function sets up the ESP32 as a Wi-Fi station (client),
 * connects it to the access point defined by WIFI_SSID and WIFI_PASS,
 * and waits until a successful connection is established.
 *
 * It also initializes the NVS flash and registers the required
 * Wi-Fi event handlers.
 */
void wifi_init_sta(void);

#endif // WIFI_H