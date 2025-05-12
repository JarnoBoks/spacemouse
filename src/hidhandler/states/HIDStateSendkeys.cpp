#include "HIDStateSendKeys.h"
#include <hidhandler/HIDHandlerController.h> // For HIDHandlerController (context)

// Includes for the possible target states
#include "HIDStateStart.h"

void HIDStateSendkeys::apply() {
    Serial.println(F("HIDStateSendtranslation::apply()"));
    // Check if the HID report is due
    if (!isNewHidReportDue()) {
        return; // if no new HID report is due, return
    }

#if 0 // REFACTOR -
    TranslatorKeys *translator = context->getKeyTranslator(); // Get the Key translator instance from the context

#if 0
    // Check if there is something to send. If nothing is to be sent, go to the start state
    if (!static_cast<TranslatorKeys *>(translator)->isAnythingChanged()) { // Check for changes in key data
        context->setState(new HIDStateStart());                            // Go back to start state
        return;
    }
#endif
    // NOTE: The original code checked if there was something to send. This is not needed anymore, as the translator will only send data if there is something to send.

    if (!isNewHidReportDue()) {
        return; // if no new HID report is due, return
    }

    translator->sendData();
    context->getHIDEventBufferTranslation()->clearStaged(); // Clear the staged key data                                                                     // Send the key data
    memcpy(data->prevKeyData, static_cast<TranslatorKeys *>(translator)->keyData, HIDKEYDATASIZE); // Copy the current key data to the previous key data

    data->lastHIDsentRep += HIDUPDATERATE_MS;
    data->hasSentNewData = true;            // return value
#endif
    context->setState(new HIDStateStart()); // Go back to start state
}
