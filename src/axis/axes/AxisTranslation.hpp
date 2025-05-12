#include "Axis.hpp"

/**
 * @brief Class representing a rotation axis in the system.
 * @details This class inherits from the Axis class and provides functionality for managing the rotation axis configuration and state.
 *          It includes methods for evaluating the axis state and notifying observers of changes.
 */
class AxisTranslation : public Axis {

public:
    AxisTranslation() = delete; // Delete the default constructor
    AxisTranslation(const AxisType_t axisType, ISensorsCalculator *sensorsCalculator) : Axis(axisType, sensorsCalculator) {};
    ~AxisTranslation() = default; // Destructor

    void evaluate() override final {
        // Call the base class evaluate method
        Axis::evaluate();
        // Notify observers of the axis of changes in the translation axis
        Observable::notifyObservers();
    };
};