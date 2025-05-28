#pragma once

#include "hidhandler/sender/HIDSender.hpp" // for HIDSender
#include "usbstack/USBInterface.hpp"
#include "usbstack/HIDReportDescriptor.h"              // for USBStart
#include <observers/HIDEventBuffer/HIDEventBuffer.hpp> // for HID_MESSAGE_SIZE
#include "hidhandler/HIDHandlerConfig.h"               // for HIDKEYDATASIZE

/**
 * @brief   Class is responsible for sending translation data to the USB HID interface of the connected computer.
 * @details It inherits from the HIDSender class and implements the sendData method to send the translation data.
 *          The class uses the SpaceMouseUSBInterface to send the data.
 * @deprecated
 */
class HIDSenderKeys : public HIDSender {

public:
    HIDSenderKeys(const uint8_t *msg) : HIDSender(msg) {} // Corrected constructor name
    virtual ~HIDSenderKeys() = default;                   // Default destructor

    void sendData() override final {
        USBSendReport(REPORTID_KEYS, getMessage(), HIDKEYDATASIZE); // send new keys values
    }
};