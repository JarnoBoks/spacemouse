// ICalculationStrategy.h
#pragma once
#include <stdint.h> // For int16_t type

class ICalculationStrategy {
public:
    virtual ~ICalculationStrategy() = default;
    virtual void calculate(int16_t *value) = 0; // Pure virtual function to be implemented by derived classes
};
