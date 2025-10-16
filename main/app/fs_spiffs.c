// fs_spiffs.c
#include "esp_spiffs.h"
#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_err.h"
#include "esp_http_server.h"

static const char *TAG = "fs_spiffs";

static const char *FS_TAG = "fs_spiffs";

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

esp_err_t fs_spiffs_send_file(httpd_req_t *req, const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) {
        ESP_LOGE(FS_TAG, "open failed: %s", path);
        httpd_resp_set_status(req, "404 Not Found");
        const char *msg = "File not found\n";
        httpd_resp_send(req, msg, HTTPD_RESP_USE_STRLEN);
        return ESP_FAIL;
    }

    char buf[512];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), f)) > 0) {
        if (httpd_resp_send_chunk(req, buf, n) != ESP_OK) {
            ESP_LOGE(FS_TAG, "send_chunk failed");
            fclose(f);
            return ESP_FAIL;
        }
    }
    fclose(f);

    // end chunked response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}
