#include "MinMaxCommand.h"
#include "calibration/sensorcalibrationmanager.h"
#include "hardware/hardware.h" // For Hardware class - necessary to retrieve the sensors.
#include "sensor/config/SensorConfig.h"
#include "visitors/MinMaxPrinter.h"

// Only log to serial if not using Arduino AVR architecture
#ifndef ARDUINO_ARCH_AVR
#ifndef ESP_PRINT(x)
#define ESP_PRINT(x) Serial.println(x)
#define ESP_DBG(x) Serial.println(x)
#endif
#else
#define ESP_PRINT(x)
#define ESP_DBG(x)
#endif

/**
 * @brief Constructor for the MinMaxCommand class.
 * @details Initializes the command with the CMD_MINMAX command name.
 */
MinMaxCommand::MinMaxCommand() : CommandBase(CMD_MINMAX) {}

/**
 * @brief Destructor for the MinMaxCommand class.
 * @details Cleans up the current debug state by deleting it.
 */
MinMaxCommand::~MinMaxCommand() {}

/**
 * @brief Executes the minmax command based on the provided parameters.
 * @param param1 First parameter
 * @param param2 Second parameter
 * @param paramCount Number of parameters provided.
 */
void MinMaxCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {
    if (paramCount == 0) {
        // No params provided, show config
        ESP_DBG(F("MinMaxCommand::execute: Show config"));

        MinMaxPrinter printer;
        // REVIEW - Move this to the hardware class
        Hardware *hardware = Hardware::getInstance(); // Get the hardware instance
        for (uint8_t id = 0; id < MAX_SENSORS; id++) {
            Sensor *sensor = hardware->sensors[id]; // Pointer to the sensor
            if (sensor == nullptr) {
                continue; // Skip if the sensor is not available
            }
            sensor->accept(printer);              // Accept the printer visitor to print the sensor name
            sensor->getConfig()->accept(printer); // Accept the printer visitor to print the sensor configuration values
        }
        return;
    }

    if (paramCount == 1) {
        long requestedCalibration = 0; // Default value for the second word
        if (!convertWordNumber(param1, (long *)&requestedCalibration)) {
            return; // First parameter is not a number
        }

        ESP_DBG(F("MinMaxCommand::execute: MinMax calibration requested: "));
        ESP_DBG(requestedCalibration);

        if (requestedCalibration == 0) {
            ESP_DBG(F("MinMaxCommand::execute: Start minmax calibration"));
            SensorCalibrationManager::getInstance()->activateMinMaxCalibration();
        } else if (requestedCalibration == 1) {
            ESP_DBG(F("MinMaxCommand::execute: Start minmax calibration and store in EEPROM"));
            SensorCalibrationManager::getInstance()->activateMinMaxCalibration();
            // TODO - Start the minmax calibration process and store the values in EEPROM
        } else {
            ESP_DBG(F("MinMaxCommand::execute: Unknown command"));
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
        char *reqSensorName = (char *)param1 + 1;                                 // Get the sensor name (skip the first character)
        Sensor *sensor = Hardware::getInstance()->getSensorByName(reqSensorName); // Get the sensor by its name

        // REVIEW - Failsafe: Sensor not found can be removed from Arduino.
        if (sensor == nullptr) {
            ESP_DBG(F("MinMaxCommand::execute: Sensor not found"));
            return; // Sensor not found, exit the function
        }

        if (sensor != nullptr && direction == '+') {
            // Set the maximum value for the sensor
            ESP_DBG(F("MinMaxCommand::execute: Set max for sensor "));
            sensor->getConfig()->setMax(requestedValue);

        } else if (sensor != nullptr && direction == '-') {
            // Set the minimum value for the sensor
            ESP_DBG(F("MinMaxCommand::execute: Set min for sensor "));
            sensor->getConfig()->setMin(requestedValue);

        } else {
            ESP_DBG(F("MinMaxCommand::execute: Unknown command"));
            return; // Invalid direction, exit the function
        }

        ESP_DBG(F("MinMaxCommand::execute: Set minmax for sensor "));
        ESP_DBG(param1);
        ESP_DBG(F(" to "));
        ESP_DBG(requestedValue);

        // Store the value in the EEPROM
        sensor->getConfig()->persist(sensor->getId());
        ESP_DBG(F("MinMaxCommand::execute: Store minmax for sensor "));
    }
}