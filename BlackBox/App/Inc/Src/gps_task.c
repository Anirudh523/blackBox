#include "gps_tasks.h"
#include "log_record.h"
#include "gps_parser.h"

osMessageQueueId_t gpsRawQueue;

void GpsTask(void *arguement) {
    const osMessageQueueAttr_t queue_attr = { .name = "gpsRawQueue"};
    gspRawQueue = osMessageQueueNew(4, GPS_BUF_SIZE, &queue_attr);

    gps_start();

    uint8_t raw[GPS_BUF_SIZE];
    for(;;) {
        if(osMessageQueueGet(gpsRawQueue, raw, NULL, osWaitForever) == osOK){
            GPSFix_t fix;
            if(gps_parse_sentence(raw, strlen((char *)raw), &fix) && fix.fix_valid) {
                LogRecord_t rec = {0};
                rec.timestamp_ms = HAL_GetTick();
                rec.lat_fixed = fix.lat_fixed;
                rec.lon_fixed = fix.lon_fixed;

                osMutexAcquire(ringBufMutex, osWaitForever);
                bool ok = ring_buffer_push(&rec);
                size_t count = ring_buffer_count();
                osMutexRelease(ringBufMutex);

                if(ok && count >= FLUSH_THRESHOLD_RECORDS) {
                    osSemaphoreRelease(flushSemaphore);
                }
            }
        }
    }
}