// fs_spiffs.c
#include "esp_spiffs.h"
#include "esp_log.h"
#include "esp_err.h"

static const char *TAG = "fs_spiffs";

esp_err_t fs_spiffs_init(void)
{
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,   // usa la partición "spiffs" del CSV
        .max_files = 5,
        .format_if_mount_failed = true
    };

    esp_err_t err = esp_vfs_spiffs_register(&conf);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "SPIFFS mount failed: %s", esp_err_to_name(err));
        return err;
    }

    size_t total = 0, used = 0;
    err = esp_spiffs_info(conf.partition_label, &total, &used);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "SPIFFS total=%u bytes, used=%u bytes", (unsigned)total, (unsigned)used);
    } else {
        ESP_LOGW(TAG, "esp_spiffs_info failed: %s", esp_err_to_name(err));
    }
    return ESP_OK;
}
