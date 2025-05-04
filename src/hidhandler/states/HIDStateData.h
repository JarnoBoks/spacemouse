// HIDStateData.h
#pragma once

#include <stdint.h>                      // for uint8_t
#include <Arduino.h>                     // for millis()
#include <hidhandler/HIDHandlerConfig.h> // for KEYDATASIZE

/**
 * @brief This class is used to store the state data for the HID state machine. It contains
 *        variables that are used to manage the state transitions and timing of the HID report sending.
 */
class HIDStateData {
public:
    HIDStateData() {
        // Constructor to initialize the state data
        for (int i = 0; i < KEYDATASIZE; i++) {
            prevKeyData[i] = 0; // Initialize the previous key data to zero
        }
        now = millis();
        lastHIDsentRep = now; // Set the last HID report time to now
    }

    /**
     * @brief This function prints the current state data for debugging purposes.
     */
    void printHIDStateData() {
        // Output the state data for debugging purposes
        Serial.print(F("__data= lastHIDsentRep: "));
        Serial.print(lastHIDsentRep);
        Serial.print(F(", now: "));
        Serial.print(now);
        Serial.print(F(", hasSentNewData: "));
        Serial.print(hasSentNewData);
        Serial.print(F(", countTransZeros: "));
        Serial.print(countTransZeros);
        Serial.print(F(", countRotZeros: ")); // Updated to use F() macro for string literal
        Serial.print(countRotZeros);
        Serial.println();
    }

    // State variables - these are used to manage the state(or state transitions) of the HID report sending
    unsigned long lastHIDsentRep; // Last time the HID report was sent
    unsigned long now;            // The time the state sequence started in milliseconds
    bool hasSentNewData = false;  // Flag to indicate if new data has been sent

    uint8_t countTransZeros = 0; // Counter for the number of Translation zero data packages sent
    uint8_t countRotZeros = 0;   // Counter for the number of Rotation zero data packages sent

    uint8_t prevKeyData[KEYDATASIZE] = {0}; // Array to hold the previous key state
#ifdef ADV_HID_JIGGLE
    bool toggleValue = false; // Variable to track if values shall be jiggled or not
#endif
}; // End of HIDStateData class
