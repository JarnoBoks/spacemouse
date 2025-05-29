#include "HIDStateSendTranslation.h"
#include <hidhandler/HIDHandlerController.h>                      // For HIDHandlerController (context)
#include <observers/HIDEventBuffer/HIDEventBufferTranslation.hpp> // For HIDEventBuffer
#include <hidhandler/sender/HIDSenderTranslation.hpp>             // For HIDSenderTranslation

// Includes for the possible target states
#include "HIDStateSendrotation.h"

void HIDStateSendtranslation::apply() {

    if (!isNewHidReportDue()) {
        return;
    }

    if (!context || !context->getHIDEventBufferTranslation()) {
        return;
    }

    //  Send a message if new data is staged or if the zero counter is less than 3
    const bool staged = context->getHIDEventBufferTranslation()->isStaged();
    if (staged || m_data->countTransZeros < 3) {
        HIDSenderTranslation hidSender(context->getHIDEventBufferTranslation()->getStaged()); // Create a new HIDSender instance with the staged translation data
        hidSender.sendData();                                                                 // Send the translation data
        context->getHIDEventBufferTranslation()->clearStaged();                               // Clear the staged translation data

        // Increment or reset the zero counter.
        m_data->countTransZeros = (staged) ? 0 : m_data->countTransZeros + 1; // Increment the zero counter if translation data is staged

        m_data->lastHIDsentRep += HIDUPDATERATE_MS;
        // REMOVE m_data->hasSentNewData = true; // REFACTOR - Is this used anywhere?
    }

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
