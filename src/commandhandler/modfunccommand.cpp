#include "modfunccommand.h"
#include "axis/axes/Axis.hpp" // For Axis class
#include "axis/config/AxisConfig.hpp"

#include <common/esp_print.h> // For ESP_PRINT

/**
 * @brief Executes the modfunct command based on the provided parameters.
 * @note This function extends the functionality of the base class IAxisConfigCommand::execute(...).
 * @param param1 First parameter
 * @param param2 Second parameter
 * @param paramCount Number of parameters provided.
 */
void ModFuncCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {
    ESP_PRINT(F("ModFuncCommand executed"));

    // Call the base class execute function to handle common functionality
    IAxisConfigCommand::execute(param1, param2, paramCount);

    // Check if the directionConfig is valid
    if (m_DirectionConfig == nullptr) {
        ESP_PRINT(F("ModFuncCommand::execute: No direction config available"));
        return; // No direction config available, exit the function
    }

    m_DirectionConfig->modFuncType = static_cast<ModFunc_t>(m_requestedValue); // Set the mod function type to the requested value
    m_Axis->getConfig()->persist(m_Axis->getAxisType());                       // Store the value in the EEPROM
}