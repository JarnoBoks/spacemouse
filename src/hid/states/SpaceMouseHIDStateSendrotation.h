#pragma once

#include "SpaceMouseHIDStateBase.h"
#include "SpaceMouseHIDStateStart.h"
#include "SpaceMouseHIDStateSendkeys.h"

#include <Arduino.h> // for millis()
#include "config.h"  // for ADV_HID_JIGGLE & NUMKEYS

class SpaceMouseHIDStateSendrotation : public SpaceMouseHIDStateBase {
    // This class is used to handle the debug parameter for axis information.
    // It inherits from ISpaceMouseHIDState and implements the apply and report methods.
    // The apply method is used to apply the changes to the axis observer.
    // The report method is used to report the current state of the axis observer.
protected:
public:
    SpaceMouseHIDStateSendrotation() = default;   // Constructor
    virtual ~SpaceMouseHIDStateSendrotation() {}; // Destructor to clean up the observer instance

    void apply() override {
        // send rotational data, if the 8 ms from the last hid report have past
        if (!isNewHidReportDue()) {
            return; // if no new HID report is due, return
        }

        // TODO SEND ROTATION DATA HERE
        lastHIDsentRep += HIDUPDATERATE_MS;
        hasSentNewData = true; // return value

// check if the next state should be keys
#if (NUMKEYS > 0)
        if (memcmp(keyData, prevKeyData, HIDMAXBUTTONS / 8) != 0)
        // compare key data to previous key data
        {
            context->setState(new SpaceMouseHIDStateSendkeys()); // Set the next state to start
        } else {
            // go back to start
            context->setState(new SpaceMouseHIDStateStart()); // Set the next state to start
        }
#else
        // if no keys are used, go to start state after rotations
        context->setState(new SpaceMouseHIDStateStart()); // Set the next state to start
#endif
    }

    void
    report() override {
        // Report the current state of the axis observer
        // This function is empty because there is no need to report anything in this state.
    }
};