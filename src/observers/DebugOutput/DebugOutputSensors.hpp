#pragma once

#include "DebugOutput.hpp"
#include "hardware/hardware.h"

class DebugOutputSensors : public DebugOutput {
private:
protected:
    virtual const int getSensorValue(const Sensor *sensor) const = 0;

public:
    DebugOutputSensors() {};
    virtual ~DebugOutputSensors() {};

    virtual void update(Hardware *hardware) override = 0;     // Pure virtual function to be implemented by derived classes
    virtual void update(Kinematics *kinematics) override = 0; // No implementation needed for this class
};
