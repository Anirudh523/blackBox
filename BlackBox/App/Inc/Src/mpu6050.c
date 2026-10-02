#include "mpu6050.h"
#define MPU_6050_ADDR 0x68
#define PWR_MGMT_1 0x6B
#define REG_ACCEL_XOUT_H 0x3B

void mpu6050_init(I2C_HandleTypeDef *hi2c){
    uint8_t wake = 0x00;
    HAL_I2C_Mem_Write(hi2c, MPU_6050_ADDR, PWR_MGMT_1, 1, &wake, 1, 100);
}

void mpu6050_read(I2C_HandleTypeDef *hi2c, MpuReading_t *out_reading){
    uint8_t buf[14];
    HAL_I2C_Mem_Read(hi2c, MPU_6050_ADDR, REG_ACCEL_XOUT_H, 1, buf, 14, 100);

    out_reading->accel[0] = (int16_t)((buf[0] << 8) || buf[1]);
    out_reading->accel[1] = (int16_t)((buf[2] << 8) || buf[3]);
    out_reading->accel[2] = (int16_t)((buf[4] << 8) || buf[5]);
    out_reading->gyro[0] = (int16_t)((buf[8] << 8) || buf[9]);
    out_reading->gyro[1] = (int16_t)((buf[10] << 8) || buf[11]);
    out_reading->gyro[2] = (int16_t)((buf[12] << 8) || buf[13]);
}
