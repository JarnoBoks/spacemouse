// File for calibration specific functions

#include <Arduino.h>
#include "calibration.h"
#include "kinematics.h"
#include "config.h"
#include "hardware/SpaceMouseHW.h"

/// @brief Hold characters to plot them
char debugOutputBuffer[20];

char const *velNames[] = {"TX:", "TY:", "TZ:", "RX:", "RY:", "RZ:"}; // 6

void debugOutput1(SpaceMouseHW_ &SMHW, int *keyVals) {
    if (isDebugOutputDue()) {
        SMHW.PrintRawReads();
        // Report back 0-1023 raw ADC 10-bit values if enabled
        for (int i = 0; i < NUMKEYS; i++) {
            Serial.print("K");
            Serial.print(i);
            Serial.print(":");
            Serial.print(keyVals[i]);
            Serial.print(", ");
        }
        Serial.print(DEBUG_LINE_END);
    }
}

void debugOutput2(SpaceMouseHW_ &SMHW) {
    if (isDebugOutputDue()) {
        SMHW.PrintCentered();
        Serial.print(DEBUG_LINE_END);
    }
}

/// @brief Report translation and rotation values if enabled.
/// @param velocity pointer to velocity array
/// @param keyOut pointer to keyOut array
void debugOutput4(int16_t *velocity, uint8_t *keyOut) {
    //
    if (isDebugOutputDue()) {
        for (int i = 0; i < 6; i++) {
            sprintf(debugOutputBuffer, "%2.2s: %4d ", velNames[i], velocity[i]);
            Serial.print(debugOutputBuffer);
        }
        for (int i = 0; i < NUMKEYS; i++) {
            Serial.print("K");
            Serial.print(i);
            Serial.print(":");
            Serial.print(keyOut[i]);
            Serial.print(", ");
        }
        Serial.print(DEBUG_LINE_END);
    }
}

/// @brief Report single axis and resulting velocities info side by side for direct reference. Very useful if you need to alter which inputs are used in the arithmetic above.
/// @param centered pointer to arrays of 8 axis
/// @param velocity pointer to array of 6 velocities

void debugOutput5(SpaceMouseHW_ &SMHW, int16_t *velocity) {
    if (isDebugOutputDue()) {
        SMHW.PrintCentered();
        Serial.print(" || ");
        for (int i = 0; i < 6; i++) {
            sprintf(debugOutputBuffer, "%2.2s: %4d ", velNames[i], velocity[i]);
            Serial.print(debugOutputBuffer);
        }
        Serial.print(DEBUG_LINE_END);
    }
}

/// @brief Check, if a new debug output shall be generated. This is used in order to generate a debug line only every DEBUGDELAY ms, see config.h
/// @return true, if debug message is due
bool isDebugOutputDue() {
    static unsigned long lastDebugOutput = 0; // time from millis(), when the last debug output was given

    if (millis() - lastDebugOutput > DEBUGDELAY) {
        lastDebugOutput = millis();
        return true;
    } else {
        return false;
    }
}

uint16_t iterationsPerSecond = 0;      // count the iterations within one second
unsigned long lastFrequencyUpdate = 0; // time from millis(), when the last frequency was calculated

/// @brief update and report the function to learn at what frequency the loop is running
void updateFrequencyReport() {
    // increase iterations counter
    iterationsPerSecond++;
    if (millis() - lastFrequencyUpdate > 1000) { // if one second has past: report frequency
        Serial.print("Frequency: ");
        Serial.print(iterationsPerSecond);
        Serial.println(" Hz");
        lastFrequencyUpdate = millis(); // reset timer
        iterationsPerSecond = 0;        // reset iteration counter
    }
}
