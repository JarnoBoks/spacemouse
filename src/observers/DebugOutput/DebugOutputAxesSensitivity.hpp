#pragma once

#include "DebugOutputAxes.hpp"
#include <knob/axis/KnobAxis.hpp>

/**
 * @brief Output class for axis values with sensitivity applied.
 * @details This class inherits from DebugOutputAxes and overrides the getAxisValue method to return the value of the axis after applying sensitivity.
 *          It also overrides the update method to print the axis values to the serial monitor.
 */
class DebugOutputAxesSensitivity : public DebugOutputAxes {
private:
    inline const int getAxisValue(const KnobAxis *axis) const override {
        // Class need the sensitivity value for printing.
        return axis->getSnsValue(); // Get the axis value
    }

public:
    inline void update(IObservable *axisCollection) override {
        // Call the base class update method
        DebugOutputAxes::update(axisCollection);
        Serial.println();
    }
};
