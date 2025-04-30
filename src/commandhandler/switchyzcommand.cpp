#include "switchyzcommand.h"
#include "kinematics/kinematics.h"
#include "kinematics/kinematicsconfig.h"
#include "visitors/SwitchYZPrinter.h"

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

void SwitchYZCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {
    ESP_DBG(F("SwitchYZCommand executed"));

    if (paramCount == 0) {
        // No parameters provided, handle accordingly
        SwitchYZPrinter printer;
        Kinematics *kinematics = Kinematics::getInstance();
        kinematics->getConfig()->accept(printer); // Accept the printer visitor to print the YZ switch configuration
        return;
    }

    if (paramCount == 1) {
        // One parameter provided, handle accordingly

        long requestedLevel = 0; // Default value for the second word
        if (!convertWordNumber(param1, (long *)&requestedLevel)) {
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
        config->switchYZ = requestedLevel;
        config->persist();

        return;
    }

    return; // No action needed for two parameters in this case
}