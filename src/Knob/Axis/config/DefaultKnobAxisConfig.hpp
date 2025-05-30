#pragma once
#include "KnobAxisConfig.hpp"

class KnobAxis;

/**
 * @brief   Class that creates the default configuration for a KnobAxis
 * @details This class reads the default KnobAxis parameters from config.h (or the for
 *          the hardware specific defaults.h). It is used to set the default values for the configuration
 *          if the KnobAxisConfig class is instantiated and the EEPROM is empty or the version number
 *          has been changed.
 */
class DefaultKnobAxisConfig {
private:
public:
    DefaultKnobAxisConfig() = default;
    ~DefaultKnobAxisConfig() = default;

    KnobAxisConfig create(const KnobAxis *axis);
};
