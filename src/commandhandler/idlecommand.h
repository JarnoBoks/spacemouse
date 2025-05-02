// idlecommand.h
#pragma once

#include "CommandBase.h"

/// @brief command text for the Idle Calibration command
static const char CMD_IDLE[] PROGMEM = "IDLE";

/**
 * @brief Class to handle the IDLE command.
 * The IDLE command is used to set the idle state of the device.
 * This command does not take any parameters.
 */
class IdleCommand : public CommandBase {
private:
public:
    IdleCommand() : CommandBase(CMD_IDLE) {} // Constructor to initialize command name

    void execute(const char *param1, const char *param2, const uint8_t paramCount) override;
};
