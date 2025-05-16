#pragma once

#ifdef ARDUINO_ARCH_AVR
//  Include inbuilt Arduino HID library by NicoHood: https://github.com/NicoHood/HID
// #include "HID.h"
#else
// FIXME - The HID library is not compatible with the ESP32. The ESP32 uses the BLE HID library instead.
#endif // ARDUINO_ARCH_AVR

// Include the State Controller header file
#include "HIDHandlerController.h"

class HIDHandlerController; // Forward declaration of HIDHandlerController class

class SpaceMouseHID {
private:
    HIDHandlerController *m_controller = nullptr; // Pointer to the state machine instance
protected:
public:
    SpaceMouseHID() {
        m_controller = new HIDHandlerController(); // Initialize the state machine to the initial state
    }

    ~SpaceMouseHID() {
        delete m_controller;
    }

    inline void execute() {
        m_controller->execute();
    }

    inline HIDHandlerController *getController() const {
        return m_controller;
    }
};