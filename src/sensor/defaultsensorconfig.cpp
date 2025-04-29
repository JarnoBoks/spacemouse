
#include "defaultsensorconfig.h"
#include "sensor/sensorconfig.h" // Include the SensorConfig class to get the default values
#include "config.h"              // Include the config file to know the hardware type
#include "hardware/hardware.h"   // Include the hardware class to get the number of sensors (MAX_SENSORS)

// Retrieve the defaults for this specific hardware type
// The default values are defined in the defaults_hall.h or defaults_joystick.h file
#include DEFAULTS_TYPE

// Initialize the static instance pointer to nullptr
DefaultSensorConfig *DefaultSensorConfig::instance = nullptr;

/**
 * @brief Get the instance of DefaultSensorConfig.
 * @return Reference to the singleton instance of DefaultSensorConfig.
 */
DefaultSensorConfig &DefaultSensorConfig::getInstance() {
    if (!instance) {
        instance = new DefaultSensorConfig();
    }
    return *instance;
}

/**
 * @brief DefaultSensorConfig constructor.
 * @details This constructor is private to enforce the singleton pattern.
 * It initializes the default axis configuration for the given hardware type.
 */
DefaultSensorConfig::DefaultSensorConfig() {
}

SensorConfig DefaultSensorConfig::getDefaultConfig(const int8_t sensorId) const {
    if (sensorId < 0 || sensorId >= MAX_SENSORS) {
        return SensorConfig(); // Return an empty configuration instance for invalid sensor IDs
    }

    // This function will return the default configuration for the given sensor.
    const int maxVals[MAX_SENSORS] = MAXVALS;
    const int minVals[MAX_SENSORS] = MINVALS;
    const bool invertList[MAX_SENSORS] = INVERTLIST;

    return SensorConfig(minVals[sensorId], maxVals[sensorId], invertList[sensorId], DEADZONE);
}
