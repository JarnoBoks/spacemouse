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
    inline const int getSensorValue(const Sensor *sensor) const override {
        return sensor->getFinValue(); // Get the filtered value from the sensor
    }

public:
    DebugOutputSensorsFiltered(const bool outputNewline = true) : DebugOutputSensors(outputNewline) {}
    virtual ~DebugOutputSensorsFiltered() = default;

    void update(IObservable *sensorCollection) override final {
        DebugOutputSensors::update(sensorCollection);
    }
};
