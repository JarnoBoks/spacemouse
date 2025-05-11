#pragma once

#include "DebugOutput.hpp"

class Axis;

class DebugOutputAxes : public DebugOutput {
private:
protected:
    virtual const int getAxisValue(const Axis *axis) const = 0; // Pure virtual function to get the axis value. Derived classes must implement this function.

public:
    DebugOutputAxes() = default;
    virtual ~DebugOutputAxes() {};

    void update(IObservable *axisCollection) override;
};
