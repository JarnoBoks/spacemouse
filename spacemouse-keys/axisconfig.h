#ifndef AXISCONFIG_H
#define AXISCONFIG_H

#include "directionconfig.h"
#include "eepromstore.h"
#include "axis.h" // for AxisType_t enum

class AxisConfig {
public:
    DirectionConfig posConfig;
    DirectionConfig negConfig;
    bool inversion;

    inline AxisConfig() : inversion(false) {
        // Constructor implementation (if needed)
        // Initialize the direction configurations with default values
        posConfig = DirectionConfig(1.0, 0, mfLINEAR); // Default values for positive direction
        negConfig = DirectionConfig(1.0, 0, mfLINEAR); // Default values for negative direction
    }

    inline AxisConfig(AxisType_t axisType) : inversion(false) {
        if (!EEPROMStore::loadConfig(*this, static_cast<int>(axisType))) {
            // TODO Get default values if loading from EEPROM fails
            posConfig = DirectionConfig(1.0, 0, mfLINEAR); // Default values for positive direction
            negConfig = DirectionConfig(1.0, 0, mfLINEAR); // Default values for negative direction
        }
    }

    inline void saveAxisConfig(AxisType_t axisType) {
        EEPROMStore::saveConfig(*this, static_cast<int>(axisType)); // Store the configuration in the EEPROM
    }
};

#endif // AXISCONFIG_H