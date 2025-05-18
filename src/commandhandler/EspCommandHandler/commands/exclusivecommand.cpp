#include "exclusivecommand.h"
#include "kinematics/kinematics.h"
#include "kinematics/config/kinematicsconfig.hpp"
#include "visitors/ExclusiveModePrinter.h"
#include "common/esp_print.h"

void ExclusiveCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {
    ESP_DBG(F("ExclusiveCommand executed"));

    if (paramCount == 0) {
        // No parameters provided, handle accordingly
        ExclusiveModePrinter printer;
        Kinematics *kinematics = Kinematics::getInstance();
        kinematics->getConfig()->accept(printer); // Accept the printer visitor to print the YZ switch configuration
        return;
    }

    if (paramCount == 1) {
        // One parameter provided, handle accordingly

        long requestedLevel = 0;
        if (!convertWordNumber(param1, (long *)&requestedLevel)) {
            ESP_WARN("Param not number");
            return; // First parameter is not a number
        }
#ifdef ARCH_ESP32
        if (requestedLevel < 0 || requestedLevel > 1) {
            ESP_DBG(F("Invalid parameter value. Expected 0 or 1."));
            return; // Invalid parameter value
        }
#endif
        KinematicsConfig *config = Kinematics::getInstance()->getConfig(); // Get the kinematics configuration instance
        // TODO - Check for Null pointer (on ESP)
        config->setExclusiveMode(requestedLevel);
        config->persist();

        return;
    }

    if (paramCount == 2) {
        // Not used - Ignore commands that have two parameters
    }
}