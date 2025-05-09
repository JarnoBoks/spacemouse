#pragma once

#include "common/ICollectable.hpp"
#include "axistype.h"                  // Include the header file for AxisType_t enum
#include "axis/config/AxisConfig.hpp"  // Include the header file for AxisConfig class
#include "axis/modifier/modfunctype.h" // Include the header file for ModFunc_t enum
#include "visitors/IPrinterVisitor.h"  // Include the header file for IPrinterVisitor interface

class SensorCollection;
class ISensorsCalculator;

constexpr const char *c_AXIS_NAMES[] = {"TX", "TY", "TZ", "RX", "RY", "RZ"}; // Axis names for serial output        // REFACTOR - Move to PROGMEM

class Axis : public ICollectable {
    // This class represents an axis in the system. It inherits from ICollectable and provides functionality for managing the axis configuration and state.
    // The class contains methods for evaluating the axis state and checking if the axis is current based on its name.
private:
    ISensorsCalculator *m_sensorsCalculator = nullptr;       // Pointer to the sensor calculator
    const AxisType_t m_axisType = AxisType_t::UNINITIALIZED; // Type of the axis
    const char *m_name = nullptr;                            // Name of the axis
    AxisConfig *m_AxisConfig = nullptr;                      // Pointer to the axis configuration
    const ICollection *context = nullptr;

    int16_t m_rawValue = 0; // Raw computed value for the axis, used to store the value that is calculated by the SensorsCalculator
    int16_t m_snsValue = 0; // Axis value after applying sensitivity
    int16_t m_modValue = 0; // Axis value after applying sensitivity & modifier function
    int16_t m_finValue = 0; // The value of the axis after reading from the hardware and applying all axis & kinematics configurations.

    bool isKillSwitchActive = false; // Flag to indicate if the kill switch for this Axis is active         // REFACTOR - Check how to model this

    void modifier(ModFunc_t type);

public:
    Axis() = default;
    Axis(const AxisType_t axisType, ISensorsCalculator *sensorsCalculator) : m_sensorsCalculator(sensorsCalculator),
                                                                             m_axisType(axisType),
                                                                             m_name(c_AXIS_NAMES[static_cast<int>(axisType)]),
                                                                             m_AxisConfig(new AxisConfig(axisType)) {};
    ~Axis() { delete m_AxisConfig; } // Destructor

    void evaluate() override;
    const bool isCurrent(const char *name) const override { return false; }; // TODO - Write this function
    void setContext(ICollection *Collection) override {};                    // TODO - Write this function

    inline void setSensorValue(const int16_t value) { m_rawValue = value; }
    inline void setFinValue(const int16_t value) { m_finValue = value; }

    inline int16_t getSensorValue() const { return m_rawValue; } // Getter for raw axis value
    inline int16_t getSnsValue() const { return m_snsValue; }    // Getter for axis value after applying sensitivity
    inline int16_t getModValue() const { return m_modValue; }    // Getter for axis value after applying sensitivity & modifier function
    inline int16_t getFinValue() const { return m_finValue; }    // Getter for the final axis value after applying all axis & kinematics configurations

    inline const ISensorsCalculator *getSensorsCalculator() const { return m_sensorsCalculator; } // Getter for sensor calculator
    inline AxisConfig *getConfig() const { return m_AxisConfig; }                                 // Getter for axis configuration
    inline const AxisType_t getAxisType() const { return m_axisType; }                            // Getter for axis type
    inline const char *getName() const { return m_name; }                                         // Getter for axis name

    void setKillSwitchActive(bool active) { isKillSwitchActive = active; } // Setter for kill switch state      // REFACTOR - Check how to model this

    void accept(IPrinterVisitor &visitor) { visitor.visit(*this); }
};
