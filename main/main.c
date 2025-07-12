/**
 * @file main.c
 * @brief Main source file for the FreeRTOS blink example.
 *
 * This application demonstrates a simple FreeRTOS task that toggles
 * an LED connected to the specified GPIO pin.
 *
 * @author Ángel Soto Boullosa
 * @date 2025-06-22
 * @version 0.0.1
 */

#include "../include/main.h"

extern void wifi_init_sta(void);

static const char *TAG = "sensor";

void init_gpio()
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BLINK_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE};
    gpio_config(&io_conf);
}
/**
 * @brief Task that blinks the LED on and off.
 *
 * @param pvParameter Unused parameter.
 */
void blink_task(void *pvParameter)
{
    (void)pvParameter; // To avoid unused parameter warning

    while (1)
    {
        gpio_set_level(BLINK_GPIO, GPIO_HIGH);
        vTaskDelay(pdMS_TO_TICKS(HIGH_TIME_MS));
        gpio_set_level(BLINK_GPIO, GPIO_LOW);
        vTaskDelay(pdMS_TO_TICKS(LOW_TIME_MS));
    }
}

float get_fake_temperature()
{
    return (float)(20.0 + (rand() % 1000) / 100.0f); // Entre 20.00 y 29.99 ºC
}

float get_fake_humidity()
{
    return (float)(40.0 + (rand() % 600) / 10.0f); // Entre 40.0 y 99.9 %
}

void get_timestamp_str(char *buffer, size_t len)
{
    time_t now = time(NULL);
    struct tm timeinfo;
    localtime_r(&now, &timeinfo); // R para ser thread-safe

    strftime(buffer, len, "%Y-%m-%d %H:%M:%S", &timeinfo);
}

/**
 * @brief Task that gathers sensor information.
 *
 * @param pvParameter Unused parameter.
 */
void sensor_task(void *pvParameter)
{
    (void)pvParameter;

    char timestamp_str[20];

    while (1)
    {
        float temperature = get_fake_temperature();
        float humidity = get_fake_humidity();
        get_timestamp_str(timestamp_str, sizeof(timestamp_str));

        printf("%s: Temperature sensor measures: %.2f ºC\n", timestamp_str, temperature);
        printf("%s: Humidity sensor measures: %.1f %%HR\n", timestamp_str, humidity);
        ESP_LOGI(TAG, "[%s] Temperature: %.2f ºC | Humidity: %.1f %%", timestamp_str, temperature, humidity);

        vTaskDelay(pdMS_TO_TICKS(MS_100));
    }
}

/**
 * @brief Main application entry point.
 */
void app_main(void)
{
    srand((unsigned)time(NULL));
    init_gpio();

    wifi_init_sta();

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

        if (timeinfo.tm_year >= (2023 - 1900))
        {
            break;
        }

        retry++;
        ESP_LOGI("sntp", "Waiting for system time to be set... (%d)", retry);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }

    if (xTaskCreate(&blink_task, "blink_task", 1024, NULL, 5, NULL) != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to create blink_task");
    }

    if (xTaskCreate(&sensor_task, "sensor_task", 2048, NULL, 5, NULL) != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to create sensor_task");
    }
}