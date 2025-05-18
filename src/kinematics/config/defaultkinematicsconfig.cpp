#include "defaultkinematicsconfig.hpp"
#include "config.h" // Include the config file to know the hardware type
#include "kinematicsconfig.hpp"

#if defined(HW_HALLEFFECT)
#include "defaults_hall.h"
#elif defined(HW_JOYSTICK)
#include "defaults_joystick.h"
#else
#error "No hardwaretype defined"
#endif

/**
 * @brief Get the default configuration for the kinematics.
 * @return The default KinematicsConfig.
 */
KinematicsConfig DefaultKinematicsConfig::create() {
    return KinematicsConfig(DEF_EXCLUSIVEMODE, DEF_SWITCHYZ);
}
