#include "KinematicsAxis.hpp"

class KnobAxis;

/**
 * @brief Class representing a translation KnobAxis in the system
 * @details This class inherits from the Axis class and provides functionality for managing the translation KnobAxis configuration and state.
 *          It includes methods for evaluating the KnobAxis state and notifying observers of changes.
 */
class KinematicsAxisTranslation : public KinematicsAxis {

public:
    KinematicsAxisTranslation() = delete; // Delete the default constructor
    KinematicsAxisTranslation(const MotionVector_t motionVectorType, KnobAxis *knobAxis)
        : KinematicsAxis(motionVectorType, knobAxis) {}

    ~KinematicsAxisTranslation() = default; // Destructor

    const bool isTranslation() const override { return true; }

    void evaluate() override final {
        // Call the base class evaluate method
        KinematicsAxis::evaluate();

        // Notify observers of the axis of changes in the translation axis
        Observable::notifyObservers();
    };
};