#pragma once

class KnobMotionVector;

class ISensorsCalculator {
public:
    ISensorsCalculator() = default;
    virtual ~ISensorsCalculator() = default;

    virtual void evaluate(KnobMotionVector *vector) = 0; // Pure virtual method to calculate sensor data
};
