#include "senscommand.h"
#include "axis/axes/Axis.hpp" // For Axis class
#include "axis/config/AxisConfig.hpp"

#include <common/esp_print.h> // For ESP_PRINT

/**
 * @brief Executes the sens command based on the provided input parameters.
 * @note This function extends the functionality of the base class IAxisConfigCommand::execute(...).
 * @param param1 First parameter
 * @param param2 Second parameter
 * @param paramCount Number of parameters provided.
 */
void SensCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {
    ESP_PRINT(F("SensCommand executed"));

    // Call the base class execute function to handle common functionality
    IAxisConfigCommand::execute(param1, param2, paramCount);

    // Check if the axis has a valid AxisDirectionConfig
    if (!m_AxisDirectionConfig) {
        ESP_PRINT(F("SensCommand::execute: No direction config available"));
        return;
    }

    m_AxisDirectionConfig->sensitivity = m_requestedValue; // Set the sensitivity value to the requested value
    m_Axis->getConfig()->persist(m_Axis->getAxisType());   // Store the value in the EEPROM
}