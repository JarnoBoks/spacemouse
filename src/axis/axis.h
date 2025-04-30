#ifndef AXIS_H
#define AXIS_H

#include <Arduino.h>
#include "observers/IObserver.h" // For MAX_AXIS_OBSERVERS
#include "modifierfunction.h"    // For ModFunc_t

enum AxisType_t : int8_t { ALL = -2,
                           UNINITIALIZED = -1,
                           TRANSX = 0,
                           TRANSY,
                           TRANSZ,
                           ROTX,
                           ROTY,
                           ROTZ,
                           LENGTH };

#define AXIS_NAMES {"TX", "TY", "TZ", "RX", "RY", "RZ"} // Axis names for debugging

// Forward declaration of classes to avoid circular dependencies
class AxisConfig; // Forward declaration of AxisConfig class
class Hardware;
class LightBehavior;
class IDebugMonitor;

class Axis {
private:
    AxisType_t type = UNINITIALIZED;
    const char *name;

    int16_t value = 0;     // Value of the axis after reading from the hardware and applying all configurations
    int16_t sensValue = 0; // Raw value from the hardware, including sensitivity application (stored for debugging purposes)

    AxisConfig *config = nullptr;
    Hardware *hardware = nullptr;
    LightBehavior *light = nullptr;
    uint8_t observerCount = 0;

    void modifier(ModFunc_t type); // Modifier function for this axis & direction

public:
    Axis(); // Default constructor
    Axis(AxisType_t type);

    inline int16_t getValue() const { return value; }         // Getter for value           // REVIEW - Same asa Sensor class, but we need to check if we can use the same function for both classes.
    inline int16_t getSensValue() const { return sensValue; } // Getter for sensValue
    inline const char *getName() const { return name; }       // Getter for name            // REVIEW - Same as Sensor class, but we need to check if we can use the same function for both classes.

    void calculateValue();

    void setLedLight(LightBehavior *behavior);

    inline AxisType_t getType() const {
        return type;
    } // Getter for type

    inline AxisConfig *getConfig() const {
        return config;
    } // Getter for config
};

#endif // AXIS_H