#pragma once

#include "hidhandler/sender/HIDSender.hpp"                  // for HIDSender
#include "hidhandler/usbinterface/SpaceMouseUSBInterface.h" // for SpaceMouseUSBInterface
#include "observers/HIDEventBuffer.hpp"                     // for HID_MESSAGE_SIZE

/**
 * @brief   Class is responsible for sending translation data to the USB HID interface of the connected computer.
 * @details It inherits from the HIDSender class and implements the sendData method to send the translation data.
 *          The class uses the SpaceMouseUSBInterface to send the data.
 */
class HIDSenderTranslation : public HIDSender {

public:
    HIDSenderTranslation(const uint8_t *msg) : HIDSender(msg) {} // Corrected constructor name
    virtual ~HIDSenderTranslation() = default;                   // Default destructor

    virtual void sendData() override {
        SpaceMouseUSBInterface_ *usbInterface = SpaceMouseUSBInterface_::getInstance(); // Get the USB interface instance
        usbInterface->SendReport(REPORTID_TRANS, getMessage(), HID_MESSAGE_SIZE);       // send new translational values
    }
};