#pragma once

#include "CommandBase.hpp"

/// @brief command text for the MinMax calibration command
static const char CMD_MINMAX[] PROGMEM = "MINMAX";

class SensorCalibrationManager;
class CollectionCarrier;

class MinMaxCommand : public CommandBase {
private:
    SensorCalibrationManager *m_SensorCalibrationManager = nullptr; // Pointer to the sensor calibration manager
public:
    MinMaxCommand() = delete; // Delete the default constructor

    /**
     * @brief Constructor for the MinMaxCommand class.
     * @param collectionCarrier Pointer to the CollectionCarrier for accessing collections.
     * @details Initializes the command with the CMD_MINMAX command name.
     */
    MinMaxCommand(CollectionCarrier *collectionCarrier) : CommandBase(CMD_MINMAX, collectionCarrier) {}

    virtual ~MinMaxCommand();

    void execute(const char *param1, const char *param2, uint8_t paramCount) override;
};
