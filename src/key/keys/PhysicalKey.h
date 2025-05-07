
// PhysicalKey.h
#pragma once
#include "Key.hpp"

// Include Arduino library for digitalRead and pinMode functions
#include <Arduino.h>

#define DEBOUNCE_TIME 200 // Default debounce time for physical key in milliseconds

/**
 * @brief Class representing a physical key with debounce functionality.
 */
class PhysicalKey : public Key {
private:
    int m_pinNumber = -1; // Pin number for the physical key

    // Physical keys have a debounce feature to prevent multiple triggers.
    bool m_lastKeyState = false;          // Last state of the key, necessary for debouncing
    unsigned long m_lastDebounceTime = 0; // Last time the key state was changed.

public:
    PhysicalKey() = default; // Default constructor
    PhysicalKey(int8_t id, int8_t pin) : Key(id), m_pinNumber(pin) {
        // Constructor to initialize the physical key with an ID and pin number
        pinMode(m_pinNumber, INPUT_PULLUP); // Set the pin mode to input with pull-up resistor
    }
    ~PhysicalKey() override = default; // Destructor

    inline void evaluate() override {

        if (m_pinNumber >= 0) {
            m_keyState = (digitalRead(m_pinNumber) == LOW); // Assuming LOW means pressed
        }

        if (m_keyState != m_lastKeyState) {
            // If the key state has changed, update the debounce time
            m_lastDebounceTime = millis();
        }

        if ((millis() - m_lastDebounceTime) > DEBOUNCE_TIME) {
            // If the key state is stable for the debounce time, update the key state
            if (m_keyState != m_lastKeyState) {
                m_lastKeyState = m_keyState;
            }

            if (functionality) {
                // Call the appropriate functionality based on the key state
                if (m_keyState) {
                    functionality->onPress(); // Call press() if the key is pressed
                } else {
                    functionality->onRelease(); // Call release() if the key is released
                }
            }
        }
    }

    friend class KeyFactoryPhysicalkey; // Allow KeyFactoryPhysicalkey to access private members
};