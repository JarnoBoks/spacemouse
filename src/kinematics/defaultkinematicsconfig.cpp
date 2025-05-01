
#include "defaultkinematicsconfig.h"
#include "config.h" // Include the config file to know the hardware type

#ifdef HALLEFFECT
#include "../defaults_hall.h"
#elif defined(JOYSTICK)
#include "../defaults_joystick.h"
#endif

// Initialize the static instance pointer to nullptr
DefaultKinematicsConfig *DefaultKinematicsConfig::instance = nullptr;

/**
 * @brief Get the instance of DefaultAxisConfig.
 * @return Reference to the singleton instance of DefaultAxisConfig.
 */
DefaultKinematicsConfig &DefaultKinematicsConfig::getInstance() {
    if (!instance) {
        instance = new DefaultKinematicsConfig();
    }
    return *instance;
}

/**
 * @brief Get the default configuration for the specified axis type.
 * @param type The axis type for which to get the default configuration.
 * @return The default AxisConfig for the specified axis type.
 */
KinematicsConfig DefaultKinematicsConfig::getDefaultConfig() {
    return KinematicsConfig(DEF_EXCLUSIVEMODE, DEF_SWITCHYZ);
}
