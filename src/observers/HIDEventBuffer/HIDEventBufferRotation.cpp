#include "HIDEventBufferRotation.hpp"
#include <axis/axes/KnobRotation.hpp>

void HIDEventBufferRotation::update(IObservable *axisRotation) {
    if (!axisRotation) {
        return;
    }

    const int16_t axisvalue = static_cast<KnobRotation *>(axisRotation)->getFinValue(); // Final value of the rotation axis.

    // Offset the axisType to the first rotation axis, in order to use it as an index in the rotation message buffer.
    // The first rotation axis is ROTX, so we have to subtract it from the axisType to retrieve the index.
    // For every axis 2 bytes are used, and thus the index is multiplied by 2.
    const uint8_t idx = (static_cast<KnobRotation *>(axisRotation)->getAxisType() - MotionVector_t::ROTX) * 2; // Message index for the rotation axis.

    updateMessage(axisvalue, idx);
}