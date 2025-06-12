#pragma once

#include "CommandBase.hpp"

/// @brief command text for the Help command
static const char CMD_HELP_P[] PROGMEM = "HELP";

/// @brief Class to handle the HELP command.
class HelpCommand : public CommandBase {
private:
public:
    HelpCommand() = delete;

    /**
     * @brief Constructor for the HelpCommand class.
     * @details Initializes the command with the CMD_HELP command name.
     * @param collectionCarrier Pointer to the CollectionCarrier for accessing collections.
     */
    HelpCommand(CollectionCarrier *collectionCarrier) : CommandBase(CMD_HELP_P, collectionCarrier) {}
    ~HelpCommand() = default;

    void execute(const char *param1, const char *param2, const uint8_t paramCount) override;
};
