// BaseStrategyDecorator.h
#pragma once
#include "ICalculationStrategy.h" // Include the ICalculationStrategy interface

// This class serves as a base class for decorators that can modify the behavior of the wrapped strategy
// without changing its interface. It allows for flexible and reusable design patterns in the calculation strategy implementation.
class BaseStrategyDecorator : public ICalculationStrategy {
private:
    ICalculationStrategy *wrappee; // Pointer to the wrapped strategy
public:
    BaseStrategyDecorator(ICalculationStrategy *wrappee) : wrappee(wrappee) {} // Constructor to initialize the wrapped strategy
    virtual ~BaseStrategyDecorator() = default;

    virtual void calculate(int16_t *value) override {
        if (wrappee) {
            wrappee->calculate(value); // Call the wrapped strategy's calculate method
        }
    }; // Pure virtual function to be implemented by derived classes
};

class KillSwitchDecorator : public BaseStrategyDecorator {
public:
    void extra(int16_t *value) {
        *value = 0;
    }
    void calculate(int16_t *value) override {
        BaseStrategyDecorator::calculate(value); // Call the wrapped strategy's calculate method
        extra(value);
    }
};

