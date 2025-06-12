#pragma once

#include <common/ICollectable.hpp>
#include <base/visitable/VisitableBase.hpp>
#include <stdint.h>

class SensorConfig; // Forward declaration of SensorConfig class

/**
 * @brief   Base class representing a sensor, implementing the ICollectable interface.
 * @details This class is a base class for different types of sensors. It provides common functionality for reading and processing sensor values.
 *          The idleposition and the deadzone are calculated on startup by the Idle calibration process.
 *          The sensor value is read from the analog-to-digital converter (ADC) and processed to obtain the final value.
 *          The class also provides methods for setting and getting the sensor's configuration, idle position, deadzone, and final value.
 *          SensorConfiguration is managed through the SensorConfig class, which holds the minimum and maximum raw values and the inversion state.
 * @note    The Sensor class is designed to be inherited by specific sensor types, such as HallSensor or JoystickSensor.
 *          It provides a common interface for reading and processing sensor values, as well as managing the sensor's configuration.
 */
class Sensor : public ICollectable, public VisitableBase {
private:
    const int8_t pin = -1;          // Default pin value to indicate uninitialized state
    const int8_t id = -1;           // Default id value to indicate uninitialized state
    SensorConfig *config = nullptr; // REVIEW const?

    int m_rawValue = 0; // Sensor raw value as read from the AD converter
    int m_cntValue = 0; // Sensor value after centering is applied
    int m_finValue = 0; // Sensor value after deadzone and mapping applied (final value).

    int m_idleposition = 0;         // The sensor rawValue at which the sensor is considered idle (centered).   //TODO - Should the default value be somewhere in the middle of the range?
    uint8_t m_deadzone = UINT8_MAX; // Default deadzone value, 255 means high deadzone to avoid jittering if the calibration has not finished.

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

    /// @brief Retrieve the idle position of the sensor.
    inline int getIdlePosition() const { return m_idleposition; }
    virtual bool setIdlePosition(int val);
    virtual bool idlePositionOk() const = 0; // Pure virtual function to be implemented by derived sensor classes

    /// @brief Retrieve the deadzone of the sensor.
    inline uint8_t getDeadzone() const { return m_deadzone; }
    virtual bool setDeadzone(const uint8_t dz);
    virtual bool isDeadzoneOk() const = 0; // Pure virtual function to be implemented by derived sensor classes

    /// @brief Retrieve the raw sensor value
    /// @return The raw sensor value as read from the AD converter.
    inline int getRawValue() const { return m_rawValue; }

    /// @brief Retrieve the centered sensor value.
    /// @details This value is the raw sensor value adjusted by the idle position.
    inline int getCntValue() const { return m_cntValue; }

    /// @brief Retrieve the final sensor value.
    /// @details This value is the centered sensor value adjusted by the deadzone and mapping.
    inline int getFinValue() const { return m_finValue; }

    /// @brief Retrieve the descriptor of the sensor.
    /// @details The descriptor is a string that describes the sensor, used for identification and logging.
    inline const char *getDescriptor() const { return descriptor; }

    /// @brief Retrieve the pin number of the sensor.
    /// @details The pin number is used to identify the physical pin on the microcontroller to which the sensor is connected.
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
