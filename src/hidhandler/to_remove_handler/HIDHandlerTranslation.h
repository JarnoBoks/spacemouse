#pragma once

#include "HIDHandlerBase.h"                      // for HIDHandlerBase
#include <hid/TranslatorKinematicsTranslation.h> // for TranslatorKinematicsTranslation
#include <stdint.h>                              // for uint8_t

class HIDHandlerTranslation : public HIDHandlerBase {
private:
    uint8_t countTransZeros = 0;

public:
    // SpaceMouse HID keys
    HIDHandlerTranslation(TranslatorKinematicsTranslation *translator)
        : HIDHandlerBase(translator),
          countTransZeros(0) {};
    virtual ~HIDHandlerTranslation() = default; // Default destructor

    void execute() {

#ifdef ADV_HID_JIGGLE
        jiggleValues(trans, toggleValue); // jiggle the non-zero values, if toggleValue is true
                                          // the toggleValue is toggled after sending the rotations, down below
#endif
        // Send new translational values

        translator->execute();                                                                                                       // Execute the translator to get the key state
        countTransZeros = (static_cast<TranslatorKinematicsTranslation *>(translator)->isAllZeroValues()) ? countTransZeros + 1 : 0; // increment or reset the zero counter
    };
};