#pragma once
#include "ObserverDebugOutput.hpp"
#include "kinematics/kinematics.h"

class Output_LoopFrequency : public ObserverDebugOutput {
private:
    unsigned long _lastFrequencyUpdate; // time from millis(), when the last debug output was given
    int _iterationsPerSecond;           // number of loop iterations per second
public:
    Output_LoopFrequency() : _lastFrequencyUpdate(millis()), _iterationsPerSecond(0) {
        Serial.println(F("..."));
    };
    virtual ~Output_LoopFrequency() {}; // nothing to do in destructor

    void update(Kinematics *kinematics) override;
};
