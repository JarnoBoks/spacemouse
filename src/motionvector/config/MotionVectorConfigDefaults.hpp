#pragma once
#include "MotionVectorConfig.hpp"
#include "motionvector/MotionVectorType.h" // For MotionVector_t enum

class MotionVectorConfig;

/**
 * @brief Class that creates the default configuration for a MotionVector
 * @details This function will return a MotionVectorConfig instance with the default configuration
 *          for the given MotionVector type. The default configuration is retrieved from the specific
 *          hardware defaults.h file, or from the config.h file if a default value is overidden there.
 * @note This class is only used if the values EEPROM is empty or the version number has changed.
 */
class MotionVectorConfigDefaults {
private:
public:
    MotionVectorConfigDefaults() = default;
    ~MotionVectorConfigDefaults() = default;
    /**
     * @brief Creates a defaults filled MotionVectorConfig instance
     * @details This function will return a MotionVectorConfig instance with the default configuration
     *          for the given MotionVector type. The default configuration is retrieved from the specific
     *          hardware defaults.h file, or from the config.h file if a default value is overidden there.
     * @param type The MotionVector type (fe. TRANSX) for which to get the default configuration.
     * @return A MotionVectorConfig instance with the default values set.
     */
    MotionVectorConfig create(MotionVector_t type);
};
