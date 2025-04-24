#ifndef CALIBRATIONCOMMAND_H
#define CALIBRATIONCOMMAND_H

#define MAX_SENSOR_COMMANDS 4
#define MAX_AXIS_COMMANDS 4

#include <Arduino.h>
#include "axis.h"                     // For AxisType_t enum
#include "axiscalibrationmanager.h"   // For AxisCalibrationManager class
#include "sensorcalibrationmanager.h" // For SensorCalibrationManager class

// --- CalibrationCommand (Command Pattern) ---
class CalibrationCommand {
protected:
    AxisCalibrationManager *_AxisCalibrationManager = nullptr;     // Pointer to the AxisCalibrationManager instance
    SensorCalibrationManager *_SensorCalibrationManager = nullptr; // Pointer to the Sensor CalibrationManager instance
public:
    CalibrationCommand() : _AxisCalibrationManager(nullptr), _SensorCalibrationManager(nullptr) {
    }

    CalibrationCommand(AxisCalibrationManager *axscalmgr) : _AxisCalibrationManager(axscalmgr), _SensorCalibrationManager(nullptr) {
    }

    CalibrationCommand(SensorCalibrationManager *senscalmgr) : _AxisCalibrationManager(nullptr), _SensorCalibrationManager(senscalmgr) {
    }

    virtual void execute() = 0;
    virtual ~CalibrationCommand() {}
};

class CalibrateIdleCommand : public CalibrationCommand {
public:
    void execute() override;
};

class CalibrateMinMaxCommand : public CalibrationCommand {
public:
    void execute() override;
};

class CalibrateDeadzoneCommand : public CalibrationCommand {
public:
    void execute() override;
};

class CalibrateSensitivityCommand : public CalibrationCommand {
private:
    AxisType_t _axisType = AxisType_t::ALL; // Axis type to be calibrated
    bool _isPositive = true;                // Flag to indicate if the direction for this sensitivity is positive or negative
    float _sensitivity = 1.0f;              // Sensitivity value to be set
public:
    CalibrateSensitivityCommand(AxisType_t axisType = AxisType_t::ALL, bool isPositive = true, float sensitivity = 1.0f) : _axisType(axisType), _isPositive(isPositive), _sensitivity(sensitivity) {}
    inline void execute() override {
        if (_AxisCalibrationManager) {
            _AxisCalibrationManager->calibrateSensitivity(nullptr, 0); // Call the AxisCalibrationManager's calibrateSensitivity method
        }
    };
};

class CalibrateGateCommand : public CalibrationCommand {
public:
    AxisType_t _axisType = AxisType_t::ALL; // Axis type to be calibrated
    bool _isPositive = true;                // Flag to indicate if the direction for this gate is positive or negative
    uint8_t _gate = 0;                      // Gate value to be set

    CalibrateGateCommand(AxisType_t axisType = AxisType_t::ALL, bool isPositive = true, uint8_t gate = 0) : _axisType(axisType), _isPositive(isPositive), _gate(gate) {}
    inline void execute() override {
        if (_AxisCalibrationManager) {
            //_AxisCalibrationManager->calibrateGate(this); // Call the AxisCalibrationManager's calibrateGate method
        }
    };
};

class CalibrateInvertCommand : public CalibrationCommand {
private:
    AxisType_t _axisType = AxisType_t::ALL; // Axis type to be calibrated
    bool _isPositive = true;                // Flag to indicate if the direction for this inversion is positive or negative
    bool _invert = false;                   // Inversion value to be set
public:
    inline void execute() override {
        if (_AxisCalibrationManager) {
            _AxisCalibrationManager->calibrateInvert(nullptr, 0); // Call the AxisCalibrationManager's calibrateInvert method
        }
    };
};

class CalibrateModifierCommand : public CalibrationCommand {
private:
    AxisType_t _axisType = AxisType_t::ALL; // Axis type to be calibrated
    bool _isPositive = true;                // Flag to indicate if the direction for this modifier is positive or negative
    ModFunc_t _modFunc = mfLINEAR;          // Modifier function type to be set
public:
    inline void execute() override {
        if (_AxisCalibrationManager) {
            _AxisCalibrationManager->calibrateModifier(nullptr, 0);
        }
    };
};

#endif // CALIBRATIONCOMMAND_H