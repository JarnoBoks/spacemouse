// minmaxcommand.h
#pragma once

#include "CommandBase.h"

/// @brief command text for the MinMax calibration command
static const char CMD_MINMAX[] PROGMEM = "MINMAX";

class SensorCalibrationManager;
class MinMaxCommand : public CommandBase {
private:
    SensorCalibrationManager *m_SensorCalibrationManager = nullptr; // Pointer to the sensor calibration manager
public:
    MinMaxCommand() = delete;

    /**
     * @brief Constructor for the MinMaxCommand class.
     * @param sensorCollection Pointer to the SensorCollection instance.
     * @details Initializes the command with the CMD_MINMAX command name.
     */
    MinMaxCommand(SensorCollection *sensorCollection) : CommandBase(CMD_MINMAX, sensorCollection) {}

    /**
     * @brief Destructor for the MinMaxCommand class.
     * @details Cleans up the sensor calibration manager instance when switching to another debug state.
     */
    virtual ~MinMaxCommand() {
        delete m_SensorCalibrationManager;
    }

    void execute(const char *param1, const char *param2, uint8_t paramCount) override;
};
