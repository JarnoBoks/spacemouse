#include "HIDStateSendRotation.h"
#include <hidhandler/HIDHandlerController.h>                   // For HIDHandlerController (context)
#include <observers/HIDEventBuffer/HIDEventBufferRotation.hpp> // For HIDEventBuffer
#include <hidhandler/sender/HIDSenderRotation.hpp>             // For HIDSenderRotation

// Includes for the possible target states
#include "HIDStateSendkeys.h"

void HIDStateSendrotation::apply() {

    if (!isNewHidReportDue()) {
        return;
    }

    //  Send a message if new data is staged or if the zero counter is less than 3
    const bool staged = context->getHIDEventBufferRotation()->isStaged();
    if (staged || m_data->countRotZeros < 3) {
        HIDSenderRotation hidSender(context->getHIDEventBufferRotation()->getStaged()); // Create a new HIDSender instance with the staged rotation data
        hidSender.sendData();                                                           // Send the rotation data
        context->getHIDEventBufferRotation()->clearStaged();                            // Clear the staged rotation data

        // Increment or reset the zero counter.
        m_data->countRotZeros = (staged) ? 0 : m_data->countRotZeros + 1; // Increment the zero counter if rotation data is staged

        m_data->lastHIDsentRep += HIDUPDATERATE_MS;
        m_data->hasSentNewData = true; // REFACTOR - Is this used anywhere?
    }

    context->setState(new HIDStateSendkeys());
}
