#ifndef AXIS_H
#define AXIS_H

#include <Arduino.h>
#include "axisconfig.h"
#include "debugmonitor.h"
#include "hardware.h"
#include "lightbehavior.h"

enum AxisType_t : int8_t { ALL = -2,
                           UNINITIALIZED = -1,
                           TX = 0,
                           TY,
                           TZ,
                           RX,
                           RY,
                           RZ,
                           LENGTH };

class Axis {
private:
    AxisType_t type = UNINITIALIZED;
    int16_t value = 0;
    AxisConfig *config = nullptr;
    Hardware *hardware = nullptr;
    LightBehavior *light = nullptr;
    DebugMonitor *observers[MAX_AXIS_OBSERVERS];
    uint8_t observerCount = 0;

public:
    Axis() = default; // Default constructor
    Axis(AxisType_t type);

    void attachObserver(DebugMonitor *observer);
    void detachObserver(DebugMonitor *observer);
    void notifyObservers();

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