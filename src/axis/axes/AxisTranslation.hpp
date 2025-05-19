#include "Axis.hpp"

/**
 * @brief Class representing a rotation axis in the system.
 * @details This class inherits from the Axis class and provides functionality for managing the rotation axis configuration and state.
 *          It includes methods for evaluating the axis state and notifying observers of changes.
 */
class AxisTranslation : public KnobMotionVector {

public:
    AxisTranslation() = delete; // Delete the default constructor
    AxisTranslation(const MotionVector_t motionVectorType, ISensorsCalculator *sensorsCalculator) : KnobMotionVector(motionVectorType, sensorsCalculator) {};
    ~AxisTranslation() = default; // Destructor

    const bool isTranslation() const override { return true; }

    void evaluate() override final {
        // Call the base class evaluate method
        KnobMotionVector::evaluate();

        // Notify observers of the axis of changes in the translation axis
        Observable::notifyObservers();
    };
};