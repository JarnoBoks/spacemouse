#include "Axis.hpp"

/**
 * @brief Class representing a rotation axis in the system.
 * @details This class inherits from the Axis class and provides functionality for managing the rotation axis configuration and state.
 *          It includes methods for evaluating the axis state and notifying observers of changes.
 */
class AxisRotation : public Axis {

public:
    AxisRotation() = delete; // Delete the default constructor
    AxisRotation(const AxisType_t axisType, ISensorsCalculator *sensorsCalculator) : Axis(axisType, sensorsCalculator) {};
    ~AxisRotation() = default; // Destructor

    void evaluate() override {
        // Call the base class evaluate method
        Axis::evaluate();
        Observable::notifyObservers(); // Notify observers of changes in the rotation axis
    };
};