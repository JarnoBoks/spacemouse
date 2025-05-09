#pragma once

#include "axis/modifier/modfunctype.h"

// Forward declaration of classes to avoid circular dependencies
class Axis;
class AxisConfig; // Forward declaration of AxisConfig class

class AxisCalibrationManager {
public:
    AxisCalibrationManager();
    ~AxisCalibrationManager();
    void calibrateSensitivity(Axis *axis, bool isPositive, float sensitivity);
    void calibrateGate(Axis *axis, bool isPositive, int gate);
    void calibrateInvert(Axis *axis, bool invert);
    void calibrateModifier(Axis *axis, bool isPositive, ModFunc_t modifier);
};
