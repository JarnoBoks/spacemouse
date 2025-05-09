// DefaultCalculationStrategy.h
#pragma once
#include "ICalculationStrategy.h"

class DefaultCalculationStrategy : public ICalculationStrategy { // Forward declaration of DefaultCalculationStrategy{
public:
    virtual ~DefaultCalculationStrategy() = default; // Destructor
    virtual void calculate(int16_t *value) override {
        // Default calculation strategy: do nothing, just return the value as is
        // This can be overridden by derived classes to implement specific calculation strategies
        // For example, you can apply a deadzone or other transformations here
        // In this case, we just return the value as is
        *value = *value; // No change to the value
    };
};
