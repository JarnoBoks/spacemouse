#pragma once

#include <Arduino.h> // for millis()
#include "config.h"  // for ADV_HID_JIGGLE

#include <hidhandler/HIDHandlerController.h>
#include "HIDStateBase.h"
#include "HIDStateStart.h"

class SpaceMouseHIDStateInit : public HIDStateBase {
protected:
public:
    SpaceMouseHIDStateInit(HIDStateData *data = nullptr) // Constructor to initialize the state data
        : HIDStateBase(data) {
    }
    virtual ~SpaceMouseHIDStateInit() = default; // Destructor to clean up the observer instance

    void apply() override {
        data->now = millis();             // Get the current time in milliseconds
        data->lastHIDsentRep = data->now; // Set the last HID report time to now
        data->hasSentNewData = false;     // this value will be returned
#ifdef ADV_HID_JIGGLE
        data->toggleValue = false; // reset the toggle value
#endif
        context->setState(new HIDStateStart(data)); // Set the next state to start
    }

    void report() override {
        // Report the current state of the axis observer
        // This function is empty because there is no need to report anything in this state.
    }
};