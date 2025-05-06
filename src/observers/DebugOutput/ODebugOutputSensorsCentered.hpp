#pragma once

#include "DebugOutputSensors.hpp"

/**
 * @brief Output class for centered sensor values.
 * @details This class inherits from DebugOutputSensors and overrides the getSensorValue method to return the centered value of the sensor.
 * It also overrides the update method to print the sensor values to the serial monitor.
 */
class DebugOutputSensorsCentered : public DebugOutputSensors {
private:
protected:
    inline const int getSensorValue(const Sensor *sensor) const override {
        return sensor->getCenteredValue(); // Get the centered value from the sensor
    }

public:
    inline void update(Hardware *hardware) override {
        DebugOutputSensors::update(hardware); // Call the base class update method
        Serial.println();
    }
};
