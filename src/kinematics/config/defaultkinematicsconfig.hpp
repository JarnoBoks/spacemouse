#pragma once

class KinematicsConfig;

/**
 * @brief Class to create a default Kinematics configuration object.
 * @details This class is used to create a default Kinematics configuration object. Reads the defaults
 * for this hardware from the config.h file and/or the defaults_*.h file for the specific hardware.
 */
class DefaultKinematicsConfig {
public:
    DefaultKinematicsConfig() = default;
    ~DefaultKinematicsConfig() = default;

    KinematicsConfig create();
};
