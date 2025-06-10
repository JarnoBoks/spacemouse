#pragma once

#include "DebugOutput.hpp"

class Sensor;

// #include <Arduino.h> // For Serial

class DebugOutputSensors : public DebugOutput {
private:
    virtual const int getSensorValue(const Sensor *sensor) const = 0;

public:
    DebugOutputSensors(const bool outputNewline = true) : DebugOutput(outputNewline) {}
    virtual ~DebugOutputSensors() {}

    virtual void update(IObservable *sensorCollection) override;
};
