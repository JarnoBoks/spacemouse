#pragma once

#include "AxisDirectionConfig.hpp" // For the DirectionConfig class
#include "axis/axes/axistype.h"    // For AxisType_t enum

// Forward declaration of classes to avoid circular dependencies
class IPrinterVisitor;

class AxisConfig {
public:
    AxisDirectionConfig posConfig;
    AxisDirectionConfig negConfig;
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
    void persist(AxisType_t axisType);

    void accept(IPrinterVisitor &visitor);
};
