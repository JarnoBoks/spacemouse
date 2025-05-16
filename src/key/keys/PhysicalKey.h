
#pragma once
#include "Key.hpp"
#include "config_physicalkey.h"

// Include Arduino library for digitalRead and pinMode functions
#include <Arduino.h>

/**
 * @brief Class representing a physical key with debounce functionality.
 * @details This class inherits from the Key class and implements the evaluate method to handle the key state changes.
 *          It uses a pin number to read the key state and includes a debounce feature to prevent multiple triggers.
 * @note The PhysicalKey class is designed to manage the physical interaction of keys, ensuring reliable state changes.
 * @warning Ensure that the pin number is correctly configured to avoid unexpected behavior.
 */
class PhysicalKey : public Key {
private:
    const int8_t m_pinNumber = -1; // Pin number for the physical key

    // Physical keys have a debounce feature to prevent multiple triggers.
    bool m_previousKeyRawState = false;   // Last state of the key, necessary for debouncing
    unsigned long m_lastDebounceTime = 0; // Last time the key state was changed.

public:
    PhysicalKey() = delete;
    PhysicalKey(int8_t id, int8_t pin) : Key(id), m_pinNumber(pin) {
        // Constructor to initialize the physical key with an ID and pin number
        pinMode(m_pinNumber, INPUT_PULLUP); // Set the pin mode to input with pull-up resistor
    }
    ~PhysicalKey() override = default; // Destructor

    inline void evaluate() override {

        bool keyRawState = false; // Variable to store the raw state of the key
        if (m_pinNumber >= 0) {
            keyRawState = (digitalRead(m_pinNumber) == LOW); // Assuming LOW means pressed
        }

        // If the key state has changed from the last reading, restart the debounce period
        if (keyRawState != m_previousKeyRawState) {
            m_lastDebounceTime = millis();
            m_previousKeyRawState = keyRawState; // Update the last key state
            return;
        }

        // If the key raw state is stable for the debounce time and the RawValue differs from the KeyState,
        // update the key state and call the appropriate functionality.
        if ((millis() - m_lastDebounceTime) > DEBOUNCE_KEYS_MS && m_keyState != keyRawState) {

            m_keyState = keyRawState;

            if (m_keystrategy != nullptr) {
                if (m_keyState) {
                    m_keystrategy->onPress();
                } else {
                    m_keystrategy->onRelease();
                }
            }
        }
    }

    friend class KeyFactoryPhysicalkey; // Allow KeyFactoryPhysicalkey to access private members
};