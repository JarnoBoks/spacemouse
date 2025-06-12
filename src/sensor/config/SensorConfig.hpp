#pragma once
#include <base/visitable/VisitableBase.hpp>
#include <stdint.h>
#include <limits.h>

class Sensor;

/**
 * @brief Sensor configuration class
 * @details This class is used to manage the configuration of sensors, including their minimum and
 *          maximum values, inversion status, and deadzone. The sensor configuration data will be
 *          retrieved from the non volatile memory. If the non volatile memory does not contain a
 *          valid configuration for the sensor, the sensor configuration will be setup according to
 *          default values defined in the hardware corresponding default files if the configuration value
 *          isn't overridden in config.h.
 */
class SensorConfig : public VisitableBase {
private:
    struct SensorConfigData_t {
        int minv = 0;        // The minimum centered value of the sensor.
        int maxv = 0;        // The maximum centered value of the sensor.
        bool invert = false; // Flag to indicate that the sensor value should be inverted (ie. magnet is mounted upside down / joystick is mounted the wrong way).
    };

    SensorConfigData_t data;                 // Data structure to hold the configuration values.
    const Sensor *m_contextSensor = nullptr; // Pointer to the Sensor object that this configuration belongs to.

    void _minWarning(bool *warning) const;
    void _maxWarning(bool *warning) const;

    bool retrieve();

public:
    SensorConfig() = delete; // Default constructor is deleted to prevent instantiation without context
    SensorConfig(const Sensor *contextSensor);
    SensorConfig(const Sensor *contextSensor, const int min, const int max, const bool invert);

    ~SensorConfig() = default; // Default destructor

    const int getMin(bool *warning = nullptr) const;
    void updateMin(const int val);
    void setMin(const int val, bool *warning = nullptr);

    const int getMax(bool *warning = nullptr) const;
    void updateMax(const int val);
    void setMax(const int val, bool *warning = nullptr);

    const int getRange(bool *warning = nullptr) const;

    inline bool isInverted() const { return data.invert; }
    inline void setInverted(const bool inv) { data.invert = inv; }

    void persist() const;
};
