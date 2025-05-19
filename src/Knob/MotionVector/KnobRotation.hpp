#include "KnobMotionVector.hpp"

/**
 * @brief Class representing a rotation MotionVector in the system.
 * @details This class inherits from the KnobMotionVector class and provides functionality for managing the rotation MotionVector configuration and state.
 *          It includes methods for evaluating the MotionVector state and notifying observers of changes.
 */
class KnobRotation : public KnobMotionVector {

public:
    KnobRotation() = delete; // Delete the default constructor
    KnobRotation(const MotionVector_t motionVectorType, ISensorsCalculator *sensorsCalculator) : KnobMotionVector(motionVectorType, sensorsCalculator) {};
    ~KnobRotation() = default; // Destructor

    const bool isTranslation() const override { return false; }

    void evaluate() override final {
        // Call the base class evaluate method
        KnobMotionVector::evaluate();

        // Notify observers of changes in the rotation axis
        Observable::notifyObservers();
    };
};