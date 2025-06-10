#pragma once

#include "DebugOutputSensors.hpp"
#include "sensor/sensors/Sensor.hpp"
#include "common/TextHelper.h"

#include <Arduino.h> // For Serial

/**
 * @brief   Output class for centered sensor values.
 * @details This class inherits from DebugOutputSensors and overrides the getSensorValue method to return the centered value of the sensor.
 *          It also overrides the update method to print the sensor values to the serial monitor.
 */
class DebugOutputSensorsCentered : public DebugOutputSensors {
private:
    inline const int getSensorValue(const Sensor *sensor) const override {
        return sensor->getCntValue(); // Get the centered value from the sensor
    }

public:
    DebugOutputSensorsCentered(const bool outputNewline = true) : DebugOutputSensors(outputNewline) {}
    virtual ~DebugOutputSensorsCentered() = default;

    void update(IObservable *sensorCollection) override final {
        DebugOutputSensors::update(sensorCollection);
    }
};
