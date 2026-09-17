#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "log_record.h"

#define RING_CAPCITY_RECORDS 512
#define FLUSH_THRESHOLD_RECORDS 128

void ring_buffer_init(void);
size_t ring_buffer_count(void);
bool ring_buffer_push(const LogRecord_t* lt);
bool ring_buffer_get_ready_chunk(LogRecord_t *out_chunk, size_t max_records, size_t *out_count);
uint32_t ring_buffer_get_overflow_count(void);

#endif


