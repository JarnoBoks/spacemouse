#pragma once

#include "hidhandler/sender/HIDSender.hpp" // for HIDSender
// REMOVE #include "hidhandler/usbinterface/SpaceMouseUSBInterface.h" // for SpaceMouseUSBInterface
#include "usbstack/USBInterface.hpp"
#include "usbstack/HIDReportDescriptor.h"              // for USBStart
#include <observers/HIDEventBuffer/HIDEventBuffer.hpp> // for HID_MESSAGE_SIZE

/**
 * @brief   Class is responsible for sending translation data to the USB HID interface of the connected computer.
 * @details It inherits from the HIDSender class and implements the sendData method to send the translation data.
 *          The class uses the SpaceMouseUSBInterface to send the data.
 */
class HIDSenderTranslation : public HIDSender {

public:
    HIDSenderTranslation(const uint8_t *msg) : HIDSender(msg) {} // Corrected constructor name
    virtual ~HIDSenderTranslation() = default;                   // Default destructor

    void sendData() override final {
#if 0
        SpaceMouseUSBInterface_ *usbInterface = SpaceMouseUSBInterface_::getInstance(); // Get the USB interface instance
        usbInterface->SendReport(REPORTID_TRANS, getMessage(), HID_MESSAGE_SIZE);       // send new translational values
#endif
        USBSendReport(REPORTID_TRANS, getMessage(), HID_MESSAGE_SIZE); // send new rotational values
    }
};