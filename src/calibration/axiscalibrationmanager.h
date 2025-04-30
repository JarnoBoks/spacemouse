#ifndef AXISCALIBRATIONMANAGER_H
#define AXISCALIBRATIONMANAGER_H

#include <Arduino.h>
#include "axis/modifierfunction.h" // For ModFunc_t enum

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

#endif // AXISCALIBRATIONMANAGER_H