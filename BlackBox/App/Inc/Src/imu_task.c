#include "mpu6050.h"
#include "log_record.h"

void ImuTask(void *argument){
    mpu6050_init(&hi2cl);
    
    for(;;){
        LogRecord_t rec;
        MpuReading_t reading;
        mpu6050_read(&hi2cl, &reading);
        rec.timestamp_ms = HAL_GetTick();
        memcpy(rec.accel, reading.accel, sizeof(rec.accel));
        memcpy(rec.gyro, reading.gyro, sizeof(rec.gyro));
        osMutexAcquire(ringBufMutex, osWaitForever);
        bool ok = ring_buffer_push(&rec);
        size_t count = ring_buffer_count();
        osMutexRelease(ringBufMutex);

        if(ok && count >= FLUSH_THRESHOLD_RECORDS){
            osSemaphoreRelease(flushSemaphore);
        }
        
        osDelay(10);
    }
}