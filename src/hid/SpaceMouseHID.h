#pragma once

#include "SpaceMouseUSBInterface.h"
#include "SpaceMouseHIDStateMachine.h"

class SpaceMouseUSBInterface_;   // Forward declaration of SpaceMouseUSBInterface_ class
class SpaceMouseHIDStateMachine; // Forward declaration of SpaceMouseHIDStateMachine class

class SpaceMouseHID {
private:
    SpaceMouseUSBInterface_ *usbInterface = nullptr;      // Pointer to the USB interface instance
    SpaceMouseHIDStateMachine *stateController = nullptr; // Pointer to the state machine instance
protected:
public:
    SpaceMouseHID() {
        // Default constructor
        usbInterface = new SpaceMouseUSBInterface_();      // Initialize the USB interface
        stateController = new SpaceMouseHIDStateMachine(); // Initialize the state machine
    }
    virtual ~SpaceMouseHID() = default; // Default destructor

    virtual void execute() = 0; // Pure virtual function to be implemented by derived classes
};