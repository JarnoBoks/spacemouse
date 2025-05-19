#pragma once

#include "DebugOutputSensors.hpp"
#include "sensor/sensors/Sensor.hpp"

#include <Arduino.h> // For Serial

/**
 * @brief Output class for raw sensor values.
 * @details This class inherits from DebugOutputSensors and overrides the getSensorValue method to return the raw value of the sensor.
 * It also overrides the update method to print the sensor values to the serial monitor.
 */
class DebugOutputSensorsRaw : public DebugOutputSensors {
private:
    inline const int getSensorValue(const Sensor *sensor) const override {
        return sensor->getRawValue(); // Get the raw value from the sensor
    }

public:
    DebugOutputSensorsRaw() {};

    void update(IObservable *sensorCollection) override {
        // Call the base class update method
        DebugOutputSensors::update(sensorCollection);

        Serial.println();
    }
};
