#include "IObserver.h"
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
 * @brief Base class for observers that output debug information to the Serial monitor.
 */
class ObserverDebugOutput : public IObserver {
private:
    unsigned long m_lastDebugOutput = 0; // time from millis(), when the last debug output was written to the Serial monitor

protected:
    bool isDebugOutputDue(); // Check if a new debug output should be printed
public:
    ObserverDebugOutput() = default;  // Default constructor
    virtual ~ObserverDebugOutput() {} // Default destructor

    virtual void update(Kinematics *kinematics) override;
    virtual void update(Hardware *hardware) override;
};
