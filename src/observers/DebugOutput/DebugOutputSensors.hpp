#pragma once

#include "DebugOutput.hpp"

class Sensor;

// #include <Arduino.h> // For Serial

class DebugOutputSensors : public DebugOutput {
private:
    virtual const int getSensorValue(const Sensor *sensor) const = 0;

public:
    DebugOutputSensors() = default;
    virtual ~DebugOutputSensors() {};

    virtual void update(IObservable *sensorCollection) override;
};
