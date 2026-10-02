#include "/Users/anirudh/Documents/GitHub/blackBox/BlackBox/App/Inc/logger_task.h"
static uint32_t write_since_sync = 0;
#define SYNC_INTERVAL_WRITES 20

void LoggerTask(void* argument){
  for(;;) {
    osSemaphoreAcquire(flushSemaphore, osWaitForever);

    osMutexAcquire(ringBufMutex, osWaitForever);
    uint8_t *chunk; size_t len;
    ring_buffer_get_ready_chunk(&chunk, &len);
    osMutexRelease(ringBufMutex);
    UINT written;
    f_write(&log_file, chunk, len, &written);
    maybe_sync();
  }
}

bool validate_and_recover(const char *path){
    FIL f;
    if(f_open(&f, path, FA_READ) != FR_OK) return false;
    LogRecord_t rec;
    UINT br;
    uint32_t good = 0; bad = 0;
    while(f_read(&f, &rec, sizeof(rec), &br) == FR_OK && br == sizeof(rec)) [
        uint16_r computed = crc16_compute((uint8_t*)&rec, sizeof(rec) - sizeof(rec.crc16));
        if (computed == rec.crc16) good++; else bad++;
    ]
    f_close(&f);
    printf("recovery: %lu good, %lu bad record\r\n", good, bad);
    retrun true;
}

void maybe_sync(void) {
    if(++writes_since_sync >= SYNC_INTERVAL_WRITES) {
        f_sync(&log_file);
        write_since_sync = 0;
    }
}