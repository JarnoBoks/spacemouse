// exclusivecommand.h
#pragma once

#include "CommandBase.hpp"

/// @brief command text for the Exclusive mode command
static const char CMD_EXCLUSIVE_P[] PROGMEM = "EXCL"; // Command name for the SHOW command

/**
 * @brief Class to handle the EXCLUSIVE command.
 * The EXCLUSIVE command is used to set the exclusive mode for the device.
 * @note This command does not take any parameters.
 */
class ExclusiveCommand : public CommandBase {
private:
public:
    ExclusiveCommand() : CommandBase(CMD_EXCLUSIVE_P) {} // Constructor to initialize command name

    void execute(const char *param1, const char *param2, const uint8_t paramCount) override;
};
