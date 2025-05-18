#pragma once

#include "common/ICollectable.hpp"
#include "common/Observable.hpp"
#include "axistype.h"                        // Include the header file for AxisType_t enum
#include "axis/config/AxisConfig.hpp"        // Include the header file for AxisConfig class
#include "axis/modifier/modfunctype.h"       // Include the header file for ModFunc_t enum
#include <printervisitors/IPrinterVisitor.h> // Include the header file for IPrinterVisitor interface, for visitor.visit() method  // REFACTOR - Move to cpp

class SensorCollection;
class ISensorsCalculator;
class IPrinterVisitor;

constexpr const char *c_AXIS_NAMES[] = {"TX", "TY", "TZ", "RX", "RY", "RZ"}; // Axis names for serial output        // TODO - Move to PROGMEM

/// @brief Number of observers that can be added to a axis
/// @details This is a constant value that defines the maximum number of observers that can be added to the axis.
/// @note This value is set to 1, as the axis is only having the HID Event buffer as observer.
constexpr uint8_t c_MAX_AXIS_OBSERVERS = 1;

/**
 * @brief   Class representing an axis in the system.
 * @details This class implements the ICollectable interface and provides functionality for managing the axis configuration and state.
 *          It allows evaluating the axis state and checking if the axis is current based on its name.
 */
class Axis : public ICollectable, public Observable {
private:
    ISensorsCalculator *m_sensorsCalculator = nullptr;       // Pointer to the sensor calculator
    const AxisType_t m_axisType = AxisType_t::UNINITIALIZED; // Type of the axis
    const char *m_name = nullptr;                            // Name of the axis
    AxisConfig *m_AxisConfig = nullptr;                      // Pointer to the axis configuration
    // REVIEW - Is context still necessary?  const ICollection *context = nullptr;

    int16_t m_rawValue = 0; // Raw computed value for the axis, used to store the value that is calculated by the SensorsCalculator
    int16_t m_snsValue = 0; // Axis value after applying sensitivity
    int16_t m_modValue = 0; // Axis value after applying sensitivity & modifier function
    int16_t m_finValue = 0; // The value of the axis after reading from the hardware and applying all axis & kinematics configurations.

    void modifier(ModFunc_t type);

protected:
public:
    Axis() = delete;
    Axis(const AxisType_t axisType, ISensorsCalculator *sensorsCalculator) : Observable(c_MAX_AXIS_OBSERVERS),
                                                                             m_sensorsCalculator(sensorsCalculator),
                                                                             m_axisType(axisType),
                                                                             m_name(c_AXIS_NAMES[static_cast<int>(axisType)]),
                                                                             m_AxisConfig(new AxisConfig(axisType)) {};
    ~Axis() { delete m_AxisConfig; } // Destructor (not used in normal SpaceMouse operation)

    void evaluate() override;
    const bool isCurrent(const char *name) const override;
    virtual const bool isTranslation() const = 0;

    void setContext(ICollection *Collection) override {}; // TODO - Write this function

    inline void setSensorValue(const int16_t value) { m_rawValue = value; }

    /// @brief Setter for the Final value of the axis.
    /// @details This function sets the final value of the axis and notifies observers if the value has changed.
    /// @param value The final value to set for the axis.
    void setFinValue(const int16_t value) {
        bool notify = (m_finValue != value);
        m_finValue = value;
        if (notify) {
            notifyObservers();
        }
    }

    inline int16_t getSensorValue() const { return m_rawValue; } // Getter for raw axis value   // FIXME - Change the name to getRawValue() for consistency
    inline int16_t getSnsValue() const { return m_snsValue; }    // Getter for axis value after applying sensitivity
    inline int16_t getModValue() const { return m_modValue; }    // Getter for axis value after applying sensitivity & modifier function

    /// @brief Getter for the final axis value after applying all axis & kinematics configurations
    /// @return The final axis value after applying all axis & kinematics configurations
    inline int16_t getFinValue() const { return m_finValue; }

    inline const ISensorsCalculator *getSensorsCalculator() const { return m_sensorsCalculator; } // Getter for sensor calculator
    inline AxisConfig *getConfig() const { return m_AxisConfig; }                                 // Getter for axis configuration
    inline const AxisType_t getAxisType() const { return m_axisType; }                            // Getter for axis type
    inline const char *getName() const { return m_name; }                                         // Getter for axis name

    inline void accept(IPrinterVisitor &printerVisitor) { printerVisitor.visit(*this); }
};
