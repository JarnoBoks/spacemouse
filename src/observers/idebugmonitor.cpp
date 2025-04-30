
#include "idebugmonitor.h"

/**
 * @brief Indicate if a new debug output should be printed.
 *         Used to generate a debug line only every DEBUGDELAY ms, see config.h
 * @see config.h
 * @retval true     debug output is due
 * @retval false    debug output is not due
 */
bool IDebugMonitor::isDebugOutputDue() {
    lastDebugOutput = 0; // time from millis(), when the last debug output was given

    if (millis() - lastDebugOutput > DEBUGDELAY) {
        lastDebugOutput = millis();
        return true;
    } else {
        return false;
    }
}

void IDebugMonitor::update(const Axis *axis) {
    // Doing nothing here, as this is a virtual function to be overridden by derived classes.
    // Additional implementation can be added in derived classes.
}

void IDebugMonitor::update(Hardware *hardware) {
    // Doing nothing here, as this is a virtual function to be overridden by derived classes.
    // Additional implementation can be added in derived classes.
}