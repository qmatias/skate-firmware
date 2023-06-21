#ifndef FIRMWARE_LOGGING_H
#define FIRMWARE_LOGGING_H

#include <Arduino.h>

#define BUILD_DEBUG true

#ifdef BUILD_DEBUG
#   define logDebugLn(msg) Serial.println(msg)
#   define logDebug(msg) Serial.print(msg)
#else
#   define logDebugLn(msg) do { } while (0)
#   define logDebug(msg) do { } while (0)
#endif

#define logInfoLn(msg) Serial.println(msg)
#define logInfo(msg) Serial.print(msg)

#define logErrLn(msg) Serial.println(msg)
#define logErr(msg) Serial.print(msg)

#define fail(msg) do { while (true) logErrLn(msg); } while (0)

#endif // FIRMWARE_LOGGING_H