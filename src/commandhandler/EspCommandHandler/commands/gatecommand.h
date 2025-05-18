#pragma once

#include "IAxisConfigCommand.h"
/// @brief command text for the Gate configuration command
static const char CMD_GATE[] PROGMEM = "GATE";

/**
 * @brief Class to handle the Gate configuration command.
 * @details This class implements the IAxisConfigCommand interface and is responsible for executing gate configuration commands.
 * It includes methods for configuring the gate settings based on provided parameters.
 * @note Ensure to validate parameters before execution.
 */
class GateCommand : public IAxisConfigCommand {

public:
    /**
     * @brief Constructor for the GateCommand class.
     * @param collectionCarrier Pointer to the CollectionCarrier for accessing collections.
     * @details Initializes the command with the CMD_GATE command name.
     */
    GateCommand(CollectionCarrier *collectionCarrier) : IAxisConfigCommand(CMD_GATE, collectionCarrier) {}
    ~GateCommand() {}
    void execute(const char *param1, const char *param2, uint8_t paramCount) override;
};
