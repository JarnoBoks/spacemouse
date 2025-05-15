#pragma once

#ifdef ARDUINO_ARCH_AVR
#include "usbastack/arduino/hidusbstack.hpp"

#define USBStart HIDUSBStack::getInstance()
#define USBSendReport(x, y, z) HIDUSBStack::getInstance()->SendReport(x, y, z)
#endif

#ifdef ARDUINO_ARCH_ESP32
#include "usbstack/esp32/usbstack.hpp"

#define USBStart USBStack::getInstance()->setup_USB()
#define USBSendReport(x, y, z) USBStack::getInstance()->SendReport(x, y, z)
#endif