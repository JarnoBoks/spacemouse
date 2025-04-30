#ifndef AXISCONFIG_H
#define AXISCONFIG_H

#include "directionconfig.h" // For the DirectionConfig class
#include "axis.h"            // For AxisType_t enum


class AxisConfig {
public:
    DirectionConfig posConfig;
    DirectionConfig negConfig;
    bool inversion;

    /** Constructor with no arguments - not used*/
    AxisConfig();

    /* Constructor with axisType as argument - used when called from an axis */
    AxisConfig(AxisType_t axisType);

    /* Constructor with parameters for sensitivity, gate, and function types - used when called from default config */
    AxisConfig(const float psens,
               const float nsens,
               const uint8_t pgate,
               const uint8_t ngate,
               const ModFunc_t pmf,
               const ModFunc_t nmf,
               const bool invert);

    /* Save the axisconfig to EEPROM */
    void saveAxisConfig(AxisType_t axisType);
};

#endif // AXISCONFIG_H