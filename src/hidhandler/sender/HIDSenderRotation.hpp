#pragma once

#include "hidhandler/sender/HIDSender.hpp" // for HIDSender
// REMOVE #include "hidhandler/usbinterface/SpaceMouseUSBInterface.h" // for SpaceMouseUSBInterface
#include "usbstack/USBInterface.hpp"
#include "usbstack/HIDReportDescriptor.h" // for USBStart#include <observers/HIDEventBuffer/HIDEventBuffer.hpp> // for HID_MESSAGE_SIZE

/**
 * @brief Class is responsible for sending rotation data to the USB HID interface of the connected computer.
 *        It inherits from the HIDSender class and implements the sendData method to send the rotation data.
 *        The class uses the SpaceMouseUSBInterface to send the data.
 */
class HIDSenderRotation : public HIDSender {

public:
    HIDSenderRotation(const uint8_t *msg) : HIDSender(msg) {} // Corrected constructor name
    virtual ~HIDSenderRotation() = default;                   // Default destructor

    void sendData() override final {
// Send new rotational values
#if 0
        SpaceMouseUSBInterface_ *usbInterface = SpaceMouseUSBInterface_::getInstance(); // Get the USB interface instance
        usbInterface->SendReport(REPORTID_ROT, getMessage(), HID_MESSAGE_SIZE);         // send new rotational values
#endif
        USBSendReport(REPORTID_ROT, getMessage(), HID_MESSAGE_SIZE); // send new rotational values
    }
};