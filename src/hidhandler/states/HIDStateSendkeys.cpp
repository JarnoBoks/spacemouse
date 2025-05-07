#include "HIDStateSendKeys.h"
#include <hidhandler/translator/TranslatorKeys.h> // for TranslatorKeys
#include <hidhandler/HIDHandlerController.h>      // For HIDHandlerController (context)

// Includes for the possible target states
#include "HIDStateStart.h"

HIDStateSendkeys::HIDStateSendkeys() {
    // REMOVE translator = new TranslatorKeys(data->prevKeyData); // Initialize the translator for key data  // FIXME: Test if this is working correctly (does the entire array is passed to the constructor or only the first element?)
}
HIDStateSendkeys::~HIDStateSendkeys() {
    // REMOVE delete translator; // Clean up the translator instance
}

void HIDStateSendkeys::apply() {
    TranslatorKeys *translator = context->getTranslator(); // Get the translator instance from the context

    // Check if there is something to send. If nothing is to be sent, go to the start state
    if (!static_cast<TranslatorKeys *>(translator)->isAnythingChanged()) { // Check for changes in key data
        context->setState(new HIDStateStart());                            // Go back to start state
        return;
    }

    if (!isNewHidReportDue()) {
        return; // if no new HID report is due, return
    }

    translator->sendData();                                                                     // Send the key data
    memcpy(data->prevKeyData, static_cast<TranslatorKeys *>(translator)->keyData, KEYDATASIZE); // Copy the current key data to the previous key data

    data->lastHIDsentRep += HIDUPDATERATE_MS;
    data->hasSentNewData = true;            // return value
    context->setState(new HIDStateStart()); // Go back to start state
}
