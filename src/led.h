#ifndef FIRMWARE_LED_H
#define FIRMWARE_LED_H

#include <Arduino.h>
#define BLE_LED_PIN LED_BUILTIN

void ledInitialize();
void ledTask();

#endif // FIRMWARE_LED_H
