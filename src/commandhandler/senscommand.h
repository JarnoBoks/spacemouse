#pragma once

#include "IAxisConfigCommand.h"

/// @brief command text for the Sensitivity configuration command
static const char CMD_SENS[] PROGMEM = "SENS";

class SensCommand : public IAxisConfigCommand {
protected:
    void dir_config_updater(AxisDirectionConfig *axisDirectionConfig) override final;

public:
    /**
     * @brief Constructor for the SensCommand class.
     * @param collectionCarrier Pointer to the CollectionCarrier for accessing collections.
     * @details Initializes the command with the CMD_SENS command name.
     */
    SensCommand(CollectionCarrier *collectionCarrier) : IAxisConfigCommand(CMD_SENS, collectionCarrier) {}
    ~SensCommand() {};

    void execute(const char *param1, const char *param2, uint8_t paramCount) override;
};
