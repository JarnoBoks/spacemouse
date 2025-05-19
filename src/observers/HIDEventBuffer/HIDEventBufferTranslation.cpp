#include "HIDEventBufferTranslation.hpp"
#include <axis/axes/KnobTranslation.hpp>

void HIDEventBufferTranslation::update(IObservable *axisTranslation) {
    if (!axisTranslation) {
        return;
    }

    const int16_t axisvalue = static_cast<KnobTranslation *>(axisTranslation)->getFinValue(); // Final value of the translation axis.

    // Offset the axisType to the first translation axis, in order to use it as an index in the message buffer.
    // The first translation axis is TRANSX,so we have to subtract it from the axisType to retrieve the index.
    // For every axis 2 bytes are used, and thus the index is multiplied by 2.
    const uint8_t idx = (static_cast<KnobTranslation *>(axisTranslation)->getAxisType() - MotionVector_t::TRANSX) * 2; // Message index for the translation axis.

    updateMessage(axisvalue, idx);
}