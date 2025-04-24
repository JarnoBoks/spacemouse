#ifndef AXISCALIBRATIONMANAGER_H
#define AXISCALIBRATIONMANAGER_H

#include <Arduino.h>
#include "axis.h"
// #include "calibrationcommand.h"
#include "modifierfunction.h"

class AxisCalibrationManager {
private:
    /*CalibrationCommand *commands[MAX_AXIS_COMMANDS];
    uint8_t commandCount = 0;*/
    void _calibrate();

public:
    void calibrateSensitivity(Axis *axis, bool isPositive, float sensitivity);
    void calibrateGate(Axis *axis, bool isPositive, int gate);
    void calibrateInvert(Axis *axis, bool invert);
    void calibrateModifier(Axis *axis, bool isPositive, ModFunc_t modifier);
};

#endif // AXISCALIBRATIONMANAGER_H