
#include "DefaultSensorConfig.hpp"
#include "config.h"                    // Include the config file to know the hardware type
#include "SensorConfig.hpp"            // Include the SensorConfig class to get the default values
#include <sensor/SensorCollection.hpp> // For cHW_MAX_SENSORS       // REFACTOR - Move MAX_SENSOR declaration to a better place
#include <common/esp_print.h>          // For ESP_ERROR() and ESP_DEBUG() macros

#if defined(HW_HALLEFFECT)
#include "defaults_hall.h"
#elif defined(HW_JOYSTICK)
#include "defaults_joystick.h"
#else
#error "No hardwaretype defined"
#endif

/**
 * @brief Create a sensor configuration, filled with values as defined in the defaults files.
 * @param sensorId The ID of the sensor
 * @return The default sensor configuration
 */
SensorConfig DefaultSensorConfig::create(const int8_t sensorId) const {
    if (sensorId < 0 || sensorId >= cHW_MAX_SENSORS) {
        ESP_ERROR("Invalid sensor ID");
        return SensorConfig(); // Return an empty configuration instance for invalid sensor IDs
    }

    const int maxVals[cHW_MAX_SENSORS] = MAXVALS;
    const int minVals[cHW_MAX_SENSORS] = MINVALS;
    const bool invertList[cHW_MAX_SENSORS] = INVERTLIST;

    return SensorConfig(minVals[sensorId], maxVals[sensorId], invertList[sensorId]);
}
