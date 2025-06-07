#pragma once

#include <stdint.h>                      // for uint8_t
#include <Arduino.h>                     // for millis()
#include <hidhandler/HIDHandlerConfig.h> // for HIDKEYDATASIZE

/**
 * @brief This class is used to store the state data for the HID state machine. It contains
 *        variables that are used to manage the state transitions and timing of the HID report sending.
 */
class HIDStateData {
public:
    HIDStateData() {
        // Constructor to initialize the state data
        for (int i = 0; i < HIDKEYDATASIZE; i++) {
            prevKeyData[i] = 0; // Initialize the previous key data to zero
        }
        now = millis();
        lastHIDsentRep = now; // Set the last HID report time to now
    }

    // State variables - these are used to manage the state(or state transitions) of the HID report sending
    unsigned long lastHIDsentRep; // Last time the HID report was sent
    unsigned long now;            // The time the state sequence started in milliseconds
    uint8_t countTransZeros = 0;  // Counter for the number of Translation zero data packages sent
    uint8_t countRotZeros = 0;    // Counter for the number of Rotation zero data packages sent

    uint8_t prevKeyData[HIDKEYDATASIZE] = {0}; // Array to hold the previous key state
#ifdef ADV_HID_JIGGLE
    bool toggleValue = false; // Variable to track if values shall be jiggled or not
#endif
}; // End of HIDStateData class
