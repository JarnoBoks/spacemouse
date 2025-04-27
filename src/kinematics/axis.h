#ifndef AXIS_H
#define AXIS_H

#include <Arduino.h>
#include "serialoutput/idebugmonitor.h" // For MAX_AXIS_OBSERVERS
#include "modifierfunction.h"           // For ModFunc_t

enum AxisType_t : int8_t { ALL = -2,
                           UNINITIALIZED = -1,
                           TRANSX = 0,
                           TRANSY,
                           TRANSZ,
                           ROTX,
                           ROTY,
                           ROTZ,
                           LENGTH };

// Forward declaration of classes to avoid circular dependencies
class AxisConfig; // Forward declaration of AxisConfig class
class Hardware;
class LightBehavior;
class IDebugMonitor;

class Axis {
private:
    AxisType_t type = UNINITIALIZED;
    int16_t value = 0;
    AxisConfig *config = nullptr;
    Hardware *hardware = nullptr;
    LightBehavior *light = nullptr;
    uint8_t observerCount = 0;

    void modifier(ModFunc_t type); // Modifier function for this axis & direction

public:
    Axis(); // Default constructor
    Axis(AxisType_t type);

    int16_t getValue() const;
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