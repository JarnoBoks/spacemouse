#include "HIDStateStart.h"
#include <hidhandler/HIDHandlerController.h> // For HIDHandlerController (context)

// Includes for the possible target states
#include "HIDStateSendtranslation.h"

void HIDStateStart::apply() {
    context->setState(new HIDStateSendtranslation());
};