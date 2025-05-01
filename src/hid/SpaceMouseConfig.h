#include "config.h"

// Send a HID report every 8 ms (125 Hz)
#define HIDUPDATERATE_MS 8

#ifndef HIDMAXBUTTONS
#define HIDMAXBUTTONS 32 // must be multiple of 8!
#endif