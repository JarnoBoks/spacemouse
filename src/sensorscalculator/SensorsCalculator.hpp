#pragma once

#include "ISensorsCalculator.hpp"
#include <stdint.h>
#include <Arduino.h> // For reference voltage settings

class Axis;             // Forward declaration of Axis class
class SensorCollection; // Forward declaration of SensorCollection class

class SensorsCalculator : public ISensorsCalculator {
protected:
    SensorCollection *m_sensorCollection = nullptr; // Pointer to the sensor collection

    uint8_t referenceVoltage = DEFAULT;
    const int value(uint8_t const id) const;

public:
    SensorsCalculator() = default; // Delete the default constructor to prevent instantiation without parameters
    SensorsCalculator(SensorCollection *sensorCollection) : m_sensorCollection(sensorCollection) {};

    virtual ~SensorsCalculator() {
    }

    virtual void evaluate(Axis *axis) = 0;
    virtual void setAnalogReference(const bool isDebug = false);
};
