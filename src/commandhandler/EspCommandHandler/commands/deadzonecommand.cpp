#include "deadzonecommand.h"
#include <commandhandler/CollectionCarrier/CollectionCarrier.hpp>
#include <sensor/SensorCollection.hpp>
#include <sensor/sensors/Sensor.hpp>
#include <sensor/config/SensorConfig.hpp>

#include <visitors/printers/DeadzonePrinter.hpp>
#include <visitors/printers/SensorNamePrinter.hpp>

#include <common/esp_print.h> // For ESP_PRINT

/**
 * @brief Executes the deadzone command based on the provided input parameters.
 * @param param1 First parameter
 * @param param2 Second parameter
 * @param paramCount Number of parameters provided.
 * @details This function handles the deadzone command for sensors, allowing users to view or set deadzone values.
 *          If no parameters are provided, it prints the current deadzone values of all sensors.
 *
 */
void DeadzoneCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {
#if 0
    // Call the base class execute function to parse the parameters.
    IAxisConfigCommand::execute(param1, param2, paramCount);

    // No update of the configuration parameters possible or needed if the requested value is less than 0
    if (m_requestedValue < 0) {
        return;
    }

    for (uint8_t i = 0; i < NUM_AX_DIRCFG; i++) {
        if (m_knobVectorDirectionConfig[i]) {
            m_knobVectorDirectionConfig[i]->setSensitivity(m_requestedValue);
        }
    }

    m_knobVector->getConfig()->persist(m_knobVector->getType()); // Store the value in the EEPROM
#endif

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
        // No params provided, show current deadzone values of the sensors.
        DeadzonePrinter deadzonePrinter;
        SensorNamePrinter namePrinter;

        for (uint8_t id = 0; id < cHW_MAX_SENSORS; id++) {
            Sensor *sensor = sensorCollection->getSensor(id); // Pointer to the sensor
            if (sensor == nullptr) {
                continue; // Skip if the sensor is not available
            }
            sensor->accept(namePrinter);                  // Let the sensor accept the Printer visitor to print the sensor name
            sensor->getConfig()->accept(deadzonePrinter); // Let the sensorconfig accept the Printer visitor to print the sensor configuration values
        }
        return;
    }

    if (paramCount == 1) {
#if 0
        // One parameter received: set deadzone for all sensors
        const long requestedDeadzone = 0;
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
#endif
    }
    if (paramCount == 2) {
#if 0
        // Command received: MINMAX <+|-><sensorname> <value>

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
#endif
    }
}