
// PhysicalKey.h
#pragma once
#include "KeyBase.h"

#include "functionality/CommandType.h"

#define DEBOUNCE_TIME 200 // Default debounce time for physical key in milliseconds

class PhysicalKey : public KeyBase {
private:
    // Physical keys have a debounce feature to prevent multiple triggers.
    bool lastKeyState = false;          // Last state of the key, necessary for debouncing
    unsigned long lastDebounceTime = 0; // Last time the key state was changed.

    // configuration settings
    int pinNumber = -1; // Pin number for the physical key

public:
    PhysicalKey() : KeyBase(), pinNumber(-1) {}
    PhysicalKey(int8_t id) : KeyBase(id), pinNumber(-1) {}
    PhysicalKey(int8_t id, int8_t pin) : KeyBase(id), pinNumber(pin) {
        // Constructor to initialize the physical key with an ID and pin number
        pinMode(pinNumber, INPUT_PULLUP); // Set the pin mode to input with pull-up resistor
    }

    void evaluate() override {
        // Implement logic to read the key status
        // int8_t pinNumber = static_cast<PhysicalKeyConfig *>(config)->getPin(); // Get the pin number from the configuration

        if (pinNumber >= 0) {
            keyState = (digitalRead(pinNumber) == LOW); // Assuming LOW means pressed
        }

        if (keyState != lastKeyState) {
            // If the key state has changed, update the debounce time
            lastDebounceTime = millis();
        }

        if ((millis() - lastDebounceTime) > DEBOUNCE_TIME) {
            // If the key state is stable for the debounce time, update the key state
            if (keyState != lastKeyState) {
                lastKeyState = keyState;
            }

            if (functionality && config->isEnabled()) {
                // Call the appropriate functionality based on the key state
                if (keyState) {
                    functionality->onPress(); // Call press() if the key is pressed
                } else {
                    functionality->onRelease(); // Call release() if the key is released
                }
            }
        }
    }

    inline void setConfig(KeyConfig *cfg) override {
        config = static_cast<PhysicalKeyConfig *>(cfg);
    }

    inline PhysicalKeyConfig *getConfig() const override {
        return static_cast<PhysicalKeyConfig *>(config);
    }
};