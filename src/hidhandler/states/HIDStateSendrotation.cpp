#pragma once
#include "HIDStateSendRotation.h"
#include <hidhandler/translator/TranslatorKinematicsRotation.h> // for TranslatorKinematicsRotation
#include <hidhandler/HIDHandlerController.h>                    // For HIDHandlerController (context)

#include "config.h" // for NUMKEYS

// Includes for the target states
#include "HIDStateStart.h"
#include "HIDStateSendkeys.h"

HIDStateSendrotation::HIDStateSendrotation(HIDStateData *data) // Constructor to initialize the state data
    : HIDStateBase(data) {
    translator = new TranslatorKinematicsRotation(); // Initialize the translator for rotation data
}
HIDStateSendrotation::~HIDStateSendrotation() {
    delete translator;
}

void HIDStateSendrotation::apply() {
    if (!isNewHidReportDue()) {
        return;
    }

    translator->execute();

    // Increment or reset the zero counter
    data->countRotZeros = (static_cast<TranslatorKinematicsRotation *>(translator)->isAllZeroValues()) ? data->countRotZeros + 1 : 0;

    data->lastHIDsentRep += HIDUPDATERATE_MS;
    data->hasSentNewData = true;

// check if the next state should be keys
#if (NUMKEYS > 0)
    if (memcmp(keyData, prevKeyData, HIDMAXBUTTONS / 8) != 0)
    // compare key data to previous key data
    {
        context->setState(new SpaceMouseHIDStateSendkeys(data)); // Set the next state to start
    } else {
        // go back to start
        context->setState(new SpaceMouseHIDStateStart(data)); // Set the next state to start
    }
#else
    // if no keys are used, go to start state after rotations
    context->setState(new HIDStateStart(data)); // Set the next state to start
#endif
}
