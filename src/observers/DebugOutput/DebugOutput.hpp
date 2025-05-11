#pragma once
#include "observers/IObserver.hpp"
#include "config.h" // For STARTDEBUG and DEBUGDELAY

#ifndef STARTDEBUG
// For fail-safe: If STARTDEBUG is not defined, set it to 0 (no debug output).
#define STARTDEBUG OFF
#endif

#ifndef DEBUGDELAY
// For fail-safe: If DEBUGDELAY is not defined, set it to 250 ms (0.25 seconds).
#define DEBUGDELAY 250
#endif

/**
 * @brief Base class for observers that output debug information (sensors and/or axes) to the Serial monitor.
 * @details This class implements the IObserver interface and provides a mechanism to check if debug output is due.
 *          It is used as a base class for specific debug output observers that will be derived from it.
 */
class DebugOutput : public IObserver {
private:
    unsigned long m_lastDebugOutput = 0; // time from millis(), when the last debug output was written to the Serial monitor

protected:
    bool isDebugOutputDue(); // Check if a new debug output should be printed

public:
    DebugOutput() = default;  // Default constructor
    virtual ~DebugOutput() {} // Default destructor

    // virtual void update(Kinematics *kinematics) override = 0;             // Pure virtual function to be implemented by derived classes
    // virtual void update(SensorCollection *sensorCollection) override = 0; // Pure virtual function to be implemented by derived classes
};
