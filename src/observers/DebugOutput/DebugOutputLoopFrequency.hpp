#pragma once
#include "DebugOutput.hpp"
#include "common/freeRAM.h"

/**
 * @brief Class for outputting the loop frequency and free RAM information.
 * @details This class inherits from DebugOutput and overrides the update method to print the loop frequency and free RAM information to the serial monitor.
 * @note The free RAM information is only available for AVR architecture.
 */
class DebugOutputLoopFrequency : public DebugOutput {
private:
    unsigned long m_lastFrequencyUpdate; // time from millis(), when the last debug output was given
    int m_iterationsPerSecond = 0;       // number of loop iterations per second
public:
    DebugOutputLoopFrequency() : m_lastFrequencyUpdate(millis()) {
        Serial.println(F("..."));
    };
    virtual ~DebugOutputLoopFrequency() {}; // nothing to do in destructor

    void update(IObservable *axisCollection) override {
        m_iterationsPerSecond++;
        if (millis() - m_lastFrequencyUpdate > 1000) { // if one second has past: report frequency
            Serial.print(F("Freq: "));
            Serial.print(m_iterationsPerSecond);
            Serial.print(F(" Hz"));

            Serial.print(F(", Free RAM: "));
            Serial.print(FreeRAM::freeRam());
            Serial.println(F(" bytes"));

            m_lastFrequencyUpdate = millis(); // reset timer
            m_iterationsPerSecond = 0;        // reset iteration counter
        }
    }
};
