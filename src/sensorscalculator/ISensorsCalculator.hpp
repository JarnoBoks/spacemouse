#pragma once

class KnobAxis;

class ISensorsCalculator {
public:
    ISensorsCalculator() = default;
    virtual ~ISensorsCalculator() = default;

    virtual void evaluate(KnobAxis *vector) = 0; // Pure virtual method to calculate sensor data
};
