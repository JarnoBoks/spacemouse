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

protected:
    Sensor *sensors[MAX_SENSORS] = {nullptr}; // Array of sensor pointers
    void notifyObservers();                   // Notify all observers of changes
public:
    Hardware() : referenceVoltage(DEFAULT), observers{nullptr}, observerCount(0), sensors{nullptr} {};

    ~Hardware() {
        for (int i = 0; i < MAX_SENSORS; i++) {
            if (sensors[i] != nullptr) {
                delete sensors[i];
                sensors[i] = nullptr;
            }
        }
    }
    virtual int16_t calculateRawValue(AxisType_t axistype) = 0;
    virtual void setAnalogReference(const uint8_t voltage);

    inline void getSensors(Sensor *sensors[]) {
        sensors = this->sensors; // Return the array of sensor pointers
    }

    void attachObserver(IDebugMonitor *observer);
    void detachObserver(IDebugMonitor *observer);
};

#endif // HARDWARE_H