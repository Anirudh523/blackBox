typedef struct {
    int32_t accel[3],
    int32_t gyro[3]
} MpuReading_t;

void mpu6050_init(I2C_HandleTypeDef *hi2c);
void mpu6050_read(I2C_HandleTypeDef *hi2c, MpuReading_t* output);