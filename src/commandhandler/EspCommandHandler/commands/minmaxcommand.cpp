#include "MinMaxCommand.h"
#include <commandhandler/CollectionCarrier/CollectionCarrier.hpp>
#include <sensor/calibration/SensorCalibrationManagerMinMax.hpp>
#include <sensor/SensorCollection.hpp>
#include <sensor/sensors/Sensor.hpp>
#include <sensor/config/SensorConfig.hpp>

#include <visitors/printers/SensorConfigMinMaxPrinter.hpp>
#include <visitors/printers/SensorNamePrinter.hpp>

#include <common/esp_print.h> // For ESP_PRINT

/**
 * @brief Destructor for the MinMaxCommand class.
 * @details Cleans up the sensor calibration manager instance when switching to another debug state.
 */
MinMaxCommand::~MinMaxCommand() {
    delete m_SensorCalibrationManager;
}

/**
 * @brief Executes the minmax command based on the provided parameters.
 * @param param1 First parameter
 * @param param2 Second parameter
 * @param paramCount Number of parameters provided.
 */
void MinMaxCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {

    if (!getCollectionIdentifier()) {
        ESP_ERROR("No collection identifier");
        return;
    }

    SensorCollection *sensorCollection = getCollectionIdentifier()->getSensorCollection();
    if (!(sensorCollection)) {
        ESP_ERROR("No sensor collection");
        return;
    }

    if (paramCount == 0) {
        // No params provided, show current configuration values of the sensors.
        SensorConfigMinMaxPrinter MinMaxPrinter;
        SensorNamePrinter NamePrinter;

        // REFACTOR - Visitor should be applied to the collection, not to each sensor
        for (uint8_t id = 0; id < cHW_MAX_SENSORS; id++) {
            Sensor *sensor = sensorCollection->getSensor(id); // Pointer to the sensor
            if (sensor == nullptr) {
                continue; // Skip if the sensor is not available
            }
            sensor->accept(NamePrinter);                // Let the sensor accept the Printer visitor to print the sensor name
            sensor->getConfig()->accept(MinMaxPrinter); // Let the sensorconfig accept the Printer visitor to print the sensor configuration values
        }
        return;
    }

    if (paramCount == 1) {
        const long requestedCalibration = 0;
        if (!convertWordNumber(param1, (long *)&requestedCalibration)) {
            ESP_WARN("Param not float");
            return;
        }
        ESP_INFO2("MinMax calibration requested", requestedCalibration);

        m_SensorCalibrationManager = new SensorCalibrationManagerMinMax(sensorCollection); // Create a new instance of the sensor calibration manager
        if (requestedCalibration == 0) {
            ESP_INFO("Start minmax calibration");
            m_SensorCalibrationManager->activate();
        } else if (requestedCalibration == 1) {
            ESP_INFO("Start minmax calibration and store in EEPROM");
            m_SensorCalibrationManager->activate();
            // TODO - Store the values in EEPROM
        } else {
            ESP_WARN("Unknown command");
        }
    }
    if (paramCount == 2) {
        // Command received: MINMAX <+|-><sensorname> <value>

        // Get the value that has to be set
        const long requestedValue = 0; // Default value for the second word
        if (!convertWordNumber(param2, (long *)&requestedValue)) {
            return; // Second parameter is not a number
        }

        // Get the direction (+ is maximum, - is minimum)
        const char direction = param1[0]; // Get the first character of the first parameter

        // Get the sensor from the sensorname (fe. HES0 = 1, HES1 = 2, etc.)
        const char *reqSensorName = (char *)param1 + 1;              // Get the sensor name (skip the first character)
        Sensor *sensor = sensorCollection->getSensor(reqSensorName); // Get the sensor by its name

        if (!sensor) {
            ESP_ERROR("Sensor not found");
            return;
        }

        if (sensor != nullptr && direction == '+') {
            // Set the maximum value for the sensor
            ESP_INFO("Set max for sensor ");
            sensor->getConfig()->setMax(requestedValue);

        } else if (sensor != nullptr && direction == '-') {
            // Set the minimum value for the sensor
            ESP_INFO("Set min for sensor ");
            sensor->getConfig()->setMin(requestedValue);

        } else {
            ESP_WARN("Unknown command");
            return; // Invalid direction, exit the function
        }

        ESP_INFO2("Set minmax for sensor ", param1);
        ESP_INFO2("to ", requestedValue);

        // Store the value in the EEPROM
        sensor->getConfig()->persist(sensor->getId());
        ESP_INFO("Store minmax for sensor");
    }
}