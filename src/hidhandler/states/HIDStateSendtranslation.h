#pragma once

#include "HIDStateBase.h"
#include <hidhandler/translator/TranslatorKinematicsTranslation.h> // for TranslatorKinematicsRotation

// Includes for the target states
#include "HIDStateSendrotation.h"

class HIDStateSendtranslation : public HIDStateBase {
protected:
public:
    HIDStateSendtranslation(HIDStateData *data) // Constructor to initialize the state data
        : HIDStateBase(data) {
        translator = new TranslatorKinematicsTranslation(); // Initialize the translator for translation data
    }
    ~HIDStateSendtranslation() {
        delete translator; // Clean up the translator instance
    }; // Destructor to clean up the observer instance

    void apply() override {
        if (!isNewHidReportDue()) {
            return; // if no new HID report is due, return
        }

#ifdef ADV_HID_JIGGLE
        jiggleValues(trans, toggleValue); // jiggle the non-zero values, if toggleValue is true
                                          // the toggleValue is toggled after sending the rotations, down below
#endif

        // Send new translational values
        translator->execute();

        // Increment or reset the zero counter
        data->countTransZeros = (static_cast<TranslatorKinematicsTranslation *>(translator)->isAllZeroValues()) ? data->countTransZeros + 1 : 0;

        data->lastHIDsentRep += HIDUPDATERATE_MS;
        data->hasSentNewData = true;
        context->setState(new HIDStateSendrotation(data)); // Set the next state to start
    }

    void report() override {
        // Report the current state of the axis observer
        // This function is empty because there is no need to report anything in this state.
    }
};