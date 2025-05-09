// NOTE - The software architecture is inspired by https://stackoverflow.com/questions/1820477/c-static-virtual-members

#pragma once
#define MAX_SENSORS 8

#include "common/Observable.hpp" // For IObservable interface & Base class

#include "..\axis\axes\Axis.hpp" // for AxisType enum
#include "sensor/sensors/Sensor.hpp"
#include "observers/IObserver.hpp"
#include "sensor/SensorCollection.hpp" // For SensorCollection class

class Hardware : public Observable {
protected:
    static Hardware *_instance; // Singleton instance - contains the derived hardware class

    SensorCollection *m_sensorCollection = nullptr; // Pointer to the sensor collection

    uint8_t referenceVoltage = DEFAULT;

    // Hardware can only be instantiated by derived classes
    Hardware() : Observable(MAX_HARDWARE_OBSERVERS), m_sensorCollection{new SensorCollection()} {
        m_sensorCollection->setup(); // Set up the sensor collection
    };

    virtual Hardware *GetHardwareInstance() = 0;

public:
    static Hardware *getInstance();

    virtual ~Hardware() {
        delete m_sensorCollection; // Delete the sensor collection instance to free memory
    }

    inline void evaluateSensorCollection() {
        if (m_sensorCollection)
            m_sensorCollection->evaluate(); // Evaluate the sensor collection
    };

    virtual int16_t calculateRawValue(AxisType_t axistype) = 0;

    virtual void setAnalogReference(const bool isDebug = false);

    inline Sensor *getSensor(const char *name) const {
        return (m_sensorCollection) ? m_sensorCollection->getSensor(name) : nullptr;
    }
    inline Sensor *getSensor(uint8_t id) const {
        return (m_sensorCollection) ? m_sensorCollection->getSensor(id) : nullptr;
    }
};

template <class HardwareType>
class HardwareImpl : public Hardware {
protected:
    virtual Hardware *GetHardwareInstance() {
        return HardwareType::getInstance();
    }
};
