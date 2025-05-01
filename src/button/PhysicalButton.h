
// PhysicalButton.h
#pragma once
#include "Button.h"

#define DEBOUNCE_TIME 200 // Default debounce time for physical button in milliseconds

class PhysicalButton : public Button {
private:
    // Physical keys have a debounce feature to prevent multiple triggers.
    bool lastButtonState = false;       // Last state of the button, necessary for debouncing
    unsigned long lastDebounceTime = 0; // Last time the button state was changed.
public:
    void pressed() override {
        if (functionality)
            functionality->onPress();
    }
    void released() override {
        // Implement release logic
        if (functionality) {
            functionality->onRelease();
        }
    }

    void evaluate() override {
        // Implement logic to read the button status
        int8_t pinNumber = static_cast<PhysicalButtonConfig *>(config)->getPinNumber(); // Get the pin number from the configuration

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

            if (buttonState) {
                pressed(); // Call press() if the button is pressed
            } else {
                released(); // Call release() if the button is released
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