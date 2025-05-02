#include "HIDStateStart.h"
#include <hidhandler/translator/TranslatorKinematicsBase.h> // for TranslatorKinematicsBase
#include <hidhandler/HIDHandlerController.h>                // For HIDHandlerController (context)

// Includes for the target states
#include "HIDStateSendtranslation.h"

HIDStateStart::HIDStateStart(HIDStateData *data) // Constructor to initialize the state data
    : HIDStateBase(data) {
    translator = new TranslatorKinematicsBase(); // Initialize the translator for translation data
}

HIDStateStart::~HIDStateStart() {
    delete translator; // Clean up the translator instance
};

void HIDStateStart::apply() {
    Serial.print("Current state: HIDStateStart");
    // Check if there is something to send. If there are zero data packages to send (have to send 3 in total) or if any of the axes has movement,
    // proceed to the next state. This function is evaluated every time the state is called.
    if (data->countTransZeros < 3 || data->countRotZeros < 3 || static_cast<TranslatorKinematicsBase *>(translator)->areAllAxisZero()) {
        context->setState(new HIDStateSendtranslation(data)); // Set the next state to start
    } else {
        // if nothing is to be sent, check for keys. If no keys, don't change state
#if (NUMKEYS > 0)
        if (memcmp(keyData, prevKeyData, HIDMAXBUTTONS / 8) != 0)
        // compare key data to previous key data
        {
            context->setState(new SpaceMouseHIDStateSendkeys(data)); // Set the next state to start
        }
#endif
        if (context->getState() == this && isNewHidReportDue()) {
            // if we are still in this state, check if we are waiting here for more than the update rate
            // keep the timestamp for the last sent package nearby
            data->lastHIDsentRep = data->now - HIDUPDATERATE_MS;
        }
    }
};