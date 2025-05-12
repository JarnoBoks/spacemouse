#include "HIDStateStart.h"
#include <hidhandler/HIDHandlerController.h>                      // For HIDHandlerController (context)
#include <observers/HIDEventBuffer/HIDEventBufferTranslation.hpp> // For HIDEventBuffer
#include <observers/HIDEventBuffer/HIDEventBufferRotation.hpp>    // For HIDEventBuffer

// Includes for the possible target states
#include "HIDStateSendtranslation.h"
#include "HIDStateCheckkeys.h"

void HIDStateStart::apply() {

    context->setState(new HIDStateSendtranslation());
#if 0
    Serial.print(F("HIDStateStart::apply()"));
    Serial.print(F(" - countTransZeros: "));
    Serial.print(m_data->countTransZeros);
    Serial.print(F(" - countRotZeros: "));
    Serial.print(m_data->countRotZeros);

    const bool stagedRot = context->getHIDEventBufferRotation()->isStaged();
    const bool stagedTrans = context->getHIDEventBufferTranslation()->isStaged();

    Serial.print(F(" - stagedRot: "));
    Serial.print(stagedRot);
    Serial.print(F(" - stagedTrans: "));
    Serial.println(stagedTrans);

    // Are there zero data packages to send (have to send 3 in total) or do any of the axes have movement
    if (stagedRot || stagedTrans || m_data->countTransZeros < 3 || m_data->countRotZeros < 3) {
        context->setState(new HIDStateSendtranslation());
    } else {
        // Maybe the keys have changed, so check for that in the next stage as well.
        context->setState(new HIDStateCheckKeys()); // Set the next state to send keys
    }
#endif
};