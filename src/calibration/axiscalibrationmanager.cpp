
#include "axiscalibrationmanager.h"
#include "kinematics/kinematics.h"

#include "axis/Axis.hpp"
#include "axis/config/AxisConfig.hpp"
#include "axis/config/AxisDirectionConfig.hpp"

AxisCalibrationManager::AxisCalibrationManager() {
    // Constructor implementation (if needed)
}

AxisCalibrationManager::~AxisCalibrationManager() {
    // Destructor implementation (if needed)
}

void AxisCalibrationManager::calibrateGate(Axis *axis, bool isPositive, int gate) {
    AxisDirectionConfig *axisConfig = (isPositive) ? &axis->getConfig()->posConfig : &axis->getConfig()->negConfig;
    axisConfig->gate = gate;
    axis->getConfig()->persist(axis->getType()); // Save the updated configuration to EEPROM
}

void AxisCalibrationManager::calibrateSensitivity(Axis *axis, bool isPositive, float sensitivity) {
    AxisDirectionConfig *axisConfig = (isPositive) ? &axis->getConfig()->posConfig : &axis->getConfig()->negConfig;
    axisConfig->sensitivity = sensitivity;
    axis->getConfig()->persist(axis->getType()); // Save the updated configuration to EEPROM
}

void AxisCalibrationManager::calibrateInvert(Axis *axis, bool invert) {
    axis->getConfig()->inversion = invert;
    axis->getConfig()->persist(axis->getType()); // Save the updated configuration to EEPROM
}

void AxisCalibrationManager::calibrateModifier(Axis *axis, bool isPositive, ModFunc_t modifier) {
    AxisDirectionConfig *axisConfig = (isPositive) ? &axis->getConfig()->posConfig : &axis->getConfig()->negConfig;
    axisConfig->setModfunc(modifier);            // Set the modifier function type
    axis->getConfig()->persist(axis->getType()); // Save the updated configuration to EEPROM
}