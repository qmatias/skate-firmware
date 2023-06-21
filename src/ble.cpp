#include "ble.h"
#include "led.h"
#include "logging.h"
#include "constants.h"

const uint32_t BLE_UPDATE_INTERVAL = 10;

#if RIGHT_SKATE
#   define BLE_SERVICE_ID "56854577-0ff2-11ee-be56-0242ac120002"
#   define BLE_DEVICE_NAME "ICEQ - Right Skate"
#   define BLE_LOCAL_NAME "ICEQ - Right Skate"
#else
#   define BLE_SERVICE_ID "56854555-0ff2-11ee-be56-0242ac120002"
#   define BLE_DEVICE_NAME "ICEQ - Left Skate"
#   define BLE_LOCAL_NAME "ICEQ - Left Skate"
#endif

#define BLE_ACC_CHAR_ID "56854522-f7a0-4046-ab6d-cbbf0b154811"
#define BLE_GYRO_CHAR_ID "56854549-827b-454c-aa00-463ed3ae7bd4"

BLEService bleService(BLE_SERVICE_ID);
BLECharacteristic accCharacteristic(BLE_ACC_CHAR_ID, BLERead | BLENotify,
                                    sizeof accData.bytes);
BLECharacteristic gyroCharacteristic(BLE_GYRO_CHAR_ID, BLERead | BLENotify,
                                     sizeof gyroData.bytes);

int connections = 0;

void onConnect(BLEDevice central) {
    logInfo("BLE connected to central: ");
    logInfoLn(central.address());
    connections++;
}

void onDisconnect(BLEDevice central) {
    logInfo("BLE disconnected from central: ");
    logInfoLn(central.address());
    connections--;
}

void bleInitialize() {
    logInfoLn("Initializing BLE...");
    if (!BLE.begin())
        fail("Failed to initialize BLE!");

    // set device name and service UUID
    BLE.setDeviceName(BLE_DEVICE_NAME);
    BLE.setLocalName(BLE_LOCAL_NAME);
    BLE.setAdvertisedService(bleService);

    // add characteristics
    bleService.addCharacteristic(accCharacteristic);
    bleService.addCharacteristic(gyroCharacteristic);

    // add service
    BLE.addService(bleService);

    // add event handlers
    BLE.setEventHandler(BLEConnected, onConnect);
    BLE.setEventHandler(BLEDisconnected, onDisconnect);

    // start advertising
    BLE.advertise();

    logInfoLn("BLE connected.");
}

void blePoll() {
    static uint32_t previousMillis = 0;
    uint32_t currentMillis = millis();
    if (currentMillis - previousMillis >= BLE_UPDATE_INTERVAL) {
        previousMillis = currentMillis;
        BLE.poll();
    }
}

void writeData() {
    if (!connections) return;

    if (accData.updated) {
        accCharacteristic.writeValue(accData.bytes, sizeof accData.bytes);
        accData.updated = false;
    }

    if (gyroData.updated) {
        gyroCharacteristic.writeValue(gyroData.bytes, sizeof gyroData.bytes);
        gyroData.updated = false;
    }
}

void bleTask() {
    logDebugLn("Running BLE task...");
    blePoll();
    writeData();
}