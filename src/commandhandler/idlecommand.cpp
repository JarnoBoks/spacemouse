#include "IdleCommand.h"
#include "calibration/sensorcalibrationmanager.h"

/**
 * @brief Executes the idle command to start calibration.
 * @details This function is called when the IDLE command is received.
 *          It activates the idle calibration in the SensorCalibrationManager with a specified number of iterations.
 * @param param1 Unused parameter.
 * @param param2 Unused parameter.
 * @param paramCount Number of parameters passed.
 */
void IdleCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {
    SensorCalibrationManager::getInstance()->activateIdleCalibration(2000); // Start the idle calibration with 2000 iterations
}