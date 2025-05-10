#include "HIDStateStart.h"
// REMOVE #include <hidhandler/translator/TranslatorKinematicsBase.h> // for TranslatorKinematicsBase
#include <hidhandler/HIDHandlerController.h> // For HIDHandlerController (context)
#include <observers/HIDEventBuffer.hpp>      // For HIDEventBuffer

// Includes for the possible target states
#include "HIDStateSendtranslation.h"
#include "HIDStateCheckkeys.h"

HIDStateStart::HIDStateStart() {
    // REMOVE translator = new TranslatorKinematicsBase(); // Initialize the translator for translation data
}

HIDStateStart::~HIDStateStart() {
    // REMOVE delete translator;
};

void HIDStateStart::apply() {

    // Are there zero data packages to send (have to send 3 in total) or do any of the axes have movement
    if (context->getHIDEventBuffer()->isRotationStaged() || context->getHIDEventBuffer()->isTranslationStaged() || data->countTransZeros < 3 || data->countRotZeros < 3) {
        context->setState(new HIDStateSendtranslation());

    } else {

        // Maybe the keys have changed, so check for that in the next stage as well.
        context->setState(new HIDStateCheckKeys()); // Set the next state to send keys
    }
};