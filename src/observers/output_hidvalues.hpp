#pragma once

#include "ObserverDebugOutput.hpp"
#include "kinematics/kinematics.h"

class Output_HIDValues : public ObserverDebugOutput {
private:
protected:
    virtual const int getHIDValue(const Axis *axis) const = 0;

public:
    Output_HIDValues() {};
    ~Output_HIDValues() {};

    void update(Kinematics *kinematics) override;
};