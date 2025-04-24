#ifndef DEBUGMONITOR_H
#define DEBUGMONITOR_H

#define MAX_AXIS_OBSERVERS 4
#define MAX_SENSOR_OBSERVERS 4

#include <Arduino.h>
#include "axis.h"
#include "sensor.h"

enum DebugLevel : int8_t {
    OFF = -1,
    SILENT = 0,
    RAW_VALUES,
    CENTERED,
    HW_OUTPUT,
    AXIS_VALUES,
    AXIS_VALUES_EXT,
    AXIS_AND_KEYS,
    FULL_DEBUG,
    LOOP_FREQUENCY
};

// --- DebugMonitor (Observer Pattern) ---
class DebugMonitor {
private:
    DebugLevel currentLevel;

public:
    void setLevel(DebugLevel level);
    void logSensorValues(Sensor *sensors[], uint8_t count);
    void logAxisValues(Axis *axes[], uint8_t count);
    void logLoopFrequency();
    void update(Axis *axis);
    void update(Sensor *sensor);
};

#endif // DEBUGMONITOR_H