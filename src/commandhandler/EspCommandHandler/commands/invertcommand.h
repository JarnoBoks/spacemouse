#pragma once

#include "IAxisConfigCommand.h"
/// @brief command text for the Invert configuration command
static const char CMD_INVERT[] PROGMEM = "INVERT";

class InvertCommand : public IAxisConfigCommand {
private:
public:
    /**
     * @brief Constructor for the InvertCommand class.
     * @param collectionCarrier Pointer to the CollectionCarrier for accessing collections.
     * @details Initializes the command with the CMD_INVERT command name.
     */
    InvertCommand(CollectionCarrier *collectionCarrier) : IAxisConfigCommand(CMD_INVERT, collectionCarrier) {}
    ~InvertCommand() {};

    void execute(const char *param1, const char *param2, uint8_t paramCount) override;
};
