#pragma once

#include "SpaceMouseHIDActionsSendBase.h" // for ISpaceMouseHIDActions
#include "../SpaceMouseTranslatorMovement.h"

#include "config.h" // for HIDMAXBUTTONS
#include <stdint.h> // for uint8_t
typedef uint8_t byte;
constexpr uint8_t KEYDATASIZE = HIDMAXBUTTONS / 8; // Size of the key data array

class SpaceMouseHIDTranslation : public SpaceMouseHIDActionsSendBase {
private:
    uint8_t countTransZeros = 0;

public:
    // SpaceMouse HID keys
    SpaceMouseHIDTranslation(SpaceMouseTranslatorTranslation *translator)
        : SpaceMouseHIDActionsSendBase(translator),
          countTransZeros(0) {};
    virtual ~SpaceMouseHIDTranslation() = default; // Default destructor

    void execute() {

#ifdef ADV_HID_JIGGLE
        jiggleValues(trans, toggleValue); // jiggle the non-zero values, if toggleValue is true
                                          // the toggleValue is toggled after sending the rotations, down below
#endif
        // Send new translational values

        translator->execute();                                                       // Execute the translator to get the key state
        countTransZeros = (translator->isAllZeroValues()) ? countTransZeros + 1 : 0; // increment or reset the zero counter
    };
};