#pragma once

#include <Arduino.h>

static const char CMD_DEADZONE_P[] PROGMEM = "DEADZONE"; // "DEADZONE" | "DEADZONE x"    Reports or update and reports the global deadzone configuration.       // TODO - Implement Deadzone configuration

class TextHelper {
public:
    static void alignValue(const int value, const uint8_t width = 4);   // Align the value to the right with spaces
    static void alignedPrint(const int value, const uint8_t width = 4); // Align the value to the right with spaces
    static void printSeparator() { Serial.print(F(" | ")); }            // Print a separator between values

    static void printBooleanDescription(const bool value) {
        Serial.print(value ? F(" (ON)") : F(" (OFF)")); // Print "ON" or "OFF" based on the boolean value
    }

    static void printLeadingComma(int idx) {
        if (idx != 0) {
            Serial.print(F(", ")); // Print a comma if it's not the first sensor
        }
    }
};
