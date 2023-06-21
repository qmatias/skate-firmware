#ifndef FIRMWARE_BLE_H
#define FIRMWARE_BLE_H

#include "imu.h"
#include <ArduinoBLE.h>

extern BLECharacteristic accCharacteristic;
extern BLECharacteristic gyroCharacteristic;

extern int connections;

void bleInitialize();
void bleTask();

#endif // FIRMWARE_BLE_H
