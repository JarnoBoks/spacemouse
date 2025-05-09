#pragma once

#include "DebugOutputSensors.hpp"
#include "sensor/sensors/Sensor.hpp"
#include "common/TextHelper.h"

#include <Arduino.h> // For Serial

/**
 * @brief Output class for centered sensor values, without a newline (but with a separator).
 * @details This class inherits from DebugOutputSensors and overrides the getSensorValue method to return the centered value of the sensor.
 * It also overrides the update method to print the sensor values to the serial monitor.
 * @details This class is used to print the sensor values without a newline at the end, but with a separator. Used in conjunction with the axis values.
 */
class DebugOutputSensorsCenteredNoNewline : public DebugOutputSensors {
private:
protected:
    inline const int getSensorValue(const Sensor *sensor) const override {
        return sensor->getCenteredValue(); // Get the centered value from the sensor
    }

public:
    void update(SensorCollection *sensorCollection) {
        // Call the base class update method
        DebugOutputSensors::update(sensorCollection);
        TextHelper::printSeparator();
    }
    inline void update(Kinematics *kinematics) override {}; // No implementation needed for this class
};