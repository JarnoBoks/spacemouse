// idlecommand.h
#pragma once

#include "CommandBase.h"
#include "..\sensor\calibration\SensorCalibrationManager.hpp" // Include the header file for the SensorCalibrationManager class

/// @brief command text for the Idle Calibration command
static const char CMD_IDLE[] PROGMEM = "IDLE";

/**
 * @brief Class to handle the IDLE command.
 * @details  The IDLE command is used to set the idle state of the device and calculate the centerpositions of the sensors.
 *           This command does not take any parameters.
 */
class IdleCommand : public CommandBase {
private:
    SensorCalibrationManager *m_SensorCalibrationManager = nullptr;

public:
    IdleCommand() = delete;
    IdleCommand(CollectionIdentifier *collectionIdentifier) : CommandBase(CMD_IDLE, collectionIdentifier) {}
    ~IdleCommand() {
        delete m_SensorCalibrationManager; // Clean up the sensor calibration manager instance when switching to another debug state
    }
    void execute(const char *param1, const char *param2, const uint8_t paramCount) override;
};
