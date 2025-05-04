#include "HIDStateStart.h"
#include <hidhandler/translator/TranslatorKinematicsBase.h> // for TranslatorKinematicsBase
#include <hidhandler/HIDHandlerController.h>                // For HIDHandlerController (context)

// Includes for the possible target states
#include "HIDStateSendtranslation.h"
#include "HIDStateCheckkeys.h"

HIDStateStart::HIDStateStart() {
    translator = new TranslatorKinematicsBase(); // Initialize the translator for translation data
}

HIDStateStart::~HIDStateStart() {
    delete translator; // Clean up the translator instance
};

void HIDStateStart::apply() {

    // Check if there is kinematics data to be send:
    // Are there zero data packages to send (have to send 3 in total) or do any of the axes have movement
    if (data->countTransZeros < 3 || data->countRotZeros < 3 || static_cast<TranslatorKinematicsBase *>(translator)->areAllAxisZero()) {
        context->setState(new HIDStateSendtranslation());

    } else {

        // Maybe the keys have changed, so check for that in the next stage as well.
        context->setState(new HIDStateCheckKeys()); // Set the next state to send keys
    }
};