#include "IAxisConfigCommand.h"

#include "kinematics/kinematics.h"
#include "axis/config/AxisConfig.hpp"
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
 * @brief Executes the minmax command based on the provided parameters.
 * @param param1 First parameter
 * @param param2 Second parameter
 * @param paramCount Number of parameters provided.
 */
// NOTE - Paramcount 0 & 1 is equal for all derived classes, so we can use the same function for all commands that have no parameters.
void IAxisConfigCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {
    if (paramCount == 0) {
        // No params provided, show config
        ESP_PRINT(F("IAxisConfigCommand::execute: Show config"));

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
        ESP_PRINT(F("IAxisConfigCommand::execute: First parameter: "));
        ESP_PRINT(param1);
    }

    if (paramCount == 2) {
        // Command received: SENS <+|-><axisname> <value>
        // TODO - Add functionality for the second parameter
        ESP_PRINT(F("IAxisConfigCommand::execute: Second parameter: "));
        ESP_PRINT(param2);

        // Get the value that has to be set
        if (!convertWordFloat(param2, &_requestedValue)) {
            ESP_PRINT(F("IAxisConfigCommand::execute: Second parameter is not a float"));
            return; // Second parameter is not a float
        }

        // Get the direction (+ is maximum, - is minimum)
        char direction = param1[0]; // Get the first character of the first parameter

        // Get the axis from the axis name
        char *reqAxisName = (char *)param1 + 1;                  // Get the axis name (skip the first character)
        _axis = Kinematics::getInstance()->getAxis(reqAxisName); // Get the axis by its name

        // REVIEW - Failsafe: Axis not found can be removed from Arduino.
        if (_axis == nullptr) {
            ESP_PRINT(F("IAxisConfigCommand::execute: Axis not found"));
            return; // Axis not found, exit the function
        }

        if (direction == '+') {
            // Set the maximum value for the sensor
            ESP_PRINT(F("IAxisConfigCommand::execute: Set positive dir for axis "));
            _directionConfig = &_axis->getConfig()->posConfig; // Set the direction config to the positive direction

        } else if (direction == '-') {
            // Set the minimum value for the sensor
            ESP_PRINT(F("IAxisConfigCommand::execute: Set negative dir for axis "));
            _directionConfig = &_axis->getConfig()->negConfig; // Set the direction config to the negative direction

        } else {
            ESP_PRINT(F("IAxisConfigCommand::execute: Unknown command"));
            return; // Invalid direction, exit the function
        }
    }
}