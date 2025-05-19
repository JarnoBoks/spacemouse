#pragma once

#include "ISensorsCalculator.hpp"
#include <stdint.h>
#include <Arduino.h> // For reference voltage settings

class KnobMotionVector; // Forward declaration of Axis class
class SensorCollection; // Forward declaration of SensorCollection class

/**
 * @brief Class to connect sensors to the axes.
 * @details This class translates the sensor readings to movements and is hardware specific.
 */
class SensorsCalculator : public ISensorsCalculator {
protected:
    SensorCollection *m_sensorCollection = nullptr; // Pointer to the sensor collection

    uint8_t referenceVoltage = DEFAULT;
    const int value(uint8_t const id) const;

public:
    SensorsCalculator() = default; // REFACTOR - Why doesn't delete work here?
    SensorsCalculator(SensorCollection *sensorCollection) : m_sensorCollection(sensorCollection) {};

    virtual ~SensorsCalculator() = default;

    virtual void evaluate(KnobMotionVector *axis) = 0;
    virtual void setAnalogReference(const bool isDebug = false);
};
