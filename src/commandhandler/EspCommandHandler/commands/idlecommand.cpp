#include "IdleCommand.h"

#include <commandhandler/CollectionCarrier/CollectionCarrier.hpp>
#include <sensor/calibration/SensorCalibrationManagerIdle.hpp>
#include <common/esp_print.h>

/**
 * @brief Destructor for the IdleCommand class.
 * @details Cleans up the sensor calibration manager instance when switching to another debug state.
 */
IdleCommand::~IdleCommand() {
    delete m_SensorCalibrationManager; // Clean up the sensor calibration manager instance when switching to another debug state
}

/**
 * @brief Executes the idle command to start calibration.
 * @details This function is called when the IDLE command is received.
 *          It activates the idle calibration in the SensorCalibrationManager with a specified number of iterations.
 * @param param1 Unused parameter.
 * @param param2 Unused parameter.
 * @param paramCount Number of parameters passed.
 */
void IdleCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {
    if (getCollectionIdentifier() == nullptr) {
        ESP_ERROR("No collection identifier");
        return;
    }
    if (getCollectionIdentifier()->getSensorCollection() == nullptr) {
        ESP_ERROR("No sensor collection");
        return;
    }
    SensorCollection *sensorCollection = getCollectionIdentifier()->getSensorCollection();

    SensorCalibrationManagerIdle *m_SensorCalibrationManager = new SensorCalibrationManagerIdle(sensorCollection); // Create a new instance of the sensor calibration manager
    m_SensorCalibrationManager->activate(2000);                                                                    // Start the idle calibration with 2000 iterations
}