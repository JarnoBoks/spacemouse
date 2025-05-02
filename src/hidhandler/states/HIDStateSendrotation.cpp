#include "HIDStateSendRotation.h"
#include <hidhandler/translator/TranslatorKinematicsRotation.h> // for TranslatorKinematicsRotation
#include <hidhandler/HIDHandlerController.h>                    // For HIDHandlerController (context)

#include "config.h" // for NUMKEYS

// Includes for the target states
#include "HIDStateStart.h"
#include "HIDStateSendkeys.h"

HIDStateSendrotation::HIDStateSendrotation(HIDStateData *data) // Constructor to initialize the state data
    : HIDStateBase(data) {
    translator = new TranslatorKinematicsRotation(); // Initialize the translator for rotation data
}
HIDStateSendrotation::~HIDStateSendrotation() {
    delete translator;
}

void HIDStateSendrotation::apply() {
    Serial.print("Send rotation: ");
    if (!isNewHidReportDue()) {
        return;
    }

    translator->execute();

    // Increment or reset the zero counter
    data->countRotZeros = (static_cast<TranslatorKinematicsRotation *>(translator)->isAllZeroValues()) ? data->countRotZeros + 1 : 0;

    data->lastHIDsentRep += HIDUPDATERATE_MS;
    data->hasSentNewData = true;

    // NOTE: In the original software, there was a check for the key data to see if it was different from the previous key data.
    //       This is not necessary in the new implementation, as the key data is handled separately in the HIDStateSendkeys class.
    context->setState(new HIDStateSendkeys(data)); // Set the next state to start
}
