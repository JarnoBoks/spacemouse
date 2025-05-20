#pragma once

#include "common/ICollectable.hpp"
#include "common/Observable.hpp"
#include <motionvector/MotionVectorType.h>   // Include the header file for MotionVector_t enum
#include <Knob/config/KnobVectorConfig.hpp>  // Include the header file for AxisConfig class
#include <Knob/ModifierFunctionType.h>       // Include the header file for ModFunc_t enum
#include <printervisitors/IPrinterVisitor.h> // Include the header file for IPrinterVisitor interface, for visitor.visit() method  // REFACTOR - Use IVisitor instead of IPrinterVisitor

class SensorCollection;
class ISensorsCalculator;

#include <Knob/MotionVector/IMotionVector.hpp>

/// @brief Number of observers that can be added to knob MotionVector.
/// @details This is a constant value that defines the maximum number of observers that can be added to the knob MotionVector.
/// TODO @note This value is set to 1, as the knob MotionVector is only having the HID Event buffer as observer.
constexpr uint8_t c_KNOB_MAX_MOTIONVECTOR_OBSERVERS = 1;

/**
 * @brief   Class representing a knob MotionVector in the system.
 * @details This class implements the ICollectable interface and provides functionality for managing the knob MotionVector configuration and state.
 *          It allows evaluating the knob MotionVector state and checking if the knob MotionVector is current based on its name.
 */
class KnobMotionVector : public ICollectable, public Observable {
private:
    ISensorsCalculator *m_sensorsCalculator = nullptr;              // Pointer to the sensor calculator
    const MotionVector_t m_type = MotionVector_t::MV_UNINITIALIZED; // Type of the knob MotionVector (translation or rotation) // REVIEW - If the collections are split just like the kinematics setup, we don't need this member anymore in the vector class
    const char *m_descriptor = nullptr;                             // Descriptor of the knob MotionVector
    KnobVectorConfig *m_Config = nullptr;                           // Pointer to the configuration object of the MotionVector

    int16_t m_rawValue = 0; // Raw computed value for the knob MotionVector, used to store the value that is calculated by the SensorsCalculator
    int16_t m_snsValue = 0; // KnobMotionVector value after applying sensitivity
    int16_t m_modValue = 0; // KnobMotionVector value after applying sensitivity & modifier function
    int16_t m_finValue = 0; // The value of the knob MotionVector after applying configurations. Observed by the HID Event buffer.

    void modifier(ModFunc_t type);

protected:
public:
    KnobMotionVector() = delete; // Delete default constructor, do not allow instantiation without parameters
    KnobMotionVector(const MotionVector_t type, ISensorsCalculator *sensorsCalculator)
        : Observable(c_KNOB_MAX_MOTIONVECTOR_OBSERVERS),
          m_sensorsCalculator(sensorsCalculator),
          m_type(type),
          m_descriptor(c_TEMP_MOTIONVECTOR_DESCRIPTORS[static_cast<int>(type)]), // Set the name of the axis based on the MotionVector_t enum
          m_Config(new KnobVectorConfig(type)) {};                          // Create a new KnobVectorConfig object for this axis

    ~KnobMotionVector() { delete m_Config; } // Destructor (not used in normal SpaceMouse operation)

    void evaluate() override;

    const bool hasDescriptor(const char *descriptor) const override;

    virtual const bool isTranslation() const = 0;

    inline void setRawValue(const int16_t value) { m_rawValue = value; } // Setter for raw axis value, used by SensorsCalculator

    /// @brief Setter for the Final value of the knob MotionVector.
    /// @details This function sets the final value of the knob MotionVector and notifies observers if the value has changed.
    /// @param value The final value to set for the knob MotionVector.
    void setFinValue(const int16_t value) {
        bool notify = (m_finValue != value);
        m_finValue = value;
        if (notify) {
            notifyObservers();
        }
    }

    inline int16_t getRawValue() const { return m_rawValue; } // Getter for knob MotionVector value retrieved from the SensorsCalculator
    inline int16_t getSnsValue() const { return m_snsValue; } // Getter for knob MotionVector value after applying sensitivity
    inline int16_t getModValue() const { return m_modValue; } // Getter for knob MotionVector value after applying sensitivity & modifier function

    /// @brief Getter for the final knob Motion Vector value after applying all axis & kinematics configurations
    /// @return The final knob Motion Vector value after applying all axis & kinematics configurations
    inline int16_t getFinValue() const { return m_finValue; }

    inline const ISensorsCalculator *getSensorsCalculator() const { return m_sensorsCalculator; } // Getter for sensor calculator
    inline KnobVectorConfig *getConfig() const { return m_Config; }                               // Getter for axis configuration
    inline const MotionVector_t getType() const { return m_type; }                                // Getter for MotionVector type
    inline const char *getName() const { return m_descriptor; }                                   // Getter for axis name

    inline void accept(IPrinterVisitor &printerVisitor) { printerVisitor.visit(*this); }
};
