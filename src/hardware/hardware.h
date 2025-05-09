// TODO - Rename to HardwareFactory / HardwareCollection
// NOTE - The software architecture is inspired by https://stackoverflow.com/questions/1820477/c-static-virtual-members

#pragma once
#define MAX_SENSORS 8

#include "axis/Axis.hpp" // for AxisType enum
#include "..\sensor\sensor.hpp"
#include "observers/IObserver.hpp"
#include "IObservable.hpp" // For IObservable interface

class Hardware : public IObservable {
private:
    IObserver *observers[MAX_HARDWARE_OBSERVERS] = {nullptr}; // Array of observers
    uint8_t observerCount = 0;

protected:
    static Hardware *_instance; // Singleton instance - contains the derived hardware class

    uint8_t referenceVoltage = DEFAULT;

    // Hardware can only be instantiated by derived classes
    Hardware() : observerCount(0), referenceVoltage(DEFAULT), sensors{nullptr} {
        for (uint8_t i = 0; i < MAX_HARDWARE_OBSERVERS; i++) {
            observers[i] = nullptr; // Initialize the observers array to nullptr
        }
    };

    virtual Hardware *GetHardwareInstance() = 0;

public:
    static Hardware *getInstance();

    // NOTE - Destructor will never be executed, while the static instance is never deleted.
    ~Hardware() {
        for (uint8_t i = 0; i < MAX_SENSORS; i++) {
            if (sensors[i] != nullptr) {
                delete sensors[i];
                sensors[i] = nullptr;
            }
        }
    }

    void updateSensorValues();
    virtual int16_t calculateRawValue(AxisType_t axistype) = 0;

    virtual void setAnalogReference(const bool isDebug = false);

    void attachObserver(IObserver *observer); // REFACTOR - Move to Interface!
    void detachObserver(IObserver *observer); // REFACTOR - Move to Interface!
    void clearObservers();                    // REFACTOR - Move to Interface!
    void notifyObservers();                   // Notify all observers of changes //REFACTOR - Move to Interface!

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
