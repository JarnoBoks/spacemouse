#pragma once

#include "DebugOutputAxes.hpp"
#include "kinematics/kinematics.h"

/**
 * @brief Output class for raw axis values.
 * @details This class inherits from DebugOutputAxes and overrides the getAxisValue method to return the raw value of the axis.
 * It also overrides the update method to print the axis values to the serial monitor.
 */
class DebugOutputAxesRaw : public DebugOutputAxes {
private:
protected:
    inline const int getAxisValue(const Axis *axis) const override {
        return axis->getRawValue(); // Get the axis value
    }

public:
    inline void update(Kinematics *kinematics) override {
        DebugOutputAxes::update(kinematics); // Call the base class update method
        Serial.println();
    }
    inline void update(Hardware *hardware) override {}; // No implementation needed for this class
};
