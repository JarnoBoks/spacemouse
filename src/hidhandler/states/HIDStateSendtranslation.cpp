#include "HIDStateSendtranslation.h"
#include <hidhandler/HIDHandlerController.h>          // For HIDHandlerController (context)
#include <observers/HIDEventBuffer.hpp>               // For HIDEventBuffer
#include <hidhandler/sender/HIDSenderTranslation.hpp> // For HIDSenderTranslation

// Includes for the possible target states
#include "HIDStateSendrotation.h"

HIDStateSendtranslation::HIDStateSendtranslation() {
    // REMOVE    translator = new TranslatorKinematicsTranslation();
}

HIDStateSendtranslation::~HIDStateSendtranslation() {
    // REMOVE delete translator; // Clean up the translator instance
};

void HIDStateSendtranslation::apply() {
    if (!isNewHidReportDue()) {
        return;
    }

    // FIXME - We have to add the JIGGLE functionality here.
    HIDSenderTranslation hidSender(context->getHIDEventBuffer()->getTranslationStaged()); // Create a new HIDSender instance with the staged translation data
    hidSender.sendData();                                                                 // Send the translation data

    // Increment or reset the zero counter.
    data->countTransZeros = (context->getHIDEventBuffer()->isTranslationStaged()) ? 0 : data->countTransZeros + 1; // Increment the zero counter if translation data is staged

    data->lastHIDsentRep += HIDUPDATERATE_MS;
    data->hasSentNewData = true; // REFACTOR - Is this used anywhere?

    // NOTE: In the original software, there was a check for the key data to see if it was different from the previous key data.
    //       This is not necessary in the new implementation, as the key data is handled separately in the HIDStateSendkeys class.
    context->setState(new HIDStateSendrotation());
}

#if 0
void HIDStateSendtranslation::apply() {

    if (!isNewHidReportDue()) {
        return; // if no new HID report is due, return
    }

#ifdef ADV_HID_JIGGLE
    jiggleValues(trans, toggleValue); // jiggle the non-zero values, if toggleValue is true
                                      // the toggleValue is toggled after sending the rotations, down below
#endif

    //  Send new translational values
    translator->sendData();

    // Increment or reset the zero counter
    // It is safe to assume the translator is executed and is of type TranslatorKinematicsRotation.
    data->countTransZeros = (static_cast<TranslatorKinematicsTranslation *>(translator)->isAllZeroValues()) ? data->countTransZeros + 1 : 0;

    data->lastHIDsentRep += HIDUPDATERATE_MS;
    data->hasSentNewData = true;
    context->setState(new HIDStateSendrotation()); // Set the next state to start
}
#endif
