#pragma once
#include "IObserver.h"
#include "kinematics/kinematics.h"

class Output_LoopFrequency : public IObserver {
private:
protected:
    unsigned long _lastFrequencyUpdate = 0; // time from millis(), when the last debug output was given
    int _iterationsPerSecond = 0;           // number of iterations per second

public:
    Output_LoopFrequency() {};
    virtual ~Output_LoopFrequency() {}; // nothing to do in destructor

    void update(Kinematics *kinematics) override;
};
