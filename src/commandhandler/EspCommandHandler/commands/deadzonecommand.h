#pragma once

#include "CommandBase.hpp"

/// @brief command text for the Sensor Deadzone command
static const char CMD_DZ_P[] PROGMEM = "DZ";

class DeadzoneCommand : public CommandBase {

public:
    /**
     * @brief Constructor for the DeadzoneCommand class.
     * @param collectionCarrier Pointer to the CollectionCarrier for accessing collections.
     * @details Initializes the command with the CMD_DZ command name.
     */
    DeadzoneCommand(CollectionCarrier *collectionCarrier) : CommandBase(CMD_DZ_P, collectionCarrier) {}
    ~DeadzoneCommand() {};

    void execute(const char *param1, const char *param2, uint8_t paramCount) override;
};
