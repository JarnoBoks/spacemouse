#include "HIDStateSendKeys.h"
#include <hidhandler/translator/TranslatorKinematicsBase.h> // for TranslatorKeys
#include <hidhandler/HIDHandlerController.h>                // For HIDHandlerController (context)

// Includes for the target states
#include "HIDStateStart.h"

HIDStateSendkeys::HIDStateSendkeys(HIDStateData *data) // Constructor to initialize the state data
    : HIDStateBase(data) {
    translator = new TranslatorKinematicsBase(); // Initialize the translator for key data  //FIXME - Should be TranslatorKeys
}
HIDStateSendkeys::~HIDStateSendkeys() {
    delete translator; // Clean up the translator instance
}

void apply() {
#if 0
        if (!isNewHidReportDue) {
            return; // if no new HID report is due, return
        }

        data->lastHIDsentRep += HIDUPDATERATE_MS;
        data->hasSentNewData = true;                // return value
        context->setState(new HIDStateStart(data)); // Go back to start state
#endif
}
