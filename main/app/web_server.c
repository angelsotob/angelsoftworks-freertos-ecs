#include <stdio.h>
#include <string.h>
#include <sys/param.h>

#include "esp_log.h"
#include "esp_timer.h"
#include "esp_chip_info.h"
#include "esp_system.h"
#include "esp_http_server.h"
#include "esp_idf_version.h"
#include "esp_netif.h"
#include "lwip/inet.h"

#include "data_snapshot.h"
#include "fs_spiffs.h"

static const char *TAG = "web";

/* ----------------------------- Helpers ----------------------------- */

static void append_kv(char *buf, size_t cap, const char *k, const char *v, bool comma)
{
    snprintf(buf + strlen(buf), cap - strlen(buf),
             "\"%s\":\"%s\"%s", k, v, comma ? "," : "");
}

static void ip_to_str(char *out, size_t cap)
{
    out[0] = 0;
    esp_netif_t *sta = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (!sta) {
        // try default
        sta = esp_netif_get_default_netif();
    }
    if (sta) {
        esp_netif_ip_info_t ip;
        if (esp_netif_get_ip_info(sta, &ip) == ESP_OK) {
            ip4addr_ntoa_r((const ip4_addr_t *)&ip.ip, out, cap);
        }
    }
    if (out[0] == 0) strlcpy(out, "0.0.0.0", cap);
}

/* ----------------------------- Handlers ---------------------------- */

static esp_err_t root_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
    return fs_spiffs_send_file(req, "/spiffs/index.html");
}

static esp_err_t health_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/json");
    return httpd_resp_sendstr(req, "{\"ok\":true}");
}

static esp_err_t info_get_handler(httpd_req_t *req)
{
    char json[384]; json[0] = 0;
    snprintf(json, sizeof(json), "{");

    // Chip & cores
    esp_chip_info_t chip; esp_chip_info(&chip);
    char cores_str[8]; snprintf(cores_str, sizeof(cores_str), "%d", chip.cores);

    // Project name (menuconfig) with fallback
#ifdef CONFIG_PROJECT_NAME
    const char *project = CONFIG_PROJECT_NAME;
#else
    const char *project = "angelsoftworks-freertos-ecs";
#endif

    // IDF version
    const char *idf = esp_get_idf_version();

    // IP (STA)
    char ip[16]; ip_to_str(ip, sizeof(ip));

    // Heap
    char heap_str[16];
    snprintf(heap_str, sizeof(heap_str), "%u", (unsigned)esp_get_free_heap_size());

    // Uptime (ms)
    char up_ms[32];
    snprintf(up_ms, sizeof(up_ms), "%llu", (unsigned long long)(esp_timer_get_time()/1000ULL));

    append_kv(json, sizeof(json), "project", project, true);
    append_kv(json, sizeof(json), "idf", idf, true);
    append_kv(json, sizeof(json), "ip", ip, true);
    append_kv(json, sizeof(json), "cores", cores_str, true);
    append_kv(json, sizeof(json), "heap_free", heap_str, true);
    append_kv(json, sizeof(json), "uptime_ms", up_ms, false);

    strlcat(json, "}", sizeof(json));

    httpd_resp_set_type(req, "application/json");
    return httpd_resp_send(req, json, HTTPD_RESP_USE_STRLEN);
}

static esp_err_t log_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/plain");
    return fs_spiffs_send_file(req, "/spiffs/log.txt");
}

static esp_err_t log_clear_post_handler(httpd_req_t *req)
{
    FILE *f = fopen("/spiffs/log.txt", "w");
    if (f) fclose(f);
    httpd_resp_set_type(req, "application/json");
    return httpd_resp_sendstr(req, "{\"cleared\":true}");
}

static esp_err_t data_get_handler(httpd_req_t *req)
{
    data_snapshot_t s; data_snapshot_get(&s);

    char json[256];
    int n = snprintf(json, sizeof(json),
        "{"
        "\"temperature_c\":%.3f,"
        "\"humidity_pct\":%.3f,"
        "\"pressure_hpa\":%.3f,"
        "\"alarm_on\":%s,"
        "\"updated_ms\":%llu"
        "}",
        (double)s.temperature_c,
        (double)s.humidity_pct,
        (double)s.pressure_hpa,
        s.alarm_on ? "true" : "false",
        (unsigned long long)(s.updated_us/1000ULL));

    (void)n;
    httpd_resp_set_type(req, "application/json");
    return httpd_resp_send(req, json, HTTPD_RESP_USE_STRLEN);
}

/* ------------------------------- URIs ------------------------------ */

static const httpd_uri_t root_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = root_get_handler,
};
static const httpd_uri_t health_uri = {
    .uri       = "/api/health",
    .method    = HTTP_GET,
    .handler   = health_get_handler,
};
static const httpd_uri_t info_uri = {
    .uri       = "/api/info",
    .method    = HTTP_GET,
    .handler   = info_get_handler,
};
static const httpd_uri_t log_uri = {
    .uri       = "/api/log",
    .method    = HTTP_GET,
    .handler   = log_get_handler,
};
static const httpd_uri_t log_clear_uri = {
    .uri       = "/api/log/clear",
    .method    = HTTP_POST,
    .handler   = log_clear_post_handler,
};
static const httpd_uri_t data_uri = {
    .uri       = "/api/data",
    .method    = HTTP_GET,
    .handler   = data_get_handler,
};

/* ----------------------------- Lifecycle --------------------------- */

static httpd_handle_t s_server = NULL;

esp_err_t web_server_start(void)
{
    data_snapshot_init(); // ensure snapshot ready

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.uri_match_fn = httpd_uri_match_wildcard;

    esp_err_t err = httpd_start(&s_server, &config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "httpd_start failed: %s", esp_err_to_name(err));
        return err;
    }
    httpd_register_uri_handler(s_server, &root_uri);
    httpd_register_uri_handler(s_server, &health_uri);
    httpd_register_uri_handler(s_server, &info_uri);
    httpd_register_uri_handler(s_server, &log_uri);
    httpd_register_uri_handler(s_server, &log_clear_uri);
    httpd_register_uri_handler(s_server, &data_uri);
    ESP_LOGI(TAG, "Web server started");
    return ESP_OK;
}

void web_server_stop(void)
{
    if (s_server) {
        httpd_stop(s_server);
        s_server = NULL;
    }
}
