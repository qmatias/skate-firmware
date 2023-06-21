#include "timer.h"
#include <Arduino.h>

Timer::Timer(unsigned long interval)
    : mLast(0), mInterval(interval), mRunning(false) {}

bool Timer::start() {
    if (mRunning)
        return false;
    mLast = millis();
    mRunning = true;
    return true;
}

void Timer::stop() { mRunning = false; }

void Timer::restart() {
    mLast = millis();
    mRunning = true;
}

bool Timer::waiting() { return millis() - mLast < mInterval; }