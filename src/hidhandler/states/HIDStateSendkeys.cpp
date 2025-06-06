#include "HIDStateSendKeys.hpp"
#include <hidhandler/HIDHandlerController.hpp>             // For HIDHandlerController (context)
#include <observers/HIDEventBuffer/HIDEventBufferKeys.hpp> // For HIDEventBuffer

#include <usbstack/HIDReportDescriptor.h> // for ReportIDs
#include <usbstack/USBInterface.hpp>      // For USB interface functions (AVR/ESP32 independent)

#include <common/esp_print.h>

// Includes for the possible target states
#include "HIDStateSendtranslation.hpp"

void HIDStateSendkeys::apply() {
    // Failsafe check to ensure that the context and data are set
    if (!context || !m_data || !context->getHIDEventBufferKeys()) {
        ESP_WARN("Context, data or eventbuffer not set");
        return;
    }

    // If a new HID report is not due, stay in the current state.
    // TODO - Add the 'isReady' check here to avoid sending data if the USB stack is not ready.
    if (!isNewHidReportDue()) {
        return;
    }

    // If no data is staged for sending, go to the next state.
    if (context->getHIDEventBufferKeys()->isStaged()) {
        USBSendReport(REPORTID_KEYS, context->getHIDEventBufferKeys()->getStaged(), HIDKEYDATASIZE); // Send new keys values to the Host
        context->getHIDEventBufferKeys()->clearStaged();                                             // Clear the staged keys data

        m_data->lastHIDsentRep += HIDUPDATERATE_MS;
        // REMOVE m_data->hasSentNewData = true;
    }

    context->setState(new HIDStateSendtranslation());
    return;
}