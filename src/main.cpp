#include <Arduino.h>

#include "ble.h"
#include "imu.h"
#include "led.h"
#include "logging.h"

void setup() {
    Serial.begin(9600);
    logInfoLn("Serial initialized.");

    ledInitialize();
    imuInitialize();
    bleInitialize();
}

void loop() {
    bleTask();
    imuTask();
    ledTask();
}