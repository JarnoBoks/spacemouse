#pragma once
#include "observers/IObserver.hpp"

#include "config.h" // For DEBUGDELAY

#ifndef DEBUGDELAY
// Fail-safe: If DEBUGDELAY is not defined, set it to 250 ms (0.25 seconds).
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
    const bool m_outputNewLine = true;   // Flag to control whether to output a new line (true) or separator (false) after the debug output

protected:
    bool isDebugOutputDue(); // Check if a new debug output should be printed
    void endOutput();        // End the debug output, e.g., by printing a new line or text separator

public:
    DebugOutput(const bool outputNewline = true) : m_outputNewLine(outputNewline) {}
    virtual ~DebugOutput() {}
};
