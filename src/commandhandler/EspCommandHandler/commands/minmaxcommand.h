#pragma once

#include "CommandBase.hpp"

/// @brief command text for the MinMax calibration command
static const char CMD_MINMAX_P[] PROGMEM = "MINMAX";

class SensorCalibrationManager;
class CollectionCarrier;

class MinMaxCommand : public CommandBase {
public:
    MinMaxCommand() = delete; // Delete the default constructor

    /**
     * @brief Constructor for the MinMaxCommand class.
     * @details Initializes the command with the CMD_MINMAX command name.
     * @param collectionCarrier Pointer to the CollectionCarrier for accessing collections.
     */
    MinMaxCommand(CollectionCarrier *collectionCarrier) : CommandBase(CMD_MINMAX_P, collectionCarrier) {}

    virtual ~MinMaxCommand() = default; // Default destructor

    void execute(const char *param1, const char *param2, uint8_t paramCount) override;
};
