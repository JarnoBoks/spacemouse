#pragma once

/**
 * @brief USBInterface
 * @details This class provides an interface for USB communication.
 *          It is designed to be used with different architectures (AVR and ESP32).
 *          The class provides methods to set up USB communication and send reports.
 *
 *          For AVR architecture, it uses the HidUSBStack class.
 *          For ESP32 architecture, it uses the USBStack class.
 *          For ATmega2560 architecture, it does not implement any USB functionality.
 */
#ifdef ARDUINO_AVR_MICRO
// Use the implementation of the HIDUSBStack class for Arduino Pro Micro
#include "usbstack/arduino/hidusbstack.hpp"

#define USBStart HidUSBStack::getInstance()
#define USBSendReport(x, y, z) HidUSBStack::getInstance()->SendReport(x, y, z)
#endif

#if defined(ARDUINO_ARCH_ESP32)
// Use the implementation of the USBStack class (TinyUSB) for ESP32
#include "usbstack/esp32/usbstack.hpp"

#define USBStart USBStack::getInstance()->setup_USB()
#define USBSendReport(x, y, z) USBStack::getInstance()->SendReport(x, y, z)
#endif

#if defined(ARDUINO_AVR_ATmega2560)
// USB Not supported for ATmega2560
#define USBStart
#define USBSendReport(x, y, z)
#endif