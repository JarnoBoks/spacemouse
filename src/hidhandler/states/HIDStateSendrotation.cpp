#include "HIDStateSendRotation.hpp"
#include <hidhandler/HIDHandlerController.hpp>                 // For HIDHandlerController (context)
#include <observers/HIDEventBuffer/HIDEventBufferRotation.hpp> // For HIDEventBuffer

#include <usbstack/HIDReportDescriptor.h> // for ReportIDs
#include <usbstack/USBInterface.hpp>      // For USB interface functions (AVR/ESP32 independent)

#include <common/esp_print.h> // For ESP_PRINT and other print macros

// Includes for the possible target states
#include "HIDStateSendkeys.hpp"

void HIDStateSendrotation::apply() {

    // Failsafe check to ensure that the context and data are set
    if (!context || !m_data || !context->getHIDEventBufferRotation()) {
        ESP_WARN("Context, data or eventbuffer not set");
        return;
    }

    // If a new HID report is not due, stay in the current state.
    if (!(isNewHidReportDue() && USBReady)) {
        return;
    }

    // If rotation data is staged for sending or we didn't send 3 zero states.
    bool isStaged = context->getHIDEventBufferRotation()->isStaged(); // Check if there is staged rotation data
    if (isStaged || m_data->countRotZeros < 3) {
        USBSendReport(REPORTID_ROT, context->getHIDEventBufferRotation()->getStaged(), HID_MESSAGE_SIZE); // Send new rotation values to the Host
        context->getHIDEventBufferRotation()->clearStaged();                                              // Clear the staged rotation data

        // Increment or reset the zero counter.
        m_data->countRotZeros = (isStaged) ? 0 : m_data->countRotZeros + 1; // Increment the zero counter if rotation data is staged

        m_data->lastHIDsentRep += HIDUPDATERATE_MS;
        // REMOVE m_data->hasSentNewData = true;
    }

    context->setState(new HIDStateSendkeys()); // Set the next state to send keys
}