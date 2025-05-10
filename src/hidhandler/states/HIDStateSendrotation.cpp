#include "HIDStateSendRotation.h"
// REMOVE #include <hidhandler/translator/TranslatorKinematicsRotation.h> // for TranslatorKinematicsRotation
#include <hidhandler/HIDHandlerController.h>       // For HIDHandlerController (context)
#include <observers/HIDEventBuffer.hpp>            // For HIDEventBuffer
#include <hidhandler/sender/HIDSenderRotation.hpp> // For HIDSenderRotation

// Includes for the possible target states
#include "HIDStateStart.h"
#include "HIDStateSendkeys.h"

HIDStateSendrotation::HIDStateSendrotation() {
    // REMOVE translator = new TranslatorKinematicsRotation(); // Initialize the translator for rotation data
}
HIDStateSendrotation::~HIDStateSendrotation() {
    // REMOVE delete translator;
}

void HIDStateSendrotation::apply() {

    if (!isNewHidReportDue()) {
        return;
    }

    // REMOVE translator->sendData();
    HIDSenderRotation hidSender(context->getHIDEventBuffer()->getRotationStaged()); // Create a new HIDSender instance with the staged rotation data
    hidSender.sendData();                                                           // Send the rotation data

    // Increment or reset the zero counter.
    // It is safe to assume the translator is executed and is of type TranslatorKinematicsRotation.
    // REMOVE data->countRotZeros = (static_cast<TranslatorKinematicsRotation *>(translator)->isAllZeroValues()) ? data->countRotZeros + 1 : 0;
    data->countRotZeros = (context->getHIDEventBuffer()->isRotationStaged()) ? 0 : data->countRotZeros + 1; // Increment the zero counter if rotation data is staged

    data->lastHIDsentRep += HIDUPDATERATE_MS;
    data->hasSentNewData = true; // REFACTOR - Is this used anywhere?

    // NOTE: In the original software, there was a check for the key data to see if it was different from the previous key data.
    //       This is not necessary in the new implementation, as the key data is handled separately in the HIDStateSendkeys class.
    context->setState(new HIDStateSendkeys());
}
