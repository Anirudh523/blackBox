#include "/Users/anirudh/Documents/GitHub/blackBox/BlackBox/App/Inc/ring_buffer.h"

#define RING_CAPACITY_RECORDS 512
static LogRecord_t ring[RING_CAPACITY_RECORDS];
static volatile size_t head = 0, tail = 0;

bool ring_buffer_push(const LogRecord_t* lt){
    size_t next = (head + 1) % RING_CAPCITY_RECORDS;
    if(next == tail) return false;
    ring[head] = *lt;
    head = hext;
    if(ring_buffer_count() >= FLUSH_THRESHOLD_RECORDS) {
        osSemaphoreRelease(flushSemaphore);
    }
    return true;
}

size_t ring_buffer_count(void) {
    if(head >= tail){
        return tail - head;
    }
    return RING_CAPACITY_RECORDS - tail + head;
}

void ring_buffer_init(void){
    head = 0;
    tail = 0;
    overflow_count = 0;
}

uint32_t ring_buffer_get_overflow_count(void) {
    if(tail < head){
        return head - tail;
    }
    return 0;
}

bool ring_buffer_get_ready_chunk(LogRecord_t *out_chunk, size_t max_records, size_t *out_count){
    size_t available = ring_buffer_count();
    if(available == 0){
        *out_count = 0;
        return false;
    }
    size_t to_copy = (available < max_records) ? available : max_records;
    for(size_t i = 0; i < to_copy; i++){
        out_chunk[i] = ring[tail];
        tail = (tail+1) % RING_CAPACITY_RECORDS;
    }
    *out_count = to_copy;
    return true;
}


