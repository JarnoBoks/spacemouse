#pragma once
#include "KnobVectorConfig.hpp"

/**
 * @brief Singleton class for default axis configuration.
 * This class reads the default KnobMotionVector parameters from config.h and provides them to the KnobVectorConfig class.
 * It is used to set the default values for the axis configuration when the KnobVectorConfig class is instantiated and the EEPROM is empty or the version number is changed.
 */
class DefaultKnobVectorConfig {
private:
    static DefaultKnobVectorConfig *instance;
    DefaultKnobVectorConfig(); // Private constructor to prevent instantiation

public:
    static DefaultKnobVectorConfig &getInstance();
    KnobVectorConfig getDefaultConfig(MotionVector_t type);
};
