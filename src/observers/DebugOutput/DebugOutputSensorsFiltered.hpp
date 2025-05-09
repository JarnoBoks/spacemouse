#pragma once

#include "DebugOutputSensors.hpp"
#include "sensor/sensors/Sensor.hpp"

#include <Arduino.h> // For Serial

/**
 * @brief Output class for filtered sensor values.
 * @details This class inherits from DebugOutputSensors and overrides the getSensorValue method to return the filtered value of the sensor.
 * It also overrides the update method to print the sensor values to the serial monitor.
 */
class DebugOutputSensorsFiltered : public DebugOutputSensors {
private:
protected:
    inline const int getSensorValue(const Sensor *sensor) const override {
        return sensor->getFinValue(); // Get the filtered value from the sensor
    }

public:
    void update(SensorCollection *sensorCollection) {
        // Call the base class update method
        DebugOutputSensors::update(sensorCollection);
        Serial.println();
    }

    inline void update(Kinematics *kinematics) override {} // No implementation needed for this class
};
