#pragma once

#if defined(ARDUINO_ARCH_ESP32)

#include <stdint.h>

class Adafruit_USBD_HID;

class USBStack {
private:
    static USBStack *m_instance;

    Adafruit_USBD_HID *m_usb_hid = nullptr; // USB HID interface

    USBStack();                                                  // Delete default constructor
    USBStack(Adafruit_USBD_HID *usb_hid) : m_usb_hid(usb_hid) {} // Constructor to initialize the USB HID interface
public:
    static USBStack *getInstance() {
        if (!m_instance) {
            m_instance = new USBStack();
        }
        return m_instance;
    }

    ~USBStack() {} // Destructor

    void setup_USB();
    bool SendReport(uint8_t id, const void *data, int len);
};

#endif // ARDUINO_ARCH_ESP32