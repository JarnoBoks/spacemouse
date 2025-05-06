#include "DebugOutput.hpp"

// Include system headers
#include <Arduino.h> // For millis()

/**
 * @brief       Indicate if a new debug output should be printed.
 * @details     Used to generate a debug line only every DEBUGDELAY ms, see config.h
 * @see         config.h
 * @retval true     debug output is due
 * @retval false    debug output is not due
 */
bool DebugOutput::isDebugOutputDue() {
    m_lastDebugOutput = 0; // time from millis(), when the last debug output was given

    if (millis() - m_lastDebugOutput > DEBUGDELAY) {
        m_lastDebugOutput = millis();
        return true;
    } else {
        return false;
    }
}