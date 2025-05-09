#pragma once

#include "DebugOutput.hpp"

class Sensor;
class SensorCollection;

class DebugOutputSensors : public DebugOutput {
private:
protected:
    virtual const int getSensorValue(const Sensor *sensor) const = 0;

public:
    DebugOutputSensors() {};
    virtual ~DebugOutputSensors() {};

    virtual void update(Kinematics *kinematics) override = 0;             // No implementation needed for this class
    virtual void update(SensorCollection *sensorCollection) override = 0; // Pure virtual function to be implemented by derived classes
};
