#pragma once

#include "DebugOutput.hpp"
#include "hardware/hardware.h"

#if 0
//REMOVE
enum SensorOutputValType_t {
    SO_RAW = 0,
    SO_CENTERED,
};
#endif

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
