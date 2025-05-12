#include "HIDStateSendRotation.h"
#include <hidhandler/HIDHandlerController.h>                   // For HIDHandlerController (context)
#include <observers/HIDEventBuffer/HIDEventBufferRotation.hpp> // For HIDEventBuffer
#include <hidhandler/sender/HIDSenderRotation.hpp>             // For HIDSenderRotation

// Includes for the possible target states
#include "HIDStateSendkeys.h"

void HIDStateSendrotation::apply() {
    // Serial.println(F("HIDStateSendRotation::apply()"));

    const bool staged = context->getHIDEventBufferRotation()->isStaged();
    // Serial.print(F(" - stagedRot: "));
    // Serial.print(staged);

    if (!isNewHidReportDue()) {
        return;
    }

    if (!(staged || m_data->countRotZeros < 3)) {
        // Serial.println(F("returning"));
        return;
    }

    SpaceMouseUSBInterface_ *usbInterface = SpaceMouseUSBInterface_::getInstance();                              // Get the USB interface instance
    usbInterface->SendReport(REPORTID_ROT, context->getHIDEventBufferRotation()->getStaged(), HID_MESSAGE_SIZE); // send new rotational values

    // HIDSenderRotation hidSender(context->getHIDEventBufferRotation()->getStaged()); // Create a new HIDSender instance with the staged rotation data
    // hidSender.sendData();                                                           // Send the rotation data
    context->getHIDEventBufferRotation()->clearStaged(); // Clear the staged rotation data

    // Increment or reset the zero counter.
    // It is safe to assume the translator is executed and is of type TranslatorKinematicsRotation.
    m_data->countRotZeros = (staged) ? 0 : m_data->countRotZeros + 1; // Increment the zero counter if rotation data is staged

    m_data->lastHIDsentRep += HIDUPDATERATE_MS;
    m_data->hasSentNewData = true; // REFACTOR - Is this used anywhere?

    // NOTE: In the original software, there was a check for the key data to see if it was different from the previous key data.
    //       This is not necessary in the new implementation, as the key data is handled separately in the HIDStateSendkeys class.
    context->setState(new HIDStateSendkeys());
}
