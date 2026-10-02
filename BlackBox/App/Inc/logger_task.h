#include "ring_buffer.h"
#include "crc16.h"
#include "fatfs.h"
#include "log_record.h"
#include "tasks.h"

static void maybe_sync(void);

bool validate_and_recover(const char *path);

void LoggerTask(void *argument);
