#pragma once

#include "DebugOutput.hpp"

class KnobAxis;

class DebugOutputAxes : public DebugOutput {
protected:
    virtual const int getAxisValue(const KnobAxis *axis) const = 0; // Pure virtual function to get the axis value. Derived classes must implement this function.

public:
    DebugOutputAxes(const bool outputNewline = true) : DebugOutput(outputNewline) {}
    virtual ~DebugOutputAxes() {}

    void update(IObservable *axisCollection) override;
};
