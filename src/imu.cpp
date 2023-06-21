#include "logging.h"
#include "imu.h"
#include "ble.h"

const unsigned long ACC_UPDATE_INTERVAL = 0;
const unsigned long GYRO_UPDATE_INTERVAL = 0;

union SensorData accData;
union SensorData gyroData;

void clearData() {
    logInfoLn("Clearing IMU data...");

    for (int i = 0; i < 3; i++) {
        accData.values[i] = 0.00;
        gyroData.values[i] = 0.00;
    }

    accData.updated = false;
    gyroData.updated = false;
}

void imuInitialize() {
    logInfoLn("Initializing IMU...");
    if (!IMU.begin())
        fail("Failed to initialize IMU!");

    Wire.setClock(400000); // Increase I2C clock speed to 400kHz

    logInfoLn("IMU connected.");
}

bool accSensorTask() {
    static unsigned long prev_acc = 0;
    unsigned long now = millis();
    if (now - prev_acc < ACC_UPDATE_INTERVAL)
        return false;

    logDebugLn("Checking acceleration availability...");
    if (!IMU.accelerationAvailable())
        return false;

    prev_acc = now;

    logDebugLn("Reading acceleration...");
    float x, y, z;
    if (!IMU.readAcceleration(x, y, z)) {
        logErrLn("Failed to read acceleration!");
        return false;
    }

    logDebugLn("Acceleration read, writing to accData...");
    accData.values[0] = x;
    accData.values[1] = y;
    accData.values[2] = z;
    accData.updated = true;

    return true;
}

bool gyroSensorTask() {
    static unsigned long prev_gyro = 0;
    unsigned long now = millis();
    if (now - prev_gyro < GYRO_UPDATE_INTERVAL)
        return false;

    logDebugLn("Checking gyroscope availability...");
    if (!IMU.gyroscopeAvailable())
        return false;

    prev_gyro = now;

    logDebugLn("Reading gyroscope...");
    float x, y, z;
    if (!IMU.readGyroscope(x, y, z)) {
        logErrLn("Failed to read gyroscope!");
        return false;
    }

    logDebugLn("Gyroscope read, writing to gyroData...");
    gyroData.values[0] = x;
    gyroData.values[1] = y;
    gyroData.values[2] = z;
    gyroData.updated = true;

    return true;
}

void printAccSensorData() {
    logDebug("Acc: ");
    logDebug(accData.values[0]);
    logDebug(", ");
    logDebug(accData.values[1]);
    logDebug(", ");
    logDebugLn(accData.values[2]);

    logDebug("Acc Subscription status: ");
    logDebugLn(accCharacteristic.subscribed());
}

void printGyroSensorData() {
    logDebug("Gyro: ");
    logDebug(gyroData.values[0]);
    logDebug(", ");
    logDebug(gyroData.values[1]);
    logDebug(", ");
    logDebugLn(gyroData.values[2]);

    logDebug("Gyro Subscription status: ");
    logDebugLn(gyroCharacteristic.subscribed());
}

void imuTask() {
    logDebugLn("Running IMU task...");
    logDebugLn("IMU task: accSensorTask");
    if (accSensorTask())
        printAccSensorData();
    logDebugLn("IMU task: gyroSensorTask");
    if (gyroSensorTask())
        printGyroSensorData();
}
