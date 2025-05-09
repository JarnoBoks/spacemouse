#pragma once

#include "common\ICollectable.hpp" // Include the ICollectable interface header file
#include <stdint.h>                // Include the standard integer types header for fixed-width integer types

class SensorConfig;    // Forward declaration of SensorConfig class
class IPrinterVisitor; // Forward declaration of IPrinterVisitor class

/**
 * @brief Base class representing a sensor, implementing the ICollectable interface.
 * @details This class is a base class for different types of sensors. It provides common functionality for reading and processing sensor values.
 * @note The Sensor class is designed to be inherited by specific sensor types, such as HallSensor or JoystickSensor.
 *        It provides a common interface for reading and processing sensor values, as well as managing the sensor's configuration.
 */
class Sensor : public ICollectable {
private:
    const int8_t pin = -1;          // Default pin value to indicate uninitialized state
    const int8_t id = -1;           // Default id value to indicate uninitialized state
    SensorConfig *config = nullptr; // REVIEW const?

    int rawvalue = 0;
    int centered = 0;
    int filtered = 0;
    int idleposition = 0;

    void readValue();
    void applyCalibration();

protected:
    const char *name = nullptr;

public:
    Sensor() = delete; // Delete the default constructor to prevent instantiation without parameters
    Sensor(const int8_t pin, const int8_t id);
    virtual ~Sensor(); // Destructor;

    const bool isCurrent(const char *name) const override;

    inline SensorConfig *getConfig() const { return config; };

    inline int getIdlePosition() const { return idleposition; }
    virtual bool setIdlePosition(int val);
    virtual bool idlePositionWarning(const int val) const = 0; // Pure virtual function to be implemented by derived classes

    inline int getFilteredValue() const { return filtered; }
    inline int getRawValue() const { return rawvalue; }
    inline int getCenteredValue() const { return centered; }

    inline const char *getName() const { return name; }
    inline const uint8_t getId() const { return static_cast<uint8_t>(id); };

    void evaluate() override {
        readValue();        // Call the readValue function to update the sensor value
        applyCalibration(); // Call the applyCalibration function to process the sensor value
    };

    virtual void setContext(ICollection *Collection) override {};

    void accept(IPrinterVisitor &visitor);
};
