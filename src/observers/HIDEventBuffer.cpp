#include "HIDEventBuffer.hpp"
#include "key/KeyCollection.hpp"
#include "axis/axes/AxisRotation.hpp"
#include "axis/axes/AxisTranslation.hpp"

void HIDEventBuffer::update(KeyCollection *keyCollection) {
    // Update the key data for the HID event buffer
    // m_keyData = keyCollection->getKeyData();
};

/// @details This function is called when the Rotation axis data has updated.
void HIDEventBuffer::update(AxisRotation *axisRotation) {

    const int16_t axisvalue = axisRotation->getFinValue();

    // Offset the axisType to the first rotational axis, in order to use it as an index in the message buffer.
    // The first rotational axis is ROTX, which is 3 in the enum, so we have to subtract 3 from the axisType.
    // For every axis 2 bytes are used, and thus the index is multiplied by 2.
    const uint8_t i_msg = (axisRotation->getAxisType() - AxisType_t::ROTX) * 2;

    m_trans_staged[i_msg] = (byte)axisvalue & 0xFF;     // Store the lower byte of the axis value
    m_trans_staged[i_msg + 1] = (byte)(axisvalue >> 8); // Store the upper byte of the velocity
};

/// @details This function is called when the Translation axis data has updated.
void HIDEventBuffer::update(AxisTranslation *axisTranslation) {

    const int16_t axisvalue = axisTranslation->getFinValue();

    // Offset the axisType to the first translation axis, in order to use it as an index in the message buffer.
    // The first translation axis is TRANSX, which is 0 in the enum, so we have to subtract 0 from the axisType.
    // For every axis 2 bytes are used, and thus the index is multiplied by 2.
    const uint8_t i_msg = (axisTranslation->getAxisType() - AxisType_t::TRANSX) * 2;

    m_trans_staged[i_msg] = (byte)axisvalue & 0xFF;     // Store the lower byte of the axis value
    m_trans_staged[i_msg + 1] = (byte)(axisvalue >> 8); // Store the upper byte of the velocity
};