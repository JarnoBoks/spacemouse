#pragma once

#include <Arduino.h>
#include "SensorsCalculator.hpp"

class SensorsCalculatorHall : public SensorsCalculator {
private:
public:
    SensorsCalculatorHall();
    SensorsCalculatorHall(SensorCollection *sensorCollection) : SensorsCalculator(sensorCollection) {}
    ~SensorsCalculatorHall() {}; // nothing to do in destructor

    virtual void evaluate(Axis *axis) override;

    void setAnalogReference(const bool isDebug = false) override;
};
