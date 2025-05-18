#pragma once

#include "CommandBase.hpp"

class SensorCalibrationManager; // Forward declaration of the SensorCalibrationManager class

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
    IdleCommand(CollectionCarrier *collectionIdentifier) : CommandBase(CMD_IDLE, collectionIdentifier) {}
    ~IdleCommand();

    void execute(const char *param1, const char *param2, const uint8_t paramCount) override;
};
