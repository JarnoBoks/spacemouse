#pragma once

#include "common/ICollectable.hpp"
#include "common/Observable.hpp"
#include <motionvector/MotionVectorType.h>            // Include the header file for MotionVector_t enum
#include <motionvector/config/MotionVectorConfig.hpp> // Include the header file for MotionVectorConfig class
#include <printervisitors/IPrinterVisitor.h>          // Include the header file for IPrinterVisitor interface, for visitor.visit() method  // REFACTOR - Use IVisitor instead of IPrinterVisitor

// REFACTOR - Move to PROGMEM
constexpr const char *c_MOTIONVECTOR_DESCRIPTORS[] = {"TX", "TY", "TZ", "RX", "RY", "RZ"}; // Axis names for serial output, ordered by the MotionVector_t.

/**
 * @brief   Base class representing a MotionVector in the system.
 * @details This class implements the ICollectable interface and provides functionality for managing the MotionVector configuration and state.
 *          It allows evaluating the MotionVector state and checking if the MotionVector is current based on its name.
 */
class MotionVector : public ICollectable, public Observable {
private:
    const MotionVector_t m_type = MotionVector_t::MV_UNINITIALIZED; // Type of the MotionVector (translation or rotation) // REVIEW - If the collections are split just like the kinematics setup, we don't need this member anymore in the vector class
    const char *m_descriptor = nullptr;                             // Descriptor of the MotionVector
    const MotionVectorConfig *m_Config = nullptr;                   // Pointer to the configuration object of the MotionVector

    int16_t m_finValue = 0; // The final value of the MotionVector after applying internal modifications.

protected:
public:
    MotionVector() = delete; // Delete default constructor, do not allow instantiation without parameters
    MotionVector(const MotionVector_t type, const uint8_t maxObservers)
        : Observable(maxObservers),
          m_type(type),
          m_descriptor(c_MOTIONVECTOR_DESCRIPTORS[static_cast<int>(type)]), // Set the name of the axis based on the MotionVector_t enum
          m_Config(new MotionVectorConfig(type)) {};                        // Create a new MotionVectorConfig object for this axis

    ~MotionVector() { delete m_Config; } // Destructor (not used in normal SpaceMouse operation)

    const bool isCurrent(const char *name) const override;

    virtual const bool isTranslation() const = 0; // REVIEW - Is this necesarry in the base class?

    /// @brief Setter for the final value of the MotionVector.
    /// @details This function sets the final value of the MotionVector and notifies observers if the value has changed.
    /// @param value The final value to set for the MotionVector.
    /// @deprecated This function is deprecated and should not be used directly.
    void setFinValue(const int16_t value) {
        bool notify = (m_finValue != value);
        m_finValue = value;
        if (notify) {
            notifyObservers();
        }
    }

    /// @brief Getter for the final Motion Vector value after applying all internal modifications.
    /// @return The final MotionVector value.
    inline int16_t getFinValue() const { return m_finValue; }

    inline const MotionVectorConfig *getConfig() const { return m_Config; } // Getter for axis configuration
    inline const MotionVector_t getType() const { return m_type; }          // Getter for MotionVector type
    inline const char *getName() const { return m_descriptor; }             // Getter for axis name

    inline void accept(IPrinterVisitor &printerVisitor) { printerVisitor.visit(*this); }
};
