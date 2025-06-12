#include "HelpCommand.h"

#include <commandhandler/CollectionCarrier/CollectionCarrier.hpp>
#include <sensor/SensorCollection.hpp>

#include <sensor/calibrator/Calibrator.hpp>
#include <sensor/calibrator/states/CalibratorStateIdle.hpp>

#include <visitors/printers/SensorIdleDeadzonePrinter.hpp>

#include <common/esp_print.h>

#define PRM_MIN_IT 500  // Minimum number of iterations for idle calibration
#define PRM_MAX_IT 5000 // Maximum number of iterations for idle calibration

/**
 * @brief Executes the info command to output information about the available commands.
 * @details This function is called when the INFO command is received.
 * @param param1 Unused parameter.
 * @param param2 Unused parameter.
 * @param paramCount Number of parameters passed.
 */
void HelpCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {
    if (getCollectionCarrier() == nullptr) {
        ESP_ERROR("No collection identifier");
        return;
    }

    SensorCollection *sensorCollection = getCollectionCarrier()->getSensorCollection();
    if (!sensorCollection) {
        ESP_ERROR("No sensor collection");
        return;
    }

    if (paramCount == 0) {
        // Placeholder - functionality for all parameters the same.
        Serial.println(F("Available commands:"));
        Serial.println(F("  - HELP: Show this help message"));
        Serial.println(F("  - INFO: Show information about the system"));
        Serial.println(F("  - MINMAX: Start min-max calibration for sensors"));
        Serial.println(F("  - IDLE: Start idle calibration for sensors"));
        Serial.println(F("  - INVERT: Invert the axis direction"));
        Serial.println(F("  - SWITCHYZ: Switch YZ axis configuration"));
        Serial.println(F("  - SENS: Set sensitivity for a specific axis"));
        Serial.println(F("  - GATE: Set gate for a specific axis"));
        Serial.println(F("  - MODFUNC: Set mod function type for a specific axis"));
        Serial.println(F("  - EXIT: Exit the command handler"));
        Serial.println(F("For more information on a specific command, type HELP <command_name>"));
    }

    if (paramCount == 1) {
        // Placeholder - functionality for all parameters the same.
    }

    if (paramCount == 2) {
        // Placeholder - functionality for all parameters the same.
    }
}