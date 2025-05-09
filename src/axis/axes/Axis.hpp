#pragma once

#include "common/ICollectable.hpp"
#include "axistype.h"                 // Include the header file for AxisType_t enum
#include "axis/config/AxisConfig.hpp" // Include the header file for AxisConfig class

class SensorCollection;
class ISensorsCalculator;

constexpr const char *c_AXIS_NAMES[] = {"TX", "TY", "TZ", "RX", "RY", "RZ"}; // Axis names for serial output        // REFACTOR - Move to PROGMEM

class Axis : public ICollectable {
    // This class represents an axis in the system. It inherits from ICollectable and provides functionality for managing the axis configuration and state.
    // The class contains methods for evaluating the axis state and checking if the axis is current based on its name.
private:
    const ISensorsCalculator *m_sensorsCalculator = nullptr; // Pointer to the sensor calculator
    const AxisType_t m_axisType;                             // Type of the axis
    const char *m_name;                                      // Name of the axis
    AxisConfig *m_config;
    const ICollection *context = nullptr;

    int16_t m_rawValue = 0;          // Raw computed value for the axis, used to store the value that is calculated by the SensorsCalculator
    bool isKillSwitchActive = false; // Flag to indicate if the kill switch for this Axis is active         // REFACTOR - Check how to model this

public:
    Axis() = default;
    Axis(const AxisType_t axisType, const ISensorsCalculator *sensorsCalculator) : m_sensorsCalculator(sensorsCalculator),
                                                                                   m_axisType(axisType),
                                                                                   m_name(c_AXIS_NAMES[static_cast<int>(axisType)]),
                                                                                   m_config(new AxisConfig(axisType)) {};
    ~Axis() { delete m_config; } // Destructor

    void evaluate() override;
    const bool isCurrent(const char *name) const override;
    void setContext(ICollection *Collection) override;

    inline void setSensorValue(const int16_t value) { m_rawValue = value; }

    inline int16_t getSensorValue() const { return m_rawValue; }                                  // Getter for sensor value
    inline const ISensorsCalculator *getSensorsCalculator() const { return m_sensorsCalculator; } // Getter for sensor calculator
    inline const AxisType_t getAxisType() const { return m_axisType; }                            // Getter for axis type
    inline const char *getName() const { return m_name; }                                         // Getter for axis name

    void setKillSwitchActive(bool active) { isKillSwitchActive = active; } // Setter for kill switch state      // REFACTOR - Check how to model this
};
