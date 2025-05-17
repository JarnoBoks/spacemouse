#include "HIDEventBufferTranslation.hpp"
#include "axis/axes/AxisTranslation.hpp"

void HIDEventBufferTranslation::update(IObservable *axisTranslation) {
    if (!axisTranslation) {
        return;
    }

    const int16_t axisvalue = static_cast<AxisTranslation *>(axisTranslation)->getFinValue();

    // Offset the axisType to the first translation axis, in order to use it as an index in the message buffer.
    // The first translation axis is TRANSX, which is 0 in the enum, so we have to subtract 0 from the axisType.
    // For every axis 2 bytes are used, and thus the index is multiplied by 2.
    const uint8_t i_msg = (static_cast<AxisTranslation *>(axisTranslation)->getAxisType() - AxisType_t::TRANSX) * 2;

    updateMessage(axisvalue, i_msg);
}