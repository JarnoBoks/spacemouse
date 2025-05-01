#pragma once

#include "ISpaceMouseHIDState.h"
#include "config.h" // for ADV_HID_JIGGLE
#include "../SpaceMouseConfig.h"

class SpaceMouseHIDStateMachine; // forward declaration for the controller class

class SpaceMouseHIDStateBase : public ISpaceMouseHIDState {
    // This class is used to handle the debug parameter for axis information.
    // It inherits from ISpaceMouseHIDState and implements the apply and report methods.
    // The apply method is used to apply the changes to the axis observer.
    // The report method is used to report the current state of the axis observer.
protected:
    // Statemachine manager
    SpaceMouseHIDStateMachine *context;

    // State machine variables
    unsigned long lastHIDsentRep = 0; // Last time the HID report was sent
    unsigned long now = 0;            // The time the state sequence started in milliseconds
    bool hasSentNewData = false;      // Flag to indicate if new data has been sent
#ifdef ADV_HID_JIGGLE
    bool toggleValue = false; // Variable to track if values shall be jiggled or not
#endif

    /// Checks if a new HID report should be send
    bool isNewHidReportDue() {
        // calculate the difference between now and the last time it was sent
        // such a difference calculation is safe with regard to integer overflow after 48 days
        return (now - lastHIDsentRep >= HIDUPDATERATE_MS);
    }

public:
    virtual ~SpaceMouseHIDStateBase() {}; // Destructor to clean up the observer instance

    void set_context(SpaceMouseHIDStateMachine *context) {
        this->context = context;
    }

    void apply() override = 0;
    void report() override = 0;
};