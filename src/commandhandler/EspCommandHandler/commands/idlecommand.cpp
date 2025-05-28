#include "IdleCommand.h"

#include <commandhandler/CollectionCarrier/CollectionCarrier.hpp>
#include <sensor/SensorCollection.hpp>

#include <visitors/printers/SensorIdleDeadzonePrinter.hpp>
#include <sensor/calibration/SensorCalibrationManagerIdle.hpp>

#include <common/esp_print.h>

#define PRM_MIN_IT 500  // Minimum number of iterations for idle calibration
#define PRM_MAX_IT 5000 // Maximum number of iterations for idle calibration

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

    SensorCollection *sensorCollection = getCollectionIdentifier()->getSensorCollection();
    if (!sensorCollection) {
        ESP_ERROR("No sensor collection");
        return;
    }

    if (paramCount == 0) {
        // No params provided, show the current idle positions and deadzone values of the sensors
        SensorIdleDeadzonePrinter printer;
        sensorCollection->accept(printer); // Accept the printer visitor to print the information for this sensor
        return;
    }

    if (paramCount == 1) {

        long requestedIterations = 0;
        if (!convertWordNumber(param1, (long *)&requestedIterations)) {
            ESP_WARN("Param not number");
            return;
        }

        requestedIterations = (requestedIterations < PRM_MIN_IT) ? PRM_MIN_IT : requestedIterations; // Ensure minimum iterations
        requestedIterations = (requestedIterations > PRM_MAX_IT) ? PRM_MAX_IT : requestedIterations; // Ensure maximum iterations

        SensorCalibrationManagerIdle *m_SensorCalibrationManager = new SensorCalibrationManagerIdle(sensorCollection);
        m_SensorCalibrationManager->activate(requestedIterations);
        return;
    }

    if (paramCount == 2) {
        // Not used - Ignore commands that have two parameters
        ESP_WARN("Too many parameters for IDLE command");
        return;
    }
}