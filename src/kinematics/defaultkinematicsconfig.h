#pragma once

#include "kinematicsconfig.h"

/**
 * @brief Singleton class for default axis configuration.
 * This class reads the default Axis parameters from config.h and provides them to the AxisConfig class.
 * It is used to set the default values for the axis configuration when the AxisConfig class is instantiated and the EEPROM is empty or the version number is changed.
 */
class DefaultKinematicsConfig {
private:
    static DefaultKinematicsConfig *instance;
    DefaultKinematicsConfig(); // Private constructor to prevent instantiation

public:
    static DefaultKinematicsConfig &getInstance();
    KinematicsConfig getDefaultConfig();
};
