#include "MinMaxCommand.h"
#include <commandhandler/CollectionCarrier/CollectionCarrier.hpp>

#include <sensor/calibrator/Calibrator.hpp>
#include <sensor/calibrator/states/CalibratorStateMinMax.hpp>

#include <sensor/SensorCollection.hpp>
#include <visitors/printers/SensorMinMaxPrinter.hpp>

#include <common/esp_print.h> // For ESP_PRINT

/**
 * @brief Executes the minmax command based on the provided parameters.
 * @param param1 First parameter
 * @param param2 Second parameter
 * @param paramCount Number of parameters provided.
 */
void MinMaxCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {

    if (!getCollectionCarrier()) {
        ESP_ERROR("No collection identifier");
        return;
    }

    SensorCollection *sensorCollection = getCollectionCarrier()->getSensorCollection();
    if (!(sensorCollection)) {
        ESP_ERROR("No sensor collection");
        return;
    }

    if (paramCount == 0) {
        // No params provided, show current MinMax configuration values of the sensors.
        // This will print the current min, max and range values of the sensors

        SensorMinMaxPrinter MinMaxPrinter;
        sensorCollection->accept(MinMaxPrinter); // Accept the Printer visitor to print the information for the sensors

        return;
    }

    if (paramCount == 1) {
        bool requestedPersistence = false;
        if (!convertWordBool(param1, &requestedPersistence)) {
            ESP_WARN("Param not boolean");
            return;
        }

        Calibrator *calibrator = sensorCollection->getCalibrator(); // Get the calibrator instance from the sensor collection
        RETURN_E_IF_NULL(calibrator, "No calibrator found in sensor collection");

        if (!calibrator->start(new CalibratorStateMinMax(requestedPersistence))) {
            ESP_WARN("Failed to start minmax calibration");
            return;
        }
    }

    if (paramCount == 2) {
        // Command received: MINMAX <+|-><sensorname> <value>

        // Get the value that has to be set
        const long requestedValue = 0; // Default value for the second word
        if (!convertWordNumber(param2, (long *)&requestedValue)) {
            ESP_WARN("Param is not a number");
            return;
        }

        // Get the direction (+ is maximum, - is minimum)
        const char direction = param1[0]; // Get the first character of the first parameter

        // Get the sensor from the sensor descriptor (fe. HES0 = 1, HES1 = 2, etc.)
        const char *reqSensorDescr = (char *)param1 + 1;              // Get the sensor descriptor (skip the first character)
        Sensor *sensor = sensorCollection->getSensor(reqSensorDescr); // Get the sensor by its descriptor
        if (!sensor) {
            ESP_ERROR("Sensor not found");
            return;
        }

        if (direction == '+') {
            // Set the maximum value for the sensor
            ESP_INFO("Set max for sensor ");
            sensor->getConfig()->setMax(requestedValue);

        } else if (direction == '-') {
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
        sensor->getConfig()->persist();
        ESP_INFO("Store minmax for sensor");
    }
}