#pragma once

#include "common/ICollectable.hpp"
#include "common/Observable.hpp"
#include "..\MotionVectorType.h"             // Include the header file for MotionVector_t enum
#include "axis/config/AxisConfig.hpp"        // Include the header file for AxisConfig class
#include <axis/ModifierFunctionType.h>       // Include the header file for ModFunc_t enum
#include <printervisitors/IPrinterVisitor.h> // Include the header file for IPrinterVisitor interface, for visitor.visit() method  // REFACTOR - Use IVisitor instead of IPrinterVisitor

class SensorCollection;
class ISensorsCalculator;

// TODO - Move to PROGMEM
constexpr const char *c_AXIS_NAMES[] = {"TX", "TY", "TZ", "RX", "RY", "RZ"}; // Axis names for serial output, ordered by the MotionVector_t.

/// @brief Number of observers that can be added to a axis.
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
    ISensorsCalculator *m_sensorsCalculator = nullptr;               // Pointer to the sensor calculator
    const MotionVector_t m_axisType = MotionVector_t::UNINITIALIZED; // Type of the axis
    const char *m_name = nullptr;                                    // Name of the axis
    AxisConfig *m_AxisConfig = nullptr;                              // Pointer to the axis configuration

    int16_t m_rawValue = 0; // Raw computed value for the axis, used to store the value that is calculated by the SensorsCalculator
    int16_t m_snsValue = 0; // Axis value after applying sensitivity
    int16_t m_modValue = 0; // Axis value after applying sensitivity & modifier function
    int16_t m_finValue = 0; // The value of the axis after applying all axis & kinematics configurations. Observed by the HID Event buffer.

    void modifier(ModFunc_t type);

protected:
public:
    Axis() = delete; // Delete default constructor, do not allow instantiation without parameters
    Axis(const MotionVector_t type, ISensorsCalculator *sensorsCalculator) : Observable(c_MAX_AXIS_OBSERVERS),
                                                                             m_sensorsCalculator(sensorsCalculator),
                                                                             m_axisType(type),
                                                                             m_name(c_AXIS_NAMES[static_cast<int>(type)]), // Set the name of the axis based on the MotionVector_t enum
                                                                             m_AxisConfig(new AxisConfig(type)) {};        // Create a new AxisConfig object for this axis

    ~Axis() { delete m_AxisConfig; } // Destructor (not used in normal SpaceMouse operation)

    void evaluate() override;

    const bool isCurrent(const char *name) const override;

    virtual const bool isTranslation() const = 0;

    inline void setRawValue(const int16_t value) { m_rawValue = value; } // Setter for raw axis value, used by SensorsCalculator

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

    inline int16_t getRawValue() const { return m_rawValue; } // Getter for raw axis value
    inline int16_t getSnsValue() const { return m_snsValue; } // Getter for axis value after applying sensitivity
    inline int16_t getModValue() const { return m_modValue; } // Getter for axis value after applying sensitivity & modifier function

    /// @brief Getter for the final axis value after applying all axis & kinematics configurations
    /// @return The final axis value after applying all axis & kinematics configurations
    inline int16_t getFinValue() const { return m_finValue; }

    inline const ISensorsCalculator *getSensorsCalculator() const { return m_sensorsCalculator; } // Getter for sensor calculator
    inline AxisConfig *getConfig() const { return m_AxisConfig; }                                 // Getter for axis configuration
    inline const MotionVector_t getAxisType() const { return m_axisType; }                        // Getter for axis type
    inline const char *getName() const { return m_name; }                                         // Getter for axis name

    inline void accept(IPrinterVisitor &printerVisitor) { printerVisitor.visit(*this); }
};
