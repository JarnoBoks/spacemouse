#pragma once

#include "HIDStateBase.h"
#include <hidhandler/translator/TranslatorKeys.h> // for TranslatorKeys

// Includes for the target states
#include "HIDStateStart.h"

class HIDStateSendkeys : public HIDStateBase {
protected:
public:
    HIDStateSendkeys(HIDStateData *data) // Constructor to initialize the state data
        : HIDStateBase(data) {
        translator = new TranslatorKinematicsBase(); // Initialize the translator for key data  //FIXME - Should be TranslatorKeys
    }
    virtual ~HIDStateSendkeys() {
        delete translator; // Clean up the translator instance
    }

    void apply() override {
#if 0
        if (!isNewHidReportDue) {
            return; // if no new HID report is due, return
        }

        data->lastHIDsentRep += HIDUPDATERATE_MS;
        data->hasSentNewData = true;                // return value
        context->setState(new HIDStateStart(data)); // Go back to start state
#endif
    }

    void report() override {
        // Report the current state of the axis observer
        // This function is empty because there is no need to report anything in this state.
    }
};