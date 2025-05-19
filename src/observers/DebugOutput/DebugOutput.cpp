#include "DebugOutput.hpp"

// Include system headers
#include <wiring_private.h> // For millis()

/**
 * @brief       Determines if a new debug output should be printed.
 * @details     Used to generate a debug line only every DEBUGDELAY ms
 * @see         config.h
 * @retval true     Debug output is due
 * @retval false    Debug output is not due
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