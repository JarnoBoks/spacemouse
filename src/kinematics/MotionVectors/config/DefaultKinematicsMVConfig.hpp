#pragma once
#include <motionvector/MotionVectorType.h> // For MotionVector_t enum

class KinematicsMVConfig;

/**
 * @brief Class that creates the default configuration for a knob MotionVector
 * @details This class reads the default KnobMotionVector parameters from config.h (or the for
 *          the hardware specific defaults.h). It is used to set the default values for the configuration
 *          if the KnobVectorConfig class is instantiated and the EEPROM is empty or the version number
 *          has been changed.
 */
class DefaultKinematicsMVConfig {
private:
public:
    DefaultKinematicsMVConfig() = default;
    ~DefaultKinematicsMVConfig() = default;
    KinematicsMVConfig create(MotionVector_t type);
};
