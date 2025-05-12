#pragma once
#include "DebugOutput.hpp"
#include "common/freeRAM.h"

class DebugOutputLoopFrequency : public DebugOutput {
private:
    unsigned long _lastFrequencyUpdate; // time from millis(), when the last debug output was given
    int _iterationsPerSecond = 0;       // number of loop iterations per second
public:
    DebugOutputLoopFrequency() : _lastFrequencyUpdate(millis()) {
        Serial.println(F("..."));
    };
    virtual ~DebugOutputLoopFrequency() {}; // nothing to do in destructor

    void update(IObservable *axisCollection) override {
        _iterationsPerSecond++;
        if (millis() - _lastFrequencyUpdate > 1000) { // if one second has past: report frequency
            Serial.print(F("Freq: "));
            Serial.print(_iterationsPerSecond);
            Serial.print(F(" Hz"));

            Serial.print(F(", Free RAM: "));
            Serial.print(FreeRAM::freeRam());
            Serial.println(F(" bytes"));

            _lastFrequencyUpdate = millis(); // reset timer
            _iterationsPerSecond = 0;        // reset iteration counter
        }
    }
};
