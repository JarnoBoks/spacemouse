#pragma once

#include <motionvector/MotionVector.hpp>
#include <motionvector/MotionVectorType.h>   // Include the header file for MotionVector_t enum
#include <Knob/config/KnobVectorConfig.hpp>  // Include the header file for AxisConfig class
#include <Knob/ModifierFunctionType.h>       // Include the header file for ModFunc_t enum
#include <printervisitors/IPrinterVisitor.h> // Include the header file for IPrinterVisitor interface, for visitor.visit() method  // REFACTOR - Use IVisitor instead of IPrinterVisitor

class SensorCollection;
class ISensorsCalculator;

/// @brief Number of observers that can be added to knob MotionVector.
/// @details This is a constant value that defines the maximum number of observers that can be added to the knob MotionVector.
/// TODO @note This value is set to 1, as the knob MotionVector is only having the HID Event buffer as observer.
constexpr uint8_t c_KNOB_MAX_MOTIONVECTOR_OBSERVERS = 1;

/**
 * @brief   Class representing a knob MotionVector in the system.
 * @details This class implements the ICollectable interface and provides functionality for managing the knob MotionVector configuration and state.
 *          It allows evaluating the knob MotionVector state and checking if the knob MotionVector is current based on its name.
 */
class KnobMotionVector : public MotionVector {
private:
    ISensorsCalculator *m_sensorsCalculator = nullptr; // Pointer to the sensor calculator
    KnobVectorConfig *m_Config = nullptr;              // Pointer to the configuration object of the MotionVector

    int16_t m_rawValue = 0; // Raw computed value for the knob MotionVector, used to store the value that is calculated by the SensorsCalculator
    int16_t m_snsValue = 0; // KnobMotionVector value after applying sensitivity
    int16_t m_modValue = 0; // KnobMotionVector value after applying sensitivity & modifier function

    void modifier(ModFunc_t type);

protected:
public:
    KnobMotionVector() = delete; // Delete default constructor, do not allow instantiation without parameters
    KnobMotionVector(const MotionVector_t type, ISensorsCalculator *sensorsCalculator)
        : MotionVector(type, c_KNOB_MAX_MOTIONVECTOR_OBSERVERS),
          m_sensorsCalculator(sensorsCalculator),
          m_Config(new KnobVectorConfig(type)) {}; // Create a new KnobVectorConfig object for this axis

    ~KnobMotionVector() { delete m_Config; } // Destructor (not used in normal SpaceMouse operation)

    void evaluate() override;

    virtual const bool isTranslation() const = 0;

    inline void setRawValue(const int16_t value) { m_rawValue = value; } // Setter for raw axis value, used by SensorsCalculator

    inline int16_t getRawValue() const { return m_rawValue; } // Getter for knob MotionVector value retrieved from the SensorsCalculator
    inline int16_t getSnsValue() const { return m_snsValue; } // Getter for knob MotionVector value after applying sensitivity
    inline int16_t getModValue() const { return m_modValue; } // Getter for knob MotionVector value after applying sensitivity & modifier function

    inline const ISensorsCalculator *getSensorsCalculator() const { return m_sensorsCalculator; } // Getter for sensor calculator
    inline KnobVectorConfig *getConfig() const { return m_Config; }                               // Getter for axis configuration

    inline void accept(IPrinterVisitor &printerVisitor) { printerVisitor.visit(*this); }
};
