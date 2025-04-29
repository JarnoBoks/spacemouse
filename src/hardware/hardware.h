// TODO - Rename to HardwareFactory

#ifndef HARDWARE_H
#define HARDWARE_H

#define MAX_SENSORS 8

#include "kinematics/axis.h" // for AxisType enum
#include "serialoutput/idebugmonitor.h"
#include "sensor/sensor.h"

class Hardware {
private:
    uint8_t referenceVoltage = DEFAULT;

    IDebugMonitor *observers[MAX_HARDWARE_OBSERVERS] = {nullptr}; // Array of observers
    uint8_t observerCount = 0;

    static Hardware *_instance; // Singleton instance - contains the derived hardware class

protected:
    // Hardware can only be instantiated by derived classes
    Hardware() : referenceVoltage(DEFAULT), observers{nullptr}, observerCount(0), sensors{nullptr} {};
    void updateSensorValues();

    void registerInstance(Hardware *instance) {
        _instance = instance; // Register the instance of the derived class
    }

public:
    static Hardware *getInstance();

    ~Hardware() {
        for (int i = 0; i < MAX_SENSORS; i++) {
            if (sensors[i] != nullptr) {
                delete sensors[i];
                sensors[i] = nullptr;
            }
        }
    }

    virtual int16_t calculateRawValue(AxisType_t axistype) = 0;

    void setAnalogReference(const uint8_t voltage);

    void attachObserver(IDebugMonitor *observer);
    void detachObserver(IDebugMonitor *observer);
    void notifyObservers(); // Notify all observers of changes

    Sensor *sensors[MAX_SENSORS] = {nullptr}; // Array of sensor pointers, public defined so it can be used in the observer class

    Sensor *getSensorByName(const char *name) const;
};

#endif // HARDWARE_H