#include "HIDStateSendKeys.h"
#include <hidhandler/HIDHandlerController.h>               // For HIDHandlerController (context)
#include <observers/HIDEventBuffer/HIDEventBufferKeys.hpp> // For HIDEventBuffer
#include <hidhandler/sender/HIDSenderKeys.hpp>             // For HIDSenderKeys

// Includes for the possible target states
#include "HIDStateSendtranslation.h"

void HIDStateSendkeys::apply() {
    if (!isNewHidReportDue()) {
        return;
    }

    //  Send a message if new data is staged or if the zero counter is less than 3
    const bool staged = context->getHIDEventBufferKeys()->isStaged();
    if (staged) {
        HIDSenderKeys hidSender(context->getHIDEventBufferKeys()->getStaged()); // Create a new HIDSender instance with the staged keys data
        hidSender.sendData();                                                   // Send the keys data
        context->getHIDEventBufferKeys()->clearStaged();                        // Clear the staged keys data

        m_data->lastHIDsentRep += HIDUPDATERATE_MS;
        m_data->hasSentNewData = true; // REFACTOR - Is this used anywhere?
    }

    context->setState(new HIDStateSendtranslation());
}