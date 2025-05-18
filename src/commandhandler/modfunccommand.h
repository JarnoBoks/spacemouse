#pragma once

#include "IAxisConfigCommand.h"

/// @brief command text for the ModFunc configuration command
static const char CMD_MODFUNC[] PROGMEM = "MODFUNC";

class ModFuncCommand : public IAxisConfigCommand {
private:
    void dir_config_updater(AxisDirectionConfig *axisDirectionConfig) override final;

public:
    /**
     * @brief Constructor for the ModFuncCommand class.
     * @param collectionCarrier Pointer to the CollectionCarrier for accessing collections.
     * @details Initializes the command with the CMD_MODFUNC command name.
     */
    ModFuncCommand(CollectionCarrier *collectionCarrier) : IAxisConfigCommand(CMD_MODFUNC, collectionCarrier) {}
    ~ModFuncCommand() {};

    void execute(const char *param1, const char *param2, uint8_t paramCount) override;
};
