#pragma once
#include "esp_err.h"
#include "esp_http_server.h"  // for httpd_req_t

#ifdef __cplusplus
extern "C" {
#endif

// You already call this in app_main()
esp_err_t fs_spiffs_init(void);

/**
 * Stream a SPIFFS file to the HTTP response using chunked transfer.
 * The handler should usually set the Content-Type before calling this.
 *
 * @param req  httpd request
 * @param path absolute path in SPIFFS (e.g. "/spiffs/log.txt")
 * @return ESP_OK on success, ESP_FAIL on error (also sends an error response)
 */
esp_err_t fs_spiffs_send_file(httpd_req_t *req, const char *path);

#ifdef __cplusplus
}
#endif
