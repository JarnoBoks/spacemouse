#ifndef IOBSERVER_H
#define IOBSERVER_H

#include <Arduino.h>
#include "config.h" // For STARTDEBUG and DEBUGDELAY

// Define maximum observers for Kinematics class
#define MAX_KINEMATICS_OBSERVERS 4

// Define maximum observers for Hardware class
#define MAX_HARDWARE_OBSERVERS 4

#ifndef STARTDEBUG
// For fail-safe: If STARTDEBUG is not defined, set it to 0 (no debug output).
#define STARTDEBUG OFF
#endif

#ifndef DEBUGDELAY
// For fail-safe: If DEBUGDELAY is not defined, set it to 250 ms (0.25 seconds).
#define DEBUGDELAY 250
#endif

// Forward declaration of classes that are observed (to avoid circular dependencies)
class Kinematics;
class Hardware;

// --- DebugMonitor (Observer Pattern) ---
class IObserver {
private:
    unsigned long lastDebugOutput = 0; // time from millis(), when the last debug output was given // REVIEW - We should be able to store a smaller value than 4 bytes here, but we need to check if the compiler does this automatically.

protected:
    bool isDebugOutputDue(); // Check if a new debug output should be printed

public:
    IObserver() = default; // Default constructor
    // NOTE IObserver(const IObserver &) = delete; // Delete copy constructor to prevent copying
    virtual ~IObserver() {}

    virtual void update(Kinematics *kinematics);
    virtual void update(Hardware *hardware);
};

#endif // IOBSERVER_H