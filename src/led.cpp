#include "led.h"
#include "ble.h"
#include "timer.h"
#include "logging.h"

Timer statusTimer = Timer(1500);
Timer transitionInTimer = Timer(500);
Timer transitionOutTimer = Timer(500);
Timer blinkTimer = Timer(165);

void ledOn() { digitalWrite(BLE_LED_PIN, HIGH); }

void ledOff() { digitalWrite(BLE_LED_PIN, LOW); }

void ledToggle() {
    digitalWrite(BLE_LED_PIN, !digitalRead(BLE_LED_PIN));
}

void ledInitialize() {
    logInfoLn("Initializing LED...");

    pinMode(BLE_LED_PIN, OUTPUT);
    ledOff();
}

void ledTask() {
    logDebugLn("Running LED Task");
    static int blinkCount = 0;

    statusTimer.start();
    if (statusTimer.waiting())
        return;

    // turn off and wait for transition...
    if (transitionInTimer.start())
        ledOff();
    if (transitionInTimer.waiting())
        return;

    // blink on and off once for each connection
    if (blinkTimer.start())
        blinkCount = connections * 2;
    if (blinkTimer.waiting())
        return;
    if (blinkCount) {
        blinkCount--;
        ledToggle();
        blinkTimer.restart();
        return;
    }

    // transition back and start over
    transitionOutTimer.start();
    if (transitionOutTimer.waiting())
        return;

    // cleanup
    ledOn();
    blinkTimer.stop();
    transitionOutTimer.stop();
    transitionInTimer.stop();
    statusTimer.stop();
}