// TODO - Rename to HardwareFactory
// NOTE - The software architecture is inspired by https://stackoverflow.com/questions/1820477/c-static-virtual-members

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
    static Hardware *_instance; // Singleton instance - contains the derived hardware class

    // Hardware can only be instantiated by derived classes
    Hardware() : referenceVoltage(DEFAULT), observers{nullptr}, observerCount(0), sensors{nullptr} {};
    void updateSensorValues();

    virtual Hardware *GetHardwareInstance() = 0;

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

template <class HardwareType>
class HardwareImpl : public Hardware {
protected:
    virtual Hardware *GetHardwareInstance() {
        return HardwareType::getInstance();
    }
};

#endif // HARDWARE_H