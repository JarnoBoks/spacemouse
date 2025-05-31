
#include "DefaultSensorConfig.hpp"
#include "config.h"                    // Include the config file to know the hardware type
#include <sensor/sensors/Sensor.hpp>   // For Sensor class
#include "SensorConfig.hpp"            // Include the SensorConfig class to get the default values
#include <sensor/SensorCollection.hpp> // For cHW_MAX_SENSORS       // REFACTOR - Move MAX_SENSOR declaration to hardware defaults
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
 * @param contextSensor The sensor for which to create the configuration.
 * @return The default sensor configuration
 */
SensorConfig DefaultSensorConfig::create(const Sensor *contextSensor) const {
    if (contextSensor == nullptr) {
        ESP_ERROR("Invalid sensor context");
        return nullptr; // Return an empty configuration instance for invalid sensor context
    }

    const int maxVals[cHW_MAX_SENSORS] = MAXVALS;
    const int minVals[cHW_MAX_SENSORS] = MINVALS;
    const bool invertList[cHW_MAX_SENSORS] = INVERTLIST;

    const uint8_t sensorId = contextSensor->getId();

    return SensorConfig(contextSensor, minVals[sensorId], maxVals[sensorId], invertList[sensorId]);
}
