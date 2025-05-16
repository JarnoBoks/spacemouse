#pragma once

#ifdef ARDUINO_AVR_MICRO
#include "usbstack/arduino/hidusbstack.hpp"

#define USBStart HidUSBStack::getInstance()
#define USBSendReport(x, y, z) HidUSBStack::getInstance()->SendReport(x, y, z)
#endif

#if defined(ARDUINO_ARCH_ESP32)
#include "usbstack/esp32/usbstack.hpp"

#define USBStart USBStack::getInstance()->setup_USB()
#define USBSendReport(x, y, z) USBStack::getInstance()->SendReport(x, y, z)
#endif

#if defined(ARDUINO_AVR_ATmega2560)
#define USBStart
#define USBSendReport(x, y, z)
#endif