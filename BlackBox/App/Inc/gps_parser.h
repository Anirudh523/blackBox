#ifndef GPS_PARSER_H
#define GPS_PARSER_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

osMessageQueueId_t gpsRawQueue;
typedef struct {
    int32_t lat_fixed;
    int32_t lon_fixed;
    bool fix_valid;
} GpsFix_t;

void gps_start(void);
bool gps_parse_sentence(const uint8_t *buf, size_t len, GpsFix_t *out_fix);

#endif