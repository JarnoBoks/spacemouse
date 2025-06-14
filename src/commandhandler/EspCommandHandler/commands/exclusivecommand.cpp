#include "exclusivecommand.h"
#include <commandhandler/CollectionCarrier/CollectionCarrier.hpp>
#include <kinematics/Kinematics.hpp>
#include <kinematics/config/kinematicsconfig.hpp>
#include <visitors/printers/ExclusiveModePrinter.hpp>

#include <common/esp_print.h>

void ExclusiveCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {
    ESP_DBG(F("ExclusiveCommand executed"));

    // REFACTOR - Let the visitor visit the kinematics object instead of calling the config directly
    if (paramCount == 0) {
        // No parameters provided, handle accordingly
        ExclusiveModePrinter printer;
        m_CollectionCarrier->getKinematics()->getConfig()->accept(printer); // Accept the printer visitor to print the exclusive mode configuration
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
        KinematicsConfig *config = m_CollectionCarrier->getKinematics()->getConfig(); // Get the kinematics configuration instance
        // TODO - Check for Null pointer (on ESP)
        config->setExclusiveMode(requestedLevel);
        config->persist();

        return;
    }

    if (paramCount == 2) {
        // Not used - Ignore commands that have two parameters
    }
}