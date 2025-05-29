#pragma once

#include <common/ICollectable.hpp>
#include <base/visitable/VisitableBase.hpp>
#include <stdint.h>

class SensorConfig; // Forward declaration of SensorConfig class

/**
 * @brief Base class representing a sensor, implementing the ICollectable interface.
 * @details This class is a base class for different types of sensors. It provides common functionality for reading and processing sensor values.
 * @note The Sensor class is designed to be inherited by specific sensor types, such as HallSensor or JoystickSensor.
 *        It provides a common interface for reading and processing sensor values, as well as managing the sensor's configuration.
 */
class Sensor : public ICollectable, public VisitableBase {
private:
    const int8_t pin = -1;          // Default pin value to indicate uninitialized state
    const int8_t id = -1;           // Default id value to indicate uninitialized state
    SensorConfig *config = nullptr; // REVIEW const?

    int m_rawValue = 0; // Sensor raw value as read from the AD converter
    int m_cntValue = 0; // Sensor value after centering is applied
    int m_finValue = 0; // Sensor value after deadzone correction and mapping is applied (final value).
    int idleposition = 0;

    void readValue();
    void applyCalibration();

protected:
    // REFACTOR - Move to PROGMEM
    const char *descriptor = nullptr;

public:
    Sensor() = delete; // Delete the default constructor to prevent instantiation without parameters
    Sensor(const int8_t pin, const int8_t id);
    virtual ~Sensor();

    const bool hasDescriptor(const char *descriptor) const override;

    /// @brief Retrieve the configuration object of the Sensor.
    inline SensorConfig *getConfig() const { return config; };

    inline int getIdlePosition() const { return idleposition; }

    virtual bool setIdlePosition(int val);
    virtual bool idlePositionWarning(const int val) const = 0; // Pure virtual function to be implemented by derived classes

    /// @brief Retrieve the raw sensor value, the value as read from the AD converter.
    inline int getRawValue() const { return m_rawValue; }

    /// @brief Retrieve the centered sensor value, the value after the IdlePosition offset is applied.
    inline int getCntValue() const { return m_cntValue; }

    /// @brief Retrieve the final sensor value, the value after deadzone correction and mapping is applied.
    inline int getFinValue() const { return m_finValue; }

    inline const char *getDescriptor() const { return descriptor; }
    inline const uint8_t getId() const { return static_cast<uint8_t>(id); };

    virtual void setContext(ICollection *Collection) override {};

    /**
     * @brief Evaluate the sensor value and apply calibration.
     * @details This function reads the sensor value and applies calibration to it.
     */
    void evaluate() override {
        readValue();        // Call the readValue function to update the sensor value
        applyCalibration(); // Call the applyCalibration function to process the sensor value
    };
};
