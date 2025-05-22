#include "KnobAxis.hpp"

/**
 * @brief Class representing a rotation KnobAxis in the system.
 * @details This class inherits from the KnobAxis class and provides functionality for managing the rotation KnobAxis configuration and state.
 *          It includes methods for evaluating the KnobAxis state and notifying observers of changes.
 */
class KnobAxisRotation : public KnobAxis {

public:
    KnobAxisRotation() = delete; // Delete the default constructor
    KnobAxisRotation(const MotionVector_t motionVectorType, ISensorsCalculator *sensorsCalculator)
        : KnobAxis(motionVectorType, sensorsCalculator) {};
    ~KnobAxisRotation() = default; // Destructor

    const bool isTranslation() const override { return false; }

    void evaluate() override final {
        // Call the base class evaluate method
        KnobAxis::evaluate();

        // Notify observers of changes in the rotation axis
        Observable::notifyObservers();
    };
};