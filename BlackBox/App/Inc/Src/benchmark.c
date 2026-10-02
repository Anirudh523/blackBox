#include "benchmark.h"
#include "fatfs.h"
#include "cmsis_os2.h"
#include <string.h>
#include <stdio.h>

extern FIL bench_file;

void run_benchmark(size_t block_size, size_t total_bytes) {
    uint8_t *dummy = pvPortMalloc(block_size);
    memset(dummy, 0xAA, block_size);

    uint32_t start = HAL_GetTick();
    UINT written;
    size_t remaining 
}