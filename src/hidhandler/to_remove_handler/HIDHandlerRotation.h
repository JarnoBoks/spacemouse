#pragma once

#include "HIDHandlerBase.h"                   // for HIDHandlerBase
#include <hid/TranslatorKinematicsRotation.h> // for TranslatorKinematicsRotation
#include <stdint.h>                           // for uint8_t

class HIDHandlerRotation : public HIDHandlerBase {
private:
    uint8_t countRotZeros = 0;

public:
    // SpaceMouse HID keys
    HIDHandlerRotation(TranslatorKinematicsRotation *translator)
        : HIDHandlerBase(translator),
          countRotZeros(0) {};
    virtual ~HIDHandlerRotation() = default; // Default destructor

    void execute() {

#ifdef ADV_HID_JIGGLE
        jiggleValues(rot, toggleValue); // jiggle the non-zero values, if toggleValue is true
        toggleValue ^= true;            // toggle the indicator to jiggle only every second report send
#endif

        translator->execute();
        countRotZeros = (static_cast<TranslatorKinematicsRotation *>(translator)->isAllZeroValues()) ? countRotZeros + 1 : 0; // increment or reset the zero counter
    };
};