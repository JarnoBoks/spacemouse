#include "IdleCommand.h"
#include "calibration/sensorcalibrationmanager.h"

void IdleCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {
    // REMOVE Serial.println("IdleCommand executed");

    // The IDLE command does not take any parameters, so param1 and param2 are not used.
    // The paramCount is also not used, but it is included for consistency with the ICommand interface.
    SensorCalibrationManager::getInstance()->activateIdleCalibration(2000); // Start the idle calibration with 2000 iterations
}