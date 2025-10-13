// log_to_file.c
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include "esp_log.h"
#include "esp_err.h"
#include "esp_vfs.h"

#define LOG_PATH        "/spiffs/log.txt"
#define LOG_PATH_BAK    "/spiffs/log.1"
#define LOG_MAX_BYTES   (200 * 1024)   // ~200 KB por fichero

static int (*s_prev_vprintf)(const char *, va_list) = NULL;

static void rotate_if_needed(void)
{
    struct stat st;
    if (stat(LOG_PATH, &st) == 0 && st.st_size >= LOG_MAX_BYTES) {
        // cierra/rota: renombra log.txt -> log.1 (borra anterior si existe)
        remove(LOG_PATH_BAK);
        rename(LOG_PATH, LOG_PATH_BAK);
    }
}

static void append_to_file(const char *buf, size_t len)
{
    rotate_if_needed();
    FILE *f = fopen(LOG_PATH, "a");
    if (!f) return;
    (void)fwrite(buf, 1, len, f);
    fclose(f);
}

// Nuestro vprintf “duplica” a UART (previo) y a fichero
static int file_tee_vprintf(const char *fmt, va_list ap)
{
    // 1) manda a salida original (UART)
    int ret_uart = 0;
    if (s_prev_vprintf) {
        // Tenemos que copiar ap porque vsnprintf/vprintf consumen la lista
        va_list ap_copy;
        va_copy(ap_copy, ap);
        ret_uart = s_prev_vprintf(fmt, ap_copy);
        va_end(ap_copy);
    }

    // 2) formatea a buffer y escribe a fichero
    char buf[512];
    va_list ap2;
    va_copy(ap2, ap);
    int n = vsnprintf(buf, sizeof(buf), fmt, ap2);
    va_end(ap2);

    if (n > 0) {
        if ((size_t)n < sizeof(buf)) {
            append_to_file(buf, (size_t)n);
        } else {
            // línea más larga que el buffer: reservar temporalmente
            char *big = (char *)malloc(n + 1);
            if (big) {
                va_list ap3;
                va_copy(ap3, ap);
                vsnprintf(big, n + 1, fmt, ap3);
                va_end(ap3);
                append_to_file(big, (size_t)n);
                free(big);
            }
        }
    }
    return ret_uart;
}

void log_to_file_init(void)
{
    // guarda el vprintf actual (UART) y enchufa el nuestro
    s_prev_vprintf = esp_log_set_vprintf(file_tee_vprintf);
    ESP_LOGI("log_to_file", "File logging enabled: %s (max %d bytes, rotated to %s)",
             LOG_PATH, (int)LOG_MAX_BYTES, LOG_PATH_BAK);
}
