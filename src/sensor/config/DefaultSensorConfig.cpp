
#include "DefaultSensorConfig.hpp"
#include "SensorConfig.hpp"            // Include the SensorConfig class to get the default values
#include "config.h"                    // Include the config file to know the hardware type
#include "sensor/SensorCollection.hpp" // For cHW_MAX_SENSORS       // FIXME - Move MAX_SENSOR declaration to a better place

#if defined(HW_HALLEFFECT)
#include "defaults_hall.h"
#elif defined(HW_JOYSTICK)
#include "defaults_joystick.h"
#else
#error "No hardwaretype defined"
#endif

// FIXME - Remove the Singleton pattern.

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
    if (sensorId < 0 || sensorId >= cHW_MAX_SENSORS) {
        return SensorConfig(); // Return an empty configuration instance for invalid sensor IDs
    }

    // This function will return the default configuration for the given sensor.
    const int maxVals[cHW_MAX_SENSORS] = MAXVALS;
    const int minVals[cHW_MAX_SENSORS] = MINVALS;
    const bool invertList[cHW_MAX_SENSORS] = INVERTLIST;

    return SensorConfig(minVals[sensorId], maxVals[sensorId], invertList[sensorId], DEADZONE);
}
