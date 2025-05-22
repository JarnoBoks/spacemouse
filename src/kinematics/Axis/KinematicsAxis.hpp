#pragma once

/*
Extends KnobMotionVectorFunctionality. (Inversion of the knob motionvector)
Not observable by the HID Event buffer -> that will observe the KinematicsMotionVectorCollections (TRANS collection and ROT collection)
*/
#include <motionvector/MotionVector.hpp>
#include <common/esp_print.h> //REFACTOR - Move to cpp file

class KnobMotionVector;
#include <knob/MotionVector/KnobMotionVector.hpp> // REFACTOR - Move to cpp file
#include <Knob/KnobMotionVectorCollection.hpp>    // REFACTOR - Move to cpp file

/// @brief Number of observers that can be added to Kinematics MotionVector.
/// @details This is a constant value that defines the maximum number of observers that can be added to the Kinematics MotionVector.
/// TODO @note This value is set to 1, as the Kinematics MotionVector is only having the HID Event buffer as observer.
constexpr uint8_t c_KIN_MAX_MOTIONVECTOR_OBSERVERS = 1;

// REVIEW - What to do if the MotionVector hasn't any observers at all? Should the base class be observable?

class KinematicsAxis : public MotionVector {
private:
    const KnobMotionVector *m_knobMotionVector = nullptr; // Pointer to the knob MotionVector
    // TODO - Add debug values for the KinematicsAxis, when we want to print values before fe. applying killswitches

public:
    KinematicsAxis() = delete; // Delete default constructor, do not allow instantiation without parameters

    KinematicsAxis(MotionVector_t type, KnobMotionVector *knobMotionVector)
        : MotionVector(type, c_KIN_MAX_MOTIONVECTOR_OBSERVERS),
          m_knobMotionVector(knobMotionVector) {} // Constructor with parameters

    ~KinematicsAxis() = default;

    void evaluate() override final {
        if (m_knobMotionVector == nullptr) {
            ESP_WARN("KnobMotionVector not set");
            return; // Error: KnobMotionVector not found, exit the function
        }

        // Get the inversion setting from the knob MotionVector
        const bool invert = m_knobMotionVector->getConfig()->getInvert();
        int16_t value = m_knobMotionVector->getFinValue(); // Get the final value from the knob MotionVector
        m_finValue = (invert) ? -value : value;            // Invert the value if the inversion setting is enabled

        // Notify observers of changes in the Kinematics MotionVector
        Observable::notifyObservers();
    };
};