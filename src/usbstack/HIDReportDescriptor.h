#pragma once

#include "config.h"  // For ADV_HID_REL and ADV_HID_JIGGLE
#include <stdint.h>  // For uint8_t
#include <Arduino.h> // For PROGMEM

// NOTE - See https://github.com/hathach/tinyusb/pull/1363/files for extension.

// USB Device Identification
// !!  For Arduino the USB VID and PID for this emulated space mouse must be set in the boards.txt file when
// !!  using Arduino IDE or in 'set_hwids.py' when using platformIO.
#define SM_USB_VID 0x256f
#define SM_USB_PID 0xc631 // Vendor ID and Product ID for the SpaceMouse

// HID Report IDs
// These IDs are used to identify the different reports sent by the SpaceMouse to the host computer.
#define REPORTID_TRANS 0x01 // Report ID for the first report (translation)
#define REPORTID_ROT 0x02   // Report ID for the second report (rotation)
#define REPORTID_KEYS 0x03  // Report ID for the third report (keys)
#define REPORTID_LEDS 0x04  // Report ID for the fourth report (LEDs)

static const uint8_t desc_hid_report[] PROGMEM = {
    0x05, 0x01, // Usage Page (Generic Desktop)
    0x09, 0x08, // Usage (Multi-Axis)
    0xA1, 0x01, // Collection (Application)

    // Report 1: Translation
    0xa1, 0x00,       // Collection (Physical)
    0x85, 0x01,       // Report ID (1)
    0x16, 0xA2, 0xFE, // Logical Minimum (-350) (0xFEA2 in little-endian)
    0x26, 0x5E, 0x01, // Logical Maximum (350) (0x015E in little-endian)
    0x36, 0x88, 0xFA, // Physical Minimum (-1400) (0xFA88 in little-endian)
    0x46, 0x78, 0x05, // Physical Maximum (1400) (0x0578 in little-endian)
    0x09, 0x30,       // Usage (X)
    0x09, 0x31,       // Usage (Y)
    0x09, 0x32,       // Usage (Z)
    0x75, 0x10,       // Report Size (16)
    0x95, 0x03,       // Report Count (3)
#ifdef ADV_HID_REL    // see Advanced HID settings in config_sample.h
    0x81, 0x06,       //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
#else
    0x81, 0x02, // Input (variable,absolute)
#endif
    0xC0, // End Collection

    // Report 2: Rotation
    0xa1, 0x00,       // Collection (Physical)
    0x85, 0x02,       // Report ID (2)
    0x16, 0xA2, 0xFE, // Logical Minimum (-350)
    0x26, 0x5E, 0x01, // Logical Maximum (350)
    0x36, 0x88, 0xFA, // Physical Minimum (-1400)
    0x46, 0x78, 0x05, // Physical Maximum (1400)
    0x09, 0x33,       // Usage (RX)
    0x09, 0x34,       // Usage (RY)
    0x09, 0x35,       // Usage (RZ)
    0x75, 0x10,       // Report Size (16)
    0x95, 0x03,       // Report Count (3)
#ifdef ADV_HID_REL    // see Advanced HID settings in config_sample.h
    0x81, 0x06,       //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
#else
    0x81, 0x02, // Input (variable,absolute)
#endif
    0xC0, // End Collection

    // Report 3: Keys  // find #define HIDMAXBUTTONS 32 in config_sample.h
    0xa1, 0x00,          // Collection (Physical)
    0x85, 0x03,          //  Report ID (3)
    0x15, 0x00,          //   Logical Minimum (0)
    0x25, 0x01,          //    Logical Maximum (1)
    0x75, 0x01,          //    Report Size (1)
    0x95, HIDMAXBUTTONS, //    Report Count (32)
    0x05, 0x09,          //    Usage Page (Button)
    0x19, 1,             //    Usage Minimum (Button #1)
    0x29, HIDMAXBUTTONS, //    Usage Maximum (Button #24)
    0x81, 0x02,          //    Input (variable,absolute)
    0xC0,                // End Collection

    // Report 4: LEDs
    0xA1, 0x02, //   Collection (Logical)
    0x85, 0x04, //     Report ID (4)
    0x05, 0x08, //     Usage Page (LEDs)
    0x09, 0x4B, //     Usage (Generic Indicator)
    0x15, 0x00, //     Logical Minimum (0)
    0x25, 0x01, //     Logical Maximum (1)
    0x95, 0x01, //     Report Count (1)
    0x75, 0x01, //     Report Size (1)
    0x91, 0x02, //     Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x95, 0x01, //     Report Count (1)
    0x75, 0x07, //     Report Size (7)
    0x91, 0x03, //     Output (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,       //   End Collection
    0xc0        // END_COLLECTION
};