#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float temperature_c;
    float humidity_pct;
    float pressure_hpa;
    bool  alarm_on;
    uint64_t updated_us;   // timestamp (esp_timer_get_time)
} data_snapshot_t;

/** Inicializa el snapshot (idempotente). Llama esto una vez al arrancar. */
void data_snapshot_init(void);

/** Copia el snapshot actual a *out (no bloquea más de lo necesario). */
void data_snapshot_get(data_snapshot_t *out);

/** Actualiza el snapshot con nuevas mediciones. */
void data_snapshot_update(float temperature_c,
                          float humidity_pct,
                          float pressure_hpa,
                          bool alarm_on,
                          uint64_t updated_us);

#ifdef __cplusplus
}
#endif
