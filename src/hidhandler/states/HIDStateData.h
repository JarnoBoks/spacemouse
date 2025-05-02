#pragma once

#include <stdint.h>                      // for uint8_t
#include <hidhandler/HIDHandlerConfig.h> // for KEYDATASIZE

/**
 * @brief This class is used to store the state data for the HID state machine. It contains
 *        variables that are used to manage the state transitions and timing of the HID report sending.
 */
class HIDStateData {
public:
    // State variables - these are used to manage the state(or state transitions) of the HID report sending
    unsigned long lastHIDsentRep = 0; // Last time the HID report was sent
    unsigned long now = 0;            // The time the state sequence started in milliseconds
    bool hasSentNewData = false;      // Flag to indicate if new data has been sent

    uint8_t countTransZeros = 0; // Counter for the number of Translation zero data packages sent
    uint8_t countRotZeros = 0;   // Counter for the number of Rotation zero data packages sent

    uint8_t prevKeyData[KEYDATASIZE] = {0}; // Array to hold the previous key state
#ifdef ADV_HID_JIGGLE
    bool toggleValue = false; // Variable to track if values shall be jiggled or not
#endif
}; // End of HIDStateData class
