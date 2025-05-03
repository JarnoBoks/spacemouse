#include "HIDStateSendtranslation.h"
#include <hidhandler/translator/TranslatorKinematicsTranslation.h> // for TranslatorKinematicsRotation
#include <hidhandler/HIDHandlerController.h>                       // For HIDHandlerController (context)

// Includes for the target states
#include "HIDStateSendrotation.h"

HIDStateSendtranslation::HIDStateSendtranslation() {
    translator = new TranslatorKinematicsTranslation();
}

HIDStateSendtranslation::~HIDStateSendtranslation() {
    delete translator; // Clean up the translator instance
};

void HIDStateSendtranslation::apply() {
    if (!isNewHidReportDue()) {
        return; // if no new HID report is due, return
    }

#ifdef ADV_HID_JIGGLE
    jiggleValues(trans, toggleValue); // jiggle the non-zero values, if toggleValue is true
                                      // the toggleValue is toggled after sending the rotations, down below
#endif

    // Serial.print("Send translation: ");
    //  Send new translational values
    translator->execute();

    // Increment or reset the zero counter
    // It is safe to assume the translator is executed and is of type TranslatorKinematicsRotation.
    data->countTransZeros = (static_cast<TranslatorKinematicsTranslation *>(translator)->isAllZeroValues()) ? data->countTransZeros + 1 : 0;

    data->lastHIDsentRep += HIDUPDATERATE_MS;
    data->hasSentNewData = true;
    context->setState(new HIDStateSendrotation()); // Set the next state to start
}
