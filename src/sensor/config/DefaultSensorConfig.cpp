
#include "DefaultSensorConfig.hpp"
#include "SensorConfig.hpp"            // Include the SensorConfig class to get the default values
#include "config.h"                    // Include the config file to know the hardware type
#include "sensor/SensorCollection.hpp" // For cHW_MAX_SENSORS       // REFACTOR - Move MAX_SENSOR declaration to a better place

#if defined(HW_HALLEFFECT)
#include "defaults_hall.h"
#elif defined(HW_JOYSTICK)
#include "defaults_joystick.h"
#else
#error "No hardwaretype defined"
#endif

SensorConfig DefaultSensorConfig::create(const int8_t sensorId) const {
    if (sensorId < 0 || sensorId >= cHW_MAX_SENSORS) {
        return SensorConfig(); // Return an empty configuration instance for invalid sensor IDs
    }

    // This function will return the default configuration for the given sensor.
    const int maxVals[cHW_MAX_SENSORS] = MAXVALS;
    const int minVals[cHW_MAX_SENSORS] = MINVALS;
    const bool invertList[cHW_MAX_SENSORS] = INVERTLIST;

    return SensorConfig(minVals[sensorId], maxVals[sensorId], invertList[sensorId], DEADZONE);
}
