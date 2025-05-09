#pragma once

#include <Arduino.h>
#include "SensorsCalculator.hpp"

class SensorsCalculatorJoystick : public SensorsCalculator {
public:
    SensorsCalculatorJoystick();
    SensorsCalculatorJoystick(SensorCollection *sensorCollection) : SensorsCalculator(sensorCollection) {} // Constructor with sensor collection
    ~SensorsCalculatorJoystick() {};                                                                       // nothing to do in destructor

    virtual void evaluate(Axis *axis) override;
};
