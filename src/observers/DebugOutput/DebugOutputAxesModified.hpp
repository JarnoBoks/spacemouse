#pragma once

#include "debugOutputAxes.hpp"

/**
 * @brief Output class for modified axes values.
 * @details Writes the value of all the axes after applying the modifier function to the Serial monitor.
 *          This class inherits from OutputAxes and overrides the getAxisValue method to return the value of the axis after applying the modifier function.
 *          After outputting the values, it prints a newline character to the Serial monitor.
 */
class DebugOutputAxesModified : public DebugOutputAxes {
private:
protected:
    inline const int getAxisValue(const Axis *axis) const override {
        return axis->getModifiedValue(); // Get the axis value
    }

public:
    inline void update(Kinematics *kinematics) override {
        DebugOutputAxes::update(kinematics); // Call the base class update method
        Serial.println();
    }
};
