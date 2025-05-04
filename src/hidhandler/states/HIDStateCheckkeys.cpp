#include "HIDStateCheckkeys.h"
#include <hidhandler/translator/TranslatorKeys.h> // for TranslatorKinematicsBase
#include <hidhandler/HIDHandlerController.h>      // For HIDHandlerController (context)

// Includes for the possible target states
#include "HIDStateSendkeys.h"
#include "HIDStateStart.h"

HIDStateCheckKeys::HIDStateCheckKeys() {
    translator = new TranslatorKeys(data->prevKeyData); // Initialize the translator for key data  // FIXME: Test if this is working correctly (does the entire array is passed to the constructor or only the first element?)
}

HIDStateCheckKeys::~HIDStateCheckKeys() {
    delete translator; // Clean up the translator instance
};

void HIDStateCheckKeys::apply() {

    // Check if there is something to send.
    // If any of the keys has changed, proceed to the 'send keys' state, other wise go back to the start state.
    if (static_cast<TranslatorKeys *>(translator)->isAnythingChanged()) {
        context->setState(new HIDStateSendkeys()); // Set the next state to send keys
    } else {
        context->setState(new HIDStateStart()); // Set the next state to translation

        if (isNewHidReportDue()) {
            // Keep the timestamp for the last sent package nearby.
            data->lastHIDsentRep = data->now - HIDUPDATERATE_MS;
        }
    }
};