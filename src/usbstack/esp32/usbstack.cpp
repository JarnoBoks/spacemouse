#ifdef ARDUINO_ARCH_ESP32

#include "usbstack.hpp"
#include "usbstack/HIDReportDescriptor.h"
#include "Adafruit_TinyUSB.h"

USBStack *USBStack::m_instance = nullptr;

USBStack::USBStack() {
    m_usb_hid = new Adafruit_USBD_HID(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 2, false);
}

void USBStack::setup_USB() {
    // Initialize the USB stack
    TinyUSBDevice.setID(SM_USB_VID, SM_USB_PID);
    TinyUSBDevice.setManufacturerDescriptor("Printables");
    TinyUSBDevice.setProductDescriptor("CAD Mouse / SpaceMouse");
    TinyUSBDevice.begin();

    m_usb_hid->begin();

    // If already enumerated, additional class driverr begin() e.g msc, hid, midi won't take effect until re-enumeration
    if (TinyUSBDevice.mounted()) {
        TinyUSBDevice.detach();
        delay(10);
        TinyUSBDevice.attach();
    }
}

/**
 * @brief   Send a HID Message to the USB stack.
 * @details This function sends a part of the HID report to the USB stack. It is used to send data
 *          to the host. The report descriptor defines the format of the message. This descriptor
 *          contains 4 parts: translation, rotation, keys and LEDs.
 *          The report partID is used to identify the part of the report that is being sent the
 *          report partID will be sent to the host as part of the HID report too.
 * @param id    Indicating the part of the data to be sent
 * @param data  Pointer to the data array
 * @param len   Length of the data array
 *
 * @return true if the data was sent successfully, false otherwise
 */
bool USBStack::SendReport(uint8_t id, const void *data, int len) {
    bool ret = m_usb_hid->sendReport(0, &id, 1);

    if (ret) {
        ret = m_usb_hid->sendReport(id, data, len);
    }

    // TODO - REMOVE
    if (!ret) {
        Serial.println(F("USBStack::SendReport failed"));
    }
    return ret;
}

#endif // ARDUINO_ARCH_ESP32