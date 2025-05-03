#pragma once

#ifdef ARDUINO_ARCH_AVR
//  Include inbuilt Arduino HID library by NicoHood: https://github.com/NicoHood/HID
#include "HID.h"
#else
// FIXME - The HID library is not compatible with the ESP32. The ESP32 uses the BLE HID library instead.
#endif // ARDUINO_ARCH_AVR

// Include the State Controller header file
#include "HIDHandlerController.h"
#include "states/HIDStateInit.h"

class HIDHandlerController; // Forward declaration of HIDHandlerController class

class SpaceMouseHID {
private:
    HIDHandlerController *controller_ = nullptr; // Pointer to the state machine instance

protected:
public:
    SpaceMouseHID() {
        Serial.println("SpaceMouseHID::SpaceMouseHID()"); // Debug output to indicate the constructor is called
        controller_ = new HIDHandlerController();         // Initialize the state machine to the initial state
    }

    ~SpaceMouseHID() {
        delete controller_;
    }

    void execute() {
        Serial.println("SpaceMouseHID::execute()"); // Debug output to indicate the execute method is called
        controller_->execute();
    }
};