#pragma once
#include "config.h"
#include <stdint.h> // for uint8_t

// Send a HID report every 8 ms (125 Hz)
#ifndef HIDUPDATERATE_MS
#define HIDUPDATERATE_MS 8
#endif

#ifndef HIDMAXBUTTONS
#define HIDMAXBUTTONS 32 // must be multiple of 8!
#endif

constexpr uint8_t KEYDATASIZE = HIDMAXBUTTONS / 8; // Size of the key data array