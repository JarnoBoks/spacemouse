#include "SensCommand.h"
// REMOVE #include "hardware/hardware.h" // For Hardware class - necessary to retrieve the sensors.
// REMOVE #include "sensor/sensorconfig.h"

#include "kinematics/kinematics.h"
#include "axis/axisconfig.h"
#include "visitors/AxisConfigPrinter.h"

// Only log to serial if not using Arduino AVR architecture
#ifndef ARDUINO_ARCH_AVR
#ifndef ESP_PRINT(x)
#define ESP_PRINT(x) Serial.println(x)
#endif
#else
#define ESP_PRINT(x)
#endif

/**
 * @brief Constructor for the MinMaxCommand class.
 * @details Initializes the command with the CMD_MINMAX command name.
 */
SensCommand::SensCommand() : ICommand(CMD_SENS) {}

/**
 * @brief Destructor for the SensCommand class.
 * @details Cleans up the current debug state by deleting it.
 */
SensCommand::~SensCommand() {}

/**
 * @brief Executes the minmax command based on the provided parameters.
 * @param param1 First parameter
 * @param param2 Second parameter
 * @param paramCount Number of parameters provided.
 */
void SensCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {
    if (paramCount == 0) {
        // No params provided, show config
        ESP_PRINT(F("SensCommand::execute: Show config"));

        AxisConfigPrinter printer;
        // REVIEW - Move this to the kinematics class?
        Kinematics *kinematics = Kinematics::getInstance();
        for (uint8_t id = 0; id < AxisType_t::LENGTH; id++) {
            Axis *axis = kinematics->getAxis((AxisType_t)id); // Pointer to the axis
            if (axis == nullptr) {
                continue; // Skip if the axis is not available
            }
            axis->accept(printer); // Accept the printer visitor to print the axis configuration
        }

        return;
    }

    if (paramCount == 1) {
        ESP_PRINT(F("SensCommand::execute: First parameter: "));
        ESP_PRINT(param1);
    }

    if (paramCount == 2) {
        // Command received: SENS <+|-><axisname> <value>
        // TODO - Add functionality for the second parameter
        ESP_PRINT(F("SensCommand::execute: Second parameter: "));
        ESP_PRINT(param2);

        // Get the value that has to be set
        float requestedValue = 0; // Default value for the second word
        if (!convertWordFloat(param2, &requestedValue)) {
            ESP_PRINT(F("SensCommand::execute: Second parameter is not a float"));
            return; // Second parameter is not a float
        }

        // Get the direction (+ is maximum, - is minimum)
        char direction = param1[0]; // Get the first character of the first parameter

        // Get the axis from the axis name
        char *reqAxisName = (char *)param1 + 1;                       // Get the axis name (skip the first character)
        Axis *axis = Kinematics::getInstance()->getAxis(reqAxisName); // Get the axis by its name

        // REVIEW - Failsafe: Axis not found can be removed from Arduino.
        if (axis == nullptr) {
            ESP_PRINT(F("SensCommand::execute: Axis not found"));
            return; // Axis not found, exit the function
        }

        if (axis != nullptr && direction == '+') {
            // Set the maximum value for the sensor
            ESP_PRINT(F("SensCommand::execute: Set positive dir for axis "));
            axis->getConfig()->posConfig.sensitivity = requestedValue; // Set the sensitivity for the positive direction

        } else if (axis != nullptr && direction == '-') {
            // Set the minimum value for the sensor
            ESP_PRINT(F("SensCommand::execute: Set negative dir for axis "));
            axis->getConfig()->negConfig.sensitivity = requestedValue; // Set the sensitivity for the negative direction

        } else {
            ESP_PRINT(F("SensCommand::execute: Unknown command"));
            return; // Invalid direction, exit the function
        }

        // Store the value in the EEPROM
        axis->getConfig()->persist(axis->getType());
    }
}