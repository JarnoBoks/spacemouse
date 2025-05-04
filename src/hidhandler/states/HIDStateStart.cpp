#include "HIDStateStart.h"
#include <hidhandler/translator/TranslatorKinematicsBase.h> // for TranslatorKinematicsBase
#include <hidhandler/HIDHandlerController.h>                // For HIDHandlerController (context)

// Includes for the target states
#include "HIDStateSendtranslation.h"
#include "HIDStateSendkeys.h"

HIDStateStart::HIDStateStart() {
    translator = new TranslatorKinematicsBase(); // Initialize the translator for translation data
}

HIDStateStart::~HIDStateStart() {
    delete translator; // Clean up the translator instance
};

void HIDStateStart::apply() {

    //  Check if there is something to send. If there are zero data packages to send (have to send 3 in total) or if any of the axes has movement,
    //  proceed to the next state. This function is evaluated every time the state is called.
    if (data->countTransZeros < 3 || data->countRotZeros < 3 || static_cast<TranslatorKinematicsBase *>(translator)->areAllAxisZero()) {
        context->setState(new HIDStateSendtranslation()); // Set the next state to translation
    } else {
#if (NUMKEYS > 0)
        // TODO - Extra state for keycheck
        <TranslatorKeys *>(translator)->isAnythingChanged()) { // Check for changes in key data
            context->setState(new HIDStateStart());            // Go back to start state
            return;
        }

        // if nothing is to be sent, check for keys. If no keys, don't change state
        if (memcmp(data->keyData, prevKeyData, HIDMAXBUTTONS / 8) != 0)
        // compare key data to previous key data
        {
            context->setState(new HIDStateSendkeys()); // Set the next state to send keys
        }
#endif
        //  if we are still in this state (ie. not in keys), check if we are waiting here
        //  for more than the update rate.
        if (context->getState() == this && isNewHidReportDue()) {

            // Keep the timestamp for the last sent package nearby.
            data->lastHIDsentRep = data->now - HIDUPDATERATE_MS;
        }
    }
};