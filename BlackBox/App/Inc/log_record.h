#ifndef LOG_RECORD_H
#define LOG_RECORD_H

#include <stdint.h>

typedef struct __attribute__((packed)) {
    uint32_t timestamp_ms;
    int16_t accel[3];
    int16_t gyro[3];
    int32_t lat_fixed;
    int32_t lon_fixed;
    uint16_t crc16;
} LogRecord_t;

#endif