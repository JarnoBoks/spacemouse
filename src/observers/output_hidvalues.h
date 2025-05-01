#pragma once

#include "IObserver.h"
#include "kinematics/kinematics.h"

class Output_HIDValues : public IObserver {
private:
protected:
    virtual const int getHIDValue(const Axis *axis) const = 0;

public:
    Output_HIDValues() {};
    ~Output_HIDValues() {};

    void update(Kinematics *kinematics) override; // Pure virtual function to be implemented by derived classes
};