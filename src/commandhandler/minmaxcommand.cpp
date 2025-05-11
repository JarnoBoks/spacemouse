#include "MinMaxCommand.h"
#include "commandhandler/collectionidentifier/CollectionIdentifier.hpp"

#include "sensor/calibration/SensorCalibrationManagerMinMax.hpp"
#include "sensor/SensorCollection.hpp"
#include "sensor/config/SensorConfig.h"
#include "visitors/MinMaxPrinter.h"

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

    if (getCollectionIdentifier() == nullptr) {
        ESP_PRINT(F("MinMaxCommand::execute: No collection identifier available"));
        return; // No collection identifier available, exit the function
    }
    if (getCollectionIdentifier()->getSensorCollection() == nullptr) {
        ESP_PRINT(F("MinMaxCommand::execute: No sensor collection available"));
        return; // No sensor collection available, exit the function
    }
    SensorCollection *sensorCollection = getCollectionIdentifier()->getSensorCollection();

    if (paramCount == 0) {
        // No params provided, show config
        ESP_PRINT(F("MinMaxCommand::execute: Show config"));

        MinMaxPrinter Printer;

        for (uint8_t id = 0; id < cHW_MAX_SENSORS; id++) {
            Sensor *sensor = sensorCollection->getSensor(id); // Pointer to the sensor
            if (sensor == nullptr) {
                continue; // Skip if the sensor is not available
            }
            sensor->accept(Printer);              // Let the sensor accept the Printer visitor to print the sensor name
            sensor->getConfig()->accept(Printer); // Let the sensorconfig accept the Printer visitor to print the sensor configuration values
        }
        return;
    }

    if (paramCount == 1) {
        long requestedCalibration = 0; // Default value for the second word
        if (!convertWordNumber(param1, (long *)&requestedCalibration)) {
            return; // First parameter is not a number
        }

        ESP_PRINT(F("MinMaxCommand::execute: MinMax calibration requested: "));
        ESP_PRINT(requestedCalibration);
        m_SensorCalibrationManager = new SensorCalibrationManagerMinMax(sensorCollection); // Create a new instance of the sensor calibration manager
        if (requestedCalibration == 0) {
            ESP_PRINT(F("MinMaxCommand::execute: Start minmax calibration"));
            m_SensorCalibrationManager->activate();
        } else if (requestedCalibration == 1) {
            ESP_PRINT(F("MinMaxCommand::execute: Start minmax calibration and store in EEPROM"));
            m_SensorCalibrationManager->activate();
            // TODO - Store the values in EEPROM
        } else {
            ESP_PRINT(F("MinMaxCommand::execute: Unknown command"));
        }
    }
    if (paramCount == 2) {
        // Command received: MINMAX <+|-><sensorname> <value>
        // TODO - Add functionality for the second parameter

        // Get the value that has to be set
        long requestedValue = 0; // Default value for the second word
        if (!convertWordNumber(param2, (long *)&requestedValue)) {
            return; // Second parameter is not a number
        }

        // Get the direction (+ is maximum, - is minimum)
        char direction = param1[0]; // Get the first character of the first parameter

        // Get the sensor from the sensorname (fe. HES0 = 1, HES1 = 2, etc.)
        char *reqSensorName = (char *)param1 + 1;                    // Get the sensor name (skip the first character)
        Sensor *sensor = sensorCollection->getSensor(reqSensorName); // Get the sensor by its name

        // REVIEW - Failsafe: Sensor not found can be removed from Arduino.
        if (sensor == nullptr) {
            ESP_PRINT(F("MinMaxCommand::execute: Sensor not found"));
            return; // Sensor not found, exit the function
        }

        if (sensor != nullptr && direction == '+') {
            // Set the maximum value for the sensor
            ESP_PRINT(F("MinMaxCommand::execute: Set max for sensor "));
            sensor->getConfig()->setMax(requestedValue);

        } else if (sensor != nullptr && direction == '-') {
            // Set the minimum value for the sensor
            ESP_PRINT(F("MinMaxCommand::execute: Set min for sensor "));
            sensor->getConfig()->setMin(requestedValue);

        } else {
            ESP_PRINT(F("MinMaxCommand::execute: Unknown command"));
            return; // Invalid direction, exit the function
        }

        ESP_PRINT(F("MinMaxCommand::execute: Set minmax for sensor "));
        ESP_PRINT(param1);
        ESP_PRINT(F(" to "));
        ESP_PRINT(requestedValue);

        // Store the value in the EEPROM
        sensor->getConfig()->persist(sensor->getId());
        ESP_PRINT(F("MinMaxCommand::execute: Store minmax for sensor "));
    }
}