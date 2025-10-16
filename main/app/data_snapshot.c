#include "data_snapshot.h"
#include "freertos/semphr.h"
#include "esp_timer.h"

static data_snapshot_t g_snap = {
    .temperature_c = 0,
    .humidity_pct  = 0,
    .pressure_hpa  = 0,
    .alarm_on      = false,
    .updated_us    = 0,
};
static StaticSemaphore_t g_mux_buf;
static SemaphoreHandle_t g_mux = NULL;

void data_snapshot_init(void)
{
    if (g_mux == NULL) {
        g_mux = xSemaphoreCreateMutexStatic(&g_mux_buf);
    }
}

void data_snapshot_get(data_snapshot_t *out)
{
    if (!out) return;
    if (g_mux && xSemaphoreTake(g_mux, pdMS_TO_TICKS(50)) == pdTRUE) {
        *out = g_snap;
        xSemaphoreGive(g_mux);
    } else {
        // Si no se pudo tomar el mutex, devolvemos lo que haya (no bloqueante).
        *out = g_snap;
    }
}

void data_snapshot_update(float temperature_c,
                          float humidity_pct,
                          float pressure_hpa,
                          bool alarm_on,
                          uint64_t updated_us)
{
    if (!g_mux) data_snapshot_init();
    if (g_mux && xSemaphoreTake(g_mux, portMAX_DELAY) == pdTRUE) {
        g_snap.temperature_c = temperature_c;
        g_snap.humidity_pct  = humidity_pct;
        g_snap.pressure_hpa  = pressure_hpa;
        g_snap.alarm_on      = alarm_on;
        g_snap.updated_us    = updated_us ? updated_us : (uint64_t)esp_timer_get_time();
        xSemaphoreGive(g_mux);
    }
}
