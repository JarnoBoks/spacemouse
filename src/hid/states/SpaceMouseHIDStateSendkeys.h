#pragma once

#include "SpaceMouseHIDStateBase.h"
#include "SpaceMouseHIDStateStart.h"

class SpaceMouseHIDStateSendkeys : public SpaceMouseHIDStateBase {
    // This class is used to handle the debug parameter for axis information.
    // It inherits from ISpaceMouseHIDState and implements the apply and report methods.
    // The apply method is used to apply the changes to the axis observer.
    // The report method is used to report the current state of the axis observer.
protected:
public:
    SpaceMouseHIDStateSendkeys() = default;   // Constructor
    virtual ~SpaceMouseHIDStateSendkeys() {}; // Destructor to clean up the observer instance

    void apply() override {
        if (!isNewHidReportDue) {
            return; // if no new HID report is due, return
        }

        lastHIDsentRep += HIDUPDATERATE_MS;
        hasSentNewData = true;                            // return value
        context->setState(new SpaceMouseHIDStateStart()); // Go back to start state
    }

    void report() override {
        // Report the current state of the axis observer
        // This function is empty because there is no need to report anything in this state.
    }
};