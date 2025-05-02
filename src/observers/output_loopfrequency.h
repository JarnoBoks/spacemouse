// output_loopfrequency.h
#pragma once
#include "IObserver.h"
#include "kinematics/kinematics.h"

class Output_LoopFrequency : public IObserver {
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
