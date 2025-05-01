#pragma once

#include "SpaceMouseHIDStateBase.h"
#include "SpaceMouseHIDStateStart.h"
#include <Arduino.h> // for millis()
#include "config.h"  // for ADV_HID_JIGGLE

class SpaceMouseHIDStateInit : public SpaceMouseHIDStateBase {
    // This class is used to handle the debug parameter for axis information.
    // It inherits from ISpaceMouseHIDState and implements the apply and report methods.
    // The apply method is used to apply the changes to the axis observer.
    // The report method is used to report the current state of the axis observer.
protected:
public:
    SpaceMouseHIDStateInit() = default;   // Constructor
    virtual ~SpaceMouseHIDStateInit() {}; // Destructor to clean up the observer instance

    void apply() override {
        // Set the current time to now
        // this is used to check if the 8ms since the last report have passed.
        now = millis();
        lastHIDsentRep = now;   // Set the last HID report time to now
        hasSentNewData = false; // this value will be returned
#ifdef ADV_HID_JIGGLE
        toggleValue = false; // reset the toggle value
#endif

        context->setState(new SpaceMouseHIDStateStart()); // Set the next state to start
    }

    void report() override {
        // Report the current state of the axis observer
        // This function is empty because there is no need to report anything in this state.
    }
};