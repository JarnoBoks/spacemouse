#include "HIDStateSendKeys.h"
#include <hidhandler/translator/TranslatorKeys.h> // for TranslatorKeys
#include <hidhandler/HIDHandlerController.h>      // For HIDHandlerController (context)

// Includes for the target states
#include "HIDStateStart.h"

HIDStateSendkeys::HIDStateSendkeys(HIDStateData *data) // Constructor to initialize the state data
    : HIDStateBase(data) {
    translator = new TranslatorKeys(data->prevKeyData); // Initialize the translator for key data  // FIXME: Test if this is working correctly (does the entire array is passed to the constructor or only the first element?)
}
HIDStateSendkeys::~HIDStateSendkeys() {
    delete translator; // Clean up the translator instance
}

void HIDStateSendkeys::apply() {
    // Check if there is something to send. If nothing is to be sent, go to the start state
#if 0
    if (memcmp(static_cast<TranslatorKeys *>(translator)->keyData, data->prevKeyData, KEYDATASIZE) == 0) { // Check for changes in key data
        context->setState(new HIDStateStart(data));                                                        // Go back to start state
        return;
    }
#endif
    if (!static_cast<TranslatorKeys *>(translator)->isAnythingChanged()) { // Check for changes in key data
        context->setState(new HIDStateStart(data));                        // Go back to start state
        return;
    }

    if (!isNewHidReportDue()) {
        return; // if no new HID report is due, return
    }

    translator->execute();                                                                      // Send the key data
    memcpy(data->prevKeyData, static_cast<TranslatorKeys *>(translator)->keyData, KEYDATASIZE); // Copy the current key data to the previous key data

    data->lastHIDsentRep += HIDUPDATERATE_MS;
    data->hasSentNewData = true;                // return value
    context->setState(new HIDStateStart(data)); // Go back to start state
}
