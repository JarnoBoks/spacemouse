#include "HIDStateStart.hpp"
#include <hidhandler/HIDHandlerController.hpp> // For HIDHandlerController (context)

// Includes for the possible target states
#include "HIDStateSendtranslation.hpp"

void HIDStateStart::apply() {
    context->setState(new HIDStateSendtranslation());
};