#pragma once

#include <Arduino.h>
#include "SensorsCalculator.hpp"

class SensorsCalculatorHall : public SensorsCalculator {
private:
public:
    SensorsCalculatorHall();
    SensorsCalculatorHall(SensorCollection *sensorCollection) : SensorsCalculator(sensorCollection) {}
    ~SensorsCalculatorHall() {}; // nothing to do in destructor

    void evaluate(Axis *axis) override final;

    void setAnalogReference(const bool isDebug = false) override;
};
