#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include "sensorconfig.h"
#include "debugmonitor.h"

class Sensor {
protected:
    const int8_t pin = -1; // Default pin value to indicate uninitialized state
    const char *name = nullptr;
    const int8_t id = -1; // Default id value to indicate uninitialized state
    const SensorConfig *config = nullptr;

    DebugMonitor *observers[MAX_SENSOR_OBSERVERS];
    uint8_t observerCount = 0;

    int rawvalue = 0;
    int centered = 0;
    int filtered = 0;

public:
    Sensor(const int8_t pin)
        : pin(pin), id(id) {
        for (uint8_t i = 0; i < MAX_SENSOR_OBSERVERS; i++) {
            observers[i] = nullptr;
        }
    }
    virtual ~Sensor() {
    };

    int getFilteredValue() {
        return filtered;
    }
    virtual const char *getName() const;
    virtual uint8_t getId() const;

    void attachObserver(DebugMonitor *observer);
    void detachObserver(DebugMonitor *observer);
    void notifyObservers();

    virtual void readValue();
    virtual void applyCalibration();
};

#endif // SENSOR_H