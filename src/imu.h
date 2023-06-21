#ifndef FIRMWARE_IMU_H
#define FIRMWARE_IMU_H

#include <Arduino_LSM6DSOX.h>

union SensorData {
    struct __attribute__((packed)) {
        float values[3];
        bool updated;
    };
    uint8_t bytes[3 * sizeof(float)];
};

extern union SensorData accData;
extern union SensorData gyroData;

void imuInitialize();
void imuTask();

#endif // FIRMWARE_IMU_H
