#include "HIDStateCheckkeys.h"
#include <hidhandler/translator/TranslatorKeys.h> // for TranslatorKinematicsBase
#include <hidhandler/HIDHandlerController.h>      // For HIDHandlerController (context)

// Includes for the possible target states
#include "HIDStateSendkeys.h"
#include "HIDStateStart.h"

void HIDStateCheckKeys::apply() {
    Serial.println(F("HIDStateCheckKeys::apply()"));

    // Check if there is something to send.
    // If any of the keys has changed, proceed to the 'send keys' state, other wise go back to the start state.
    context->setState(new HIDStateStart()); // Set the next state to translation
#if 0                                       // REFACTOR
    if (static_cast<TranslatorKeys *>(translator)->hasStagedData()) {
        context->setState(new HIDStateSendkeys()); // Set the next state to send keys
    } else {
        context->setState(new HIDStateStart()); // Set the next state to translation

        if (isNewHidReportDue()) {
            // Keep the timestamp for the last sent package nearby.
            m_data->lastHIDsentRep = m_data->now - HIDUPDATERATE_MS;
        }
    }
#endif
};