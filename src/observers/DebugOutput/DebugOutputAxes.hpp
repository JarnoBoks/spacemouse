#pragma once

#include "DebugOutput.hpp"
#include "kinematics/kinematics.h"

class DebugOutputAxes : public DebugOutput {
private:
protected:
    virtual const int getAxisValue(const Axis *axis) const = 0; // Pure virtual function to get the axis value. Derived classes must implement this function.

public:
    DebugOutputAxes() = default;
    virtual ~DebugOutputAxes() {};

    void update(Kinematics *kinematics) override;
    inline void update(Hardware *hardware) override {}; // No implementation needed for this class
};
