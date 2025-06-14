#pragma once

#include <common/ICollectable.hpp>
#include <common/Observable.hpp>
#include <base/visitable/VisitableBase.hpp>
#include <base/axis/MotionVectorType.h> // Include the header file for MotionVector_t enum

constexpr const char *c_AXIS_DESCRIPTORS[] = {"TX", "TY", "TZ", "RX", "RY", "RZ"}; // Axis names for serial output, ordered by the MotionVector_t.

/**
 * @brief   Base class representing a Axis in the system.
 * @details This class implements the ICollectable interface and provides functionality for managing the Axis configuration and state.
 *          It allows evaluating the Axis state and checking if the Axis is current based on its name.
 */
class AxisBase : public ICollectable, public Observable, public VisitableBase {
private:
    MotionVector_t m_type = MotionVector_t::MV_UNINITIALIZED; // Type of the Axis (translation or rotation)
    const char *m_descriptor = nullptr;                       // Descriptor of the Axis

protected:
    int16_t m_finValue = 0; // The final value of the Axis after applying internal modifications.

public:
    AxisBase() = delete; // Delete default constructor, do not allow instantiation without parameters
    AxisBase(const MotionVector_t type, const uint8_t maxObservers)
        : Observable(maxObservers),
          m_type(type),
          m_descriptor(c_AXIS_DESCRIPTORS[static_cast<int>(type)]) // Set the name of the axis based on the MotionVector_t enum
    {}
    virtual ~AxisBase() = default;

    const bool hasDescriptor(const char *descriptor) const;

    // NOTE - We shoud use derived classes for the different types of Axis (translation and rotation) instead of using this function.
    // However, the Arduino has very limited memory and we have to reduce the number of virtual functions.
    const bool isTranslation() const {
        return (m_type >= MotionVector_t::TRANSX && m_type <= MotionVector_t::TRANSZ);
    }

    /// @brief Setter for the final value of the Axis.
    /// @details This function sets the final value of the Axis and notifies observers if the value has changed.
    /// @param value The final value to set for the Axis.
    void setFinValue(const int16_t value) {
        // REVIEW bool notify = (m_finValue != value);
        m_finValue = value;
        // REVIEW if (notify) {
        notifyObservers(); // REVIEW - Do we have any issues if an observer isn't called if there is no change? (fe. in printing)
        // }
    }

    /// @brief Getter for the final Axis value after applying all internal modifications.
    /// @return The final MotionVector value.
    inline int16_t getFinValue() const { return m_finValue; }

    void setType(const MotionVector_t type) { m_type = type; }        // Setter for the Axis type
    inline const MotionVector_t getType() const { return m_type; }    // Getter for Axis type
    inline const char *getDescriptor() const { return m_descriptor; } // Getter for Axis descriptor
};
