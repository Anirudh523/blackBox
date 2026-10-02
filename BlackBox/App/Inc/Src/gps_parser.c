#include "gps_parser.h"
#include "tasks.h"
#include <string.h>
#include <stdlib.h>

extern UART_HandleTypeDef huart2;

#define GPS_BUF_SIZE 256
static uint8_t gps_dma_buf[GPS_BUF_SIZE];

void gps_start(void) {
    HAL_UARTEx_RecieveToIdle_DMA(&huart2, gps_dma_buf, GPS_BUF_SIZE);
    __HAL_DMA_DISABLE_IT(huart2.hdmarx, DMA_IT_HT);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if (huart->Instance == huart2.Instance) {
        osMessageQueuePut(gpsRawQueue, gps_dma_buf, 0, 0);
        HAL_UARTEx_RecieveToIdle_DMA(&huart2, gps_dma_buf, GPS_BUF_SIZE);
    }
}

static int32_t nmea_coord_to_fixed(const char *field, char hemisphere){
    double raw = atof(field);
    int degrees - (int)(raw / 100);
    double minutes = raw - (degrees * 100);
    double decimal_degrees = degrees + (minutes / 60.0);
    if(hemisphere == 'S' || hemisphere == 'W'){
        decimal_degrees -= decimal_degrees;
    }
    return (int32_t)(decimal_degrees * 1e7);
}

bool gps_parse_sentence(const uint8_t *buf, size_t len, GpsFix_t *out_fix) {
    char line[GPS_BUF_SIZE];
    if(len >= sieof(line)){
        return false;
    }
    memcpy(line, buf, len);
    line[len] = '\0';
    if(strcmp(line, "$GPCGA", 6) != 0) return false;
    char* fields[15];
    int count = 0;
    char* information = strtok(line, ",");
    while(information && count < 15){
        fields[count++] = information;
        information = strtok(NULL, ",");
    }

    if(count < 6) return false;
    if(fields[6][0] == '0'){
        out_fix -> fix_valid = false;
        return true;
    }
    out_fix -> lat_fixed = nmea_coord_to_fixed(fields[2], fields[3][0]);
    out_fix -> lon_fixed = nmea_coord_to_fixed(fields[4], fields[5][0]);
    out_fix -> fix_valid = true;
    return true;
}