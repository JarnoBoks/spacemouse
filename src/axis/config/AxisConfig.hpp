#pragma once

#include "AxisDirectionConfig.hpp" // For the DirectionConfig class
#include "axis/axes/axistype.h"    // For AxisType_t enum

class IPrinterVisitor;

class AxisConfig {
private:
    bool retrieve(const AxisType_t axisType);

public:
    AxisDirectionConfig posConfig; // Object for positive direction configuration
    AxisDirectionConfig negConfig; // Object for negative direction configuration
    bool inversion = false;        // Inversion flag for the axis

    ~AxisConfig() = default; // Default destructor
    AxisConfig();

    AxisConfig(const AxisType_t axisType);

    AxisConfig(const float psens,
               const float nsens,
               const uint8_t pgate,
               const uint8_t ngate,
               const ModFunc_t pmf,
               const ModFunc_t nmf,
               const bool invert);

    void persist(const AxisType_t axisType) const;

    void accept(IPrinterVisitor &visitor);
};
