// output_loopfrequency.cpp
#include "output_loopfrequency.hpp"
#include "kinematics/kinematics.h"

void Output_LoopFrequency::update(Kinematics *kinematics) {
    _iterationsPerSecond++;
    if (millis() - _lastFrequencyUpdate > 1000) { // if one second has past: report frequency
        Serial.print(F("Freq: "));
        Serial.print(_iterationsPerSecond);
        Serial.println(F(" Hz"));
        _lastFrequencyUpdate = millis(); // reset timer
        _iterationsPerSecond = 0;        // reset iteration counter
    }
}
