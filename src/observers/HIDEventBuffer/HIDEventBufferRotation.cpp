#include "HIDEventBufferRotation.hpp"
#include <kinematics/axis/KinematicsAxis.hpp>

void HIDEventBufferRotation::update(IObservable *kinVectorRotation) {
    if (!kinVectorRotation) {
        return;
    }

    const int16_t axisvalue = static_cast<KinematicsAxis *>(kinVectorRotation)->getFinValue(); // Final value of the rotation axis.

    // Offset the axisType to the first rotation axis, in order to use it as an index in the rotation message buffer.
    // The first rotation axis is ROTX, so we have to subtract it from the axisType to retrieve the index.
    // For every axis 2 bytes are used, and thus the index is multiplied by 2.
    const uint8_t idx = (static_cast<KinematicsAxis *>(kinVectorRotation)->getType() - MotionVector_t::ROTX) * 2; // Message index for the rotation axis.

    updateMessage(axisvalue, idx);
}