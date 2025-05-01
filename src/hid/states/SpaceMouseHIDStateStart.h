#pragma once

#include <hid/SpaceMouseConfig.h>
#include <hid/SpaceMouseHIDStateMachine.h>
#include "SpaceMouseHIDStateBase.h"
#include "SpaceMouseHIDStateSendtranslation.h"

class SpaceMouseHIDStateStart : public SpaceMouseHIDStateBase {
    // This class is used to handle the debug parameter for axis information.
    // It inherits from ISpaceMouseHIDState and implements the apply and report methods.
    // The apply method is used to apply the changes to the axis observer.
    // The report method is used to report the current state of the axis observer.
protected:
public:
    SpaceMouseHIDStateStart() = default; // Constructor
    virtual ~SpaceMouseHIDStateStart();  // Destructor to clean up the observer instance

    void apply() override {
        // Evaluate everytime, without waiting for 8ms
        if (countTransZeros < 3 || countRotZeros < 3 || (x != 0 || y != 0 || z != 0 || rx != 0 || ry != 0 || rz != 0)) {
            // If one of the values is not zero
            // or not all zero data packages are sent (sent 3 of them)
            // start sending data
            context->setState(new SpaceMouseHIDStateSendtranslation()); // Set the next state to start
        } else {
            // if nothing is to be sent, check for keys. If no keys, don't change state
#if (NUMKEYS > 0)
            if (memcmp(keyData, prevKeyData, HIDMAXBUTTONS / 8) != 0)
            // compare key data to previous key data
            {
                context->setState(new SpaceMouseHIDStateSendkeys()); // Set the next state to start
            }
#endif
            if (nextState == ST_START && isNewHidReportDue()) {
                // if we are not leaving the start state and
                // we are waiting here for more than the update rate,
                // keep the timestamp for the last sent package nearby
                lastHIDsentRep = now - HIDUPDATERATE_MS;
            }
        }
    };

    void report() override;
};