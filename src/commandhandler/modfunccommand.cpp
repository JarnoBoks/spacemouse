#include "modfunccommand.h"
#include "axis/axisconfig.h"

// Only log to serial if not using Arduino AVR architecture
#ifndef ARDUINO_ARCH_AVR
#ifndef ESP_PRINT(x)
#define ESP_PRINT(x) Serial.println(x)
#endif
#else
#define ESP_PRINT(x)
#endif

/**
 * @brief Executes the gate command based on the provided parameters.
 * @param param1 First parameter
 * @param param2 Second parameter
 * @param paramCount Number of parameters provided.
 */
void ModFuncCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {
    ESP_PRINT(F("ModFuncCommand executed"));
    // Call the base class execute function to handle common functionality
    IAxisConfigCommand::execute(param1, param2, paramCount);

    // Check if the directionConfig is valid
    if (_directionConfig == nullptr) {
        ESP_PRINT(F("ModFuncCommand::execute: No direction config available"));
        return; // No direction config available, exit the function
    }

    _directionConfig->modFuncType = static_cast<ModFunc_t>(_requestedValue); // Set the mod function type to the requested value
    _axis->getConfig()->persist(_axis->getType());                           // Store the value in the EEPROM
}