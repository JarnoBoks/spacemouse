#pragma once

#include <base/axis/AxisBase.hpp>
#include <base/axis/MotionVectorType.h>        // Include the header file for MotionVector_t enum
#include <knob/Axis/config/KnobAxisConfig.hpp> // Include the header file for AxisConfig class
#include <knob/Axis/ModifierFunctionType.h>    // Include the header file for ModFunc_t enum
#include <visitors/printers/IPrinterVisitor.h> // Include the header file for IPrinterVisitor interface, for visitor.visit() method  // REFACTOR - Use IVisitor instead of IPrinterVisitor

class SensorCollection;
class ISensorsCalculator;

/// @brief Number of observers that can be added to knob Axis.
/// @details This is a constant value that defines the maximum number of observers that can be added to the knob Axis.
/// TODO @note This value is set to 1, as the knob Axis is only having the HID Event buffer as observer.
constexpr uint8_t c_KNOB_MAX_AXIS_OBSERVERS = 1;

/**
 * @brief   Class representing a knob Axis in the system.
 * @details This class implements the ICollectable interface and provides functionality for managing the knob Axis configuration and state.
 *          It allows evaluating the knob Axis state and checking if the knob Axis is current based on its name.
 */
class KnobAxis : public AxisBase {
private:
    ISensorsCalculator *m_sensorsCalculator = nullptr; // Pointer to the sensor calculator
    KnobAxisConfig *m_Config = nullptr;                // Pointer to the configuration object of the Axis

    int16_t m_rawValue = 0; // Raw computed value for the knob Axis, used to store the value that is calculated by the SensorsCalculator
    int16_t m_snsValue = 0; // KnobAxis value after applying sensitivity
    int16_t m_modValue = 0; // KnobAxis value after applying sensitivity & modifier function

    void modifier(ModFunc_t type);

protected:
public:
    KnobAxis() = delete; // Delete default constructor, do not allow instantiation without parameters
    KnobAxis(const MotionVector_t type, ISensorsCalculator *sensorsCalculator)
        : AxisBase(type, c_KNOB_MAX_AXIS_OBSERVERS),
          m_sensorsCalculator(sensorsCalculator),
          m_Config(new KnobAxisConfig(type)) {}; // Create a new KnobAxisConfig object for this axis

    ~KnobAxis() { delete m_Config; } // Destructor (not used in normal SpaceMouse operation)

    void evaluate() override;

    virtual const bool isTranslation() const = 0;

    inline void setRawValue(const int16_t value) { m_rawValue = value; } // Setter for raw axis value, used by SensorsCalculator

    inline int16_t getRawValue() const { return m_rawValue; } // Getter for knob Axis value retrieved from the SensorsCalculator
    inline int16_t getSnsValue() const { return m_snsValue; } // Getter for knob Axis value after applying sensitivity
    inline int16_t getModValue() const { return m_modValue; } // Getter for knob Axis value after applying sensitivity & modifier function

    inline const ISensorsCalculator *getSensorsCalculator() const { return m_sensorsCalculator; } // Getter for sensor calculator
    inline KnobAxisConfig *getConfig() const { return m_Config; }                                 // Getter for axis configuration

    inline void accept(IPrinterVisitor &printerVisitor) { printerVisitor.visit(*this); }
};
