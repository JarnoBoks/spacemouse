#include "KinematicsAxis.hpp"

class KnobAxis;

/**
 * @brief Class representing a translation KnobAxis in the system
 * @details This class inherits from the Axis class and provides functionality for managing the translation KnobAxis configuration and state.
 *          It includes methods for evaluating the KnobAxis state and notifying observers of changes.
 */
class KinematicsAxisRotation : public KinematicsAxis {

public:
    KinematicsAxisRotation() = delete; // Delete the default constructor
    KinematicsAxisRotation(const MotionVector_t motionVectorType, KnobAxis *knobAxis)
        : KinematicsAxis(motionVectorType, knobAxis) {}

    ~KinematicsAxisRotation() = default; // Destructor

    const bool isTranslation() const override { return false; }

    void evaluate() override final {
        // Call the base class evaluate method
        KinematicsAxis::evaluate();

        // Notify observers of the axis of changes in the rotation axis
        Observable::notifyObservers();
    };
};