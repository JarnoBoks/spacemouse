#pragma once

#include "CommandBase.hpp"

class SensorCalibrationManager; // Forward declaration of the SensorCalibrationManager class

/// @brief command text for the Idle Calibration command
static const char CMD_IDLE_P[] PROGMEM = "IDLE";

/**
 * @brief Class to handle the IDLE command.
 * @details  The IDLE command is used to show or re-calculate the idle position and deadzone
 *           of the hardware sensors.
 *           Entering the command "IDLE" without parameters, shows the current idle position and deadzone of the sensors.
 *           Entering the command "IDLE <iterations>" will re-calculate the idle position and deadzone of the sensors,
 *           in <iterations> iterations. Iterations should be a number between 500 and 5000.
 */
class IdleCommand : public CommandBase {
private:
public:
    IdleCommand() = delete;

    /**
     * @brief Constructor for the IdleCommand class.
     * @details Initializes the command with the CMD_IDLE command name.
     * @param collectionCarrier Pointer to the CollectionCarrier for accessing collections.
     */
    IdleCommand(CollectionCarrier *collectionIdentifier) : CommandBase(CMD_IDLE_P, collectionIdentifier) {}
    ~IdleCommand() = default;

    void execute(const char *param1, const char *param2, const uint8_t paramCount) override;
};
