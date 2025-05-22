#include "KnobAxis.hpp"

/**
 * @brief Class representing a translation KnobAxis in the system
 * @details This class inherits from the Axis class and provides functionality for managing the translation KnobAxis configuration and state.
 *          It includes methods for evaluating the KnobAxis state and notifying observers of changes.
 */
class KnobAxisTranslation : public KnobAxis {

public:
    KnobAxisTranslation() = delete; // Delete the default constructor
    KnobAxisTranslation(const MotionVector_t motionVectorType, ISensorsCalculator *sensorsCalculator)
        : KnobAxis(motionVectorType, sensorsCalculator) {};
    ~KnobAxisTranslation() = default; // Destructor

    const bool isTranslation() const override { return true; }

    void evaluate() override final {
        // Call the base class evaluate method
        KnobAxis::evaluate();

        // Notify observers of the axis of changes in the translation axis
        Observable::notifyObservers();
    };
};