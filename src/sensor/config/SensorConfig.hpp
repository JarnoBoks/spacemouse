#pragma once
#include <base/visitable/VisitableBase.hpp>
#include <stdint.h>
#include <limits.h>

/**
 * @brief Sensor configuration class
 * @details This class is used to manage the configuration of sensors, including their minimum and
 *          maximum values, inversion status, and deadzone. The sensor configuration will be retrieved
 *          from the EEPROM or Preferences store based on the sensor ID. If the configuration is not found,
 *          it will use the default values defined in the defaults files.
 */
class SensorConfig : public VisitableBase {
private:
    struct SensorConfigData_t {
        int minv = 0;
        int maxv = 0;
        bool invert = false;
        uint8_t deadzone = UINT8_MAX; // Default deadzone value, 255 means high deadzone to avoid jittering if not explicitly set
    };

    SensorConfigData_t data; // Data structure to hold the configuration values

    void _minWarning(bool *warning) const;
    void _maxWarning(bool *warning) const;

    bool retrieve(const uint8_t sensorId);

public:
    /**
     * @brief Default constructor
     * @details This constructor initializes the sensor configuration with the values that are configured
     *          in the sourcecode. This constructor is used when the sensor configuration is not found
     *          in the EEPROM or Preferences store nor in the defaults files. Called from the DefaultSensorConfig.
     * @note This constructor should not be used, but is here for failsafe purposes.
     */
    SensorConfig() = default;

    /**
     * @brief Constructor used when called from the Sensor, will try to load defaults from the EEPROM or Preferences store.
     * @param sensorId The ID of the sensor
     */
    SensorConfig(const int8_t sensorId);

    /**
     * @brief Constructor with parameters for min, max, invert and deadzone
     * @details This constructor is used when called from DefaultSensorConfig.
     *          It initializes the sensor configuration with the provided min, max, invert, and deadzone values.
     */
    SensorConfig(const int min, const int max, const bool invert);

    const int getMin(bool *warning = nullptr) const;
    void updateMin(const int val);
    void setMin(const int val, bool *warning = nullptr);

    const int getMax(bool *warning = nullptr) const;
    void updateMax(const int val);
    void setMax(const int val, bool *warning = nullptr);

    const int getRange(bool *warning = nullptr) const;

    inline bool isInverted() const { return data.invert; }
    inline void setInverted(const bool inv) { data.invert = inv; }

    inline uint8_t getDeadzone() const { return data.deadzone; }
    inline void setDeadzone(const uint8_t dz) { data.deadzone = dz; }

    void persist(const uint8_t sensorId) const;
};
