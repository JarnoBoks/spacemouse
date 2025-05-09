#pragma once

#include "DebugOutputAxes.hpp"
#include "kinematics/kinematics.h"

/**
 * @brief Output class for axis values with sensitivity applied.
 * @details This class inherits from DebugOutputAxes and overrides the getAxisValue method to return the value of the axis after applying sensitivity.
 *          It also overrides the update method to print the axis values to the serial monitor.
 */
class DebugOutputAxesSensitivity : public DebugOutputAxes {
private:
protected:
    inline const int getAxisValue(const Axis *axis) const override {
        return axis->getSnsValue(); // Get the axis value
    }

public:
    inline void update(Kinematics *kinematics) override {
        DebugOutputAxes::update(kinematics); // Call the base class update method
        Serial.println();
    }

    inline void update(SensorCollection *sensorCollection) override {}; // No implementation needed for this class
};
