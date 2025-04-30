#ifndef IOBSERVER_H
#define IOBSERVER_H

// Define the maximum number of observers that can connect to axes and hardware
#define MAX_AXIS_OBSERVERS 4
#define MAX_HARDWARE_OBSERVERS 4

#include <Arduino.h>
#include "config.h" // For STARTDEBUG and DEBUGDELAY

#ifndef STARTDEBUG
// For fail-safe: If STARTDEBUG is not defined, set it to 0 (no debug output).
#define STARTDEBUG OFF
#endif

#ifndef DEBUGDELAY
// For fail-safe: If DEBUGDELAY is not defined, set it to 250 ms (0.25 seconds).
#define DEBUGDELAY 250
#endif

enum DebugLevel : int8_t {
    OFF = -1,
    SILENT = 0,
    RAW_VALUES, // 1 - Raw values of the sensors
    CENTERED,   // 2 - Centered values of the sensors
    HW_OUTPUT,  // 3 - Hardware output values (e.g. mapped & deadzone applied)
    AXIS_VALUES,
    AXIS_VALUES_EXT,
    AXIS_AND_KEYS,
    FULL_DEBUG,
    LOOP_FREQUENCY
};

// Forward declaration of classes that are observed (to avoid circular dependencies)
class Axis;
class Hardware;

// --- DebugMonitor (Observer Pattern) ---
class IObserver {
private:
    unsigned long lastDebugOutput = 0; // time from millis(), when the last debug output was given // FIXME - We should be able to store a smaller value than 4 bytes here, but we need to check if the compiler does this automatically.

protected:
    bool isDebugOutputDue(); // Check if a new debug output should be printed

public:
    IObserver() : lastDebugOutput(0) {} // Constructor
    virtual ~IObserver() {}             // Destructor

    void logAxisValues(Axis *axes[], uint8_t count);

    void logLoopFrequency();

    virtual void update(const Axis *axis);
    virtual void update(Hardware *hardware);
};

#endif // IOBSERVER_H