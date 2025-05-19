#pragma once
#include "common/Collection.hpp" // Include the ICollection interface header file
#include "common/Observable.hpp" // Include the ICollection interface header file

#include <Knob/MotionVector/IMotionVector.hpp>
#include <Knob/MotionVectorType.h> // Include the header file for MotionVector_t enum

#include <kinematics/MotionVectors/config/KinematicsMVConfig.hpp>

/// @brief Number of observers that can be added to a kinematics MotionVector.
/// @details This is a constant value that defines the maximum number of observers that can be added to the kinematics MotionVector.
/// TODO @note This value is set to 1, as the kinematics MotionVector is only having the HID Event buffer as observer.
constexpr uint8_t c_KIN_MAX_MOTIONVECTOR_OBSERVERS = 1;

class KinematicsMotionVector : public ICollectable, public Observable {
private:
    const char *m_descriptor = nullptr;     // Descriptor of the knob MotionVector
    KinematicsMVConfig *m_Config = nullptr; // Pointer to the configuration object of the MotionVector

    int16_t m_rawValue = 0; // Raw computed value for the kinematics MotionVector, used to store the value that is calculated by the SensorsCalculator
    int16_t m_snsValue = 0; // KnobMotionVector value after applying sensitivity

public:
    KinematicsMotionVector() = delete; // Delete default constructor, do not allow instantiation without parameters
    KinematicsMotionVector(MotionVector_t type)
        : Observable(c_KIN_MAX_MOTIONVECTOR_OBSERVERS),
          m_descriptor(c_MOTIONVECTOR_DESCRIPTORS[static_cast<int>(type)]), // Set the name of the axis based on the MotionVector_t enum
          m_Config(new KinematicsMVConfig(type)) {};                        // Create a new KnobVectorConfig object for this axis

    ~KinematicsMotionVector() { delete m_Config; } // Destructor (not used in normal SpaceMouse operation)

    void evaluate() override;

    const bool isCurrent(const char *name) const override;
};