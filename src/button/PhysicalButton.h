
// PhysicalButton.h
#pragma once
#include "ButtonBase.h"

#include "functionality/CommandType.h"

#define DEBOUNCE_TIME 200 // Default debounce time for physical button in milliseconds

class PhysicalButton : public ButtonBase {
private:
    // Physical keys have a debounce feature to prevent multiple triggers.
    bool lastButtonState = false;       // Last state of the button, necessary for debouncing
    unsigned long lastDebounceTime = 0; // Last time the button state was changed.

    // configuration settings
    int pinNumber = -1; // Pin number for the physical button

public:
    PhysicalButton() : ButtonBase(), pinNumber(-1) {}
    PhysicalButton(int8_t id) : ButtonBase(id), pinNumber(-1) {}
    PhysicalButton(int8_t id, int8_t pin) : ButtonBase(id), pinNumber(pin) {
        // Constructor to initialize the physical button with an ID and pin number
        pinMode(pinNumber, INPUT_PULLUP); // Set the pin mode to input with pull-up resistor
    }

    void evaluate() override {
        // Implement logic to read the button status
        // int8_t pinNumber = static_cast<PhysicalButtonConfig *>(config)->getPin(); // Get the pin number from the configuration

        if (pinNumber >= 0) {
            buttonState = (digitalRead(pinNumber) == LOW); // Assuming LOW means pressed
        }

        if (buttonState != lastButtonState) {
            // If the button state has changed, update the debounce time
            lastDebounceTime = millis();
        }

        if ((millis() - lastDebounceTime) > DEBOUNCE_TIME) {
            // If the button state is stable for the debounce time, update the button state
            if (buttonState != lastButtonState) {
                lastButtonState = buttonState;
            }

            if (functionality && config->isEnabled()) {
                // Call the appropriate functionality based on the button state
                if (buttonState) {
                    functionality->onPress(); // Call press() if the button is pressed
                } else {
                    functionality->onRelease(); // Call release() if the button is released
                }
            }
        }
    }

    inline void setConfig(ButtonConfig *cfg) override {
        config = static_cast<PhysicalButtonConfig *>(cfg);
    }

    inline PhysicalButtonConfig *getConfig() const override {
        return static_cast<PhysicalButtonConfig *>(config);
    }
};