#pragma once

class Axis;

class ISensorsCalculator {
public:
    ISensorsCalculator() = default;
    virtual ~ISensorsCalculator() = default;

    virtual void calculate(Axis *axis) = 0; // Pure virtual method to calculate sensor data
};
