#pragma once

/*
Not observable by the HID Event buffer -> that will observe the KinematicsMotionVectorCollections (TRANS collection and ROT collection)
*/
#include <base/axis/AxisBase.hpp>

class KnobAxis;

/// @brief Number of observers that can be added to KinematicsAxis.
/// @details This is a constant value that defines the maximum number of observers that can be added to the KinematicsAxis.
/// TODO @note This value is set to 1, as the KinematicsAxis is only having the HID Event buffer as observer.
constexpr uint8_t c_KIN_MAX_AXIS_OBSERVERS = 1;

/**
 * @brief Base class for Kinematics Axis.
 * @details This class inherits from AxisBase and provides functionality for managing the Kinematics Axis configuration and state.
 */
class KinematicsAxis : public AxisBase {
private:
    const KnobAxis *m_knobAxis = nullptr; // Pointer to the knob Axis

public:
    KinematicsAxis() = delete; // Delete default constructor, do not allow instantiation without parameters

    KinematicsAxis(MotionVector_t type, KnobAxis *knobAxis)
        : AxisBase(type, c_KIN_MAX_AXIS_OBSERVERS),
          m_knobAxis(knobAxis) {}

    ~KinematicsAxis() = default;

    void evaluate() override;
};