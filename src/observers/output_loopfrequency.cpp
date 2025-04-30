#include "output_loopfrequency.h"
#include "kinematics/kinematics.h"
#include "text.h"

// REVIEW - Move to Visitor?

void Output_LoopFrequency::update(Kinematics *kinematics) {
    // increase iterations counter
    _iterationsPerSecond++;
    if (millis() - _lastFrequencyUpdate > 1000) { // if one second has past: report frequency
        Serial.print("Freq: ");
        Serial.print(_iterationsPerSecond);
        Serial.println(" Hz");
        _lastFrequencyUpdate = millis(); // reset timer
        _iterationsPerSecond = 0;        // reset iteration counter
    }
}
