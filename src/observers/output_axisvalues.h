#ifndef OUTPUT_AXISVALUES_H
#define OUTPUT_AXISVALUES_H

#include "IObserver.h"
#include "kinematics/kinematics.h"

enum AxisOutputValType_t {
    AO_OUTPUT = 0,
};

class Output_AxisValues : public IObserver {
private:
protected:
    virtual const int getAxisValue(const Axis *axis) const = 0;

public:
    Output_AxisValues() {};
    ~Output_AxisValues() {}; // nothing to do in destructor

    void update(Kinematics *kinematics) override; // Pure virtual function to be implemented by derived classes
};

class Output_AxisValuesRaw : public Output_AxisValues {
private:
protected:
    inline const int getAxisValue(const Axis *axis) const override {
        return axis->getSensValue(); // Get the axis value
    }
};

#endif // OUTPUT_AXISVALUES_H