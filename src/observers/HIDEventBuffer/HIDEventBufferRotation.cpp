#include "HIDEventBufferRotation.hpp"
#include "axis/axes/AxisRotation.hpp"

void HIDEventBufferRotation::update(IObservable *axisRotation) {
    if (!axisRotation) {
        return;
    }

    const int16_t axisvalue = static_cast<AxisRotation *>(axisRotation)->getFinValue();

    // Offset the axisType to the first rotation axis, in order to use it as an index in the message buffer.
    // The first rotation axis is ROTX, which is 3 in the enum, so we have to subtract 3 from the axisType.
    // For every axis 2 bytes are used, and thus the index is multiplied by 2.
    const uint8_t i_msg = (static_cast<AxisRotation *>(axisRotation)->getAxisType() - AxisType_t::ROTX) * 2;

    updateMessage(axisvalue, i_msg);
}