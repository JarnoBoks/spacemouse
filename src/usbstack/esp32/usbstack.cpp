
#if defined(ARDUINO_ARCH_ESP32)

#include "usbstack.hpp"
#include "usbstack/HIDReportDescriptor.h"
#include "Adafruit_TinyUSB.h"

USBStack *USBStack::m_instance = nullptr;

USBStack::USBStack() {
}

void USBStack::setup_USB() {
    // Initialize the USB stack
    TinyUSBDevice.setID(SM_USB_VID, SM_USB_PID); // TODO - set_hwids.py can be removed for ESP32, setting the VID/PID should be done in the code
    TinyUSBDevice.setManufacturerDescriptor("Printables");
    TinyUSBDevice.setProductDescriptor("CAD Mouse / SpaceMouse");

#if 0
// REVIEW - Is this necessary for ESP32? Does it damage the connection? If not damaging, keep it in the code for consistency with other platforms.
        // Manual begin() is required on core without built-in support e.g. mbed rp2040
        if (!TinyUSBDevice.isInitialized()) {
            TinyUSBDevice.begin(0);
        }
#endif

    // Set up HID
    m_usb_hid.setPollInterval(2);
    m_usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
    m_usb_hid.setStringDescriptor("TinyUSB HID Composite");
    m_usb_hid.begin();

    // If already enumerated, additional class driver begin() e.g msc, hid, midi won't take effect until re-enumeration
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

    bool ret = m_usb_hid.sendReport(id, data, len);
    if (!ret) {
        Serial.println(F("USBStack::SendReport failed"));
    }
    return ret;
}

void USBStack::process_USB() {
// TODO - Remote wakeup (see example in Adafruit_TinyUSB_Library/examples/HID/hid_composite/hid_composite.ino)
#if 0
    // Remote wakeup
    if (TinyUSBDevice.suspended() && // SOMETHING TO DO) {
        // Wake up host if we are in suspend mode
        // and REMOTE_WAKEUP feature is enabled by host
        TinyUSBDevice.remoteWakeup();
    }
#endif
    bool ret = false;
    if (m_usb_hid.ready()) {
        // Process the HID reports
        // REMOVE ret = m_usb_hid.mouseMove(RID_MOUSE, 5, 5); // Example: move mouse right + down

        // REMOVE ret = tud_hid_n_mouse_report(RID_MOUSE, 5, 5, 0, 0, 0); // Example: move mouse right + down
    }

    // Process the USB stack
    // TinyUSBDevice.task();

    // Call the HID task to process any incoming reports
    // m_usb_hid.task();
}

#endif // ARDUINO_ARCH_ESP32