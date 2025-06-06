#include "HIDStateSendTranslation.hpp"
#include <hidhandler/HIDHandlerController.hpp>                    // For HIDHandlerController (context)
#include <observers/HIDEventBuffer/HIDEventBufferTranslation.hpp> // For HIDEventBuffer

#include <usbstack/HIDReportDescriptor.h> // for ReportIDs
#include <usbstack/USBInterface.hpp>      // For USB interface functions (AVR/ESP32 independent)

#include <common/esp_print.h> // For ESP_PRINT and other print macros

// Includes for the possible target states
#include "HIDStateSendrotation.hpp"

void HIDStateSendtranslation::apply() {

    // Failsafe check to ensure that the context and data are set
    if (!context || !m_data || !context->getHIDEventBufferTranslation()) {
        ESP_WARN("Context, data or eventbuffer not set");
        return;
    }

    // If a new HID report is not due, stay in the current state.
    // TODO - Add the 'isReady' check here to avoid sending data if the USB stack is not ready.
    if (!isNewHidReportDue()) {
        return;
    }

    // If translation data is staged for sending or we didn't send 3 zero states.
    bool isStaged = context->getHIDEventBufferTranslation()->isStaged(); // Check if there is staged translation data
    if (isStaged || m_data->countTransZeros < 3) {
        USBSendReport(REPORTID_TRANS, context->getHIDEventBufferTranslation()->getStaged(), HID_MESSAGE_SIZE); // Send new translation values to the Host
        context->getHIDEventBufferTranslation()->clearStaged();                                                // Clear the staged translation data

        // Increment or reset the zero counter.
        m_data->countTransZeros = (isStaged) ? 0 : m_data->countTransZeros + 1; // Increment the zero counter if translation data is staged

        m_data->lastHIDsentRep += HIDUPDATERATE_MS;
        // REMOVE m_data->hasSentNewData = true;
    }

    context->setState(new HIDStateSendrotation()); // Set the next state to send rotation
}

// TODO - Add JIGGLE functionality for the translation state.
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
