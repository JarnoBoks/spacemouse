#pragma once

#include "SpaceMouseHIDActionsSendBase.h" // for ISpaceMouseHIDActions
#include "../SpaceMouseUSBInterface.h"
#include "config.h" // for HIDMAXBUTTONS
#include <stdint.h> // for uint8_t

typedef uint8_t byte;
constexpr uint8_t KEYDATASIZE = HIDMAXBUTTONS / 8; // Size of the key data array

class SpaceMouseHIDRotation : public SpaceMouseHIDActionsSendBase {
    // This class is used to handle the debug parameter for axis information.
    // It inherits from ISpaceMouseHIDState and implements the apply and report methods.
    // The apply method is used to apply the changes to the axis observer.
    // The report method is used to report the current state of the axis observer.
private:
    uint8_t countRotZeros = 0;

public:
    // SpaceMouse HID keys
    SpaceMouseHIDRotation(SpaceMouseTranslatorRotation *translator)
        : SpaceMouseHIDActionsSendBase(translator),
          countRotZeros(0) {};
    virtual ~SpaceMouseHIDRotation() = default; // Default destructor

    void execute() {

#ifdef ADV_HID_JIGGLE
        jiggleValues(rot, toggleValue); // jiggle the non-zero values, if toggleValue is true
        toggleValue ^= true;            // toggle the indicator to jiggle only every second report send
#endif

        translator->execute();                                                   // Execute the translator to get the key state
        countRotZeros = (translator->isAllZeroValues()) ? countRotZeros + 1 : 0; // increment or reset the zero counter
    };
};