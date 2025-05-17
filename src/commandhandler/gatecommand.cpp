#include "gatecommand.h"
#include "axis/axes/Axis.hpp" // For Axis class
#include "axis/config/AxisConfig.hpp"

#include <common/esp_print.h> // For ESP_PRINT

/**
 * @brief Executes the gate command based on the provided parameters.
 * @note This function extends the functionality of the base class IAxisConfigCommand::execute(...).
 * @param param1 First parameter
 * @param param2 Second parameter
 * @param paramCount Number of parameters provided.
 */
void GateCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {
    ESP_PRINT(F("GateCommand executed"));

    // Call the base class execute function to handle common functionality
    IAxisConfigCommand::execute(param1, param2, paramCount); // Call the base class execute function

    // Check if the axis has a valid AxisDirectionConfig
    if (m_AxisDirectionConfig == nullptr) {
        ESP_PRINT(F("GateCommand::execute: No direction config available"));
        return;
    }

    m_AxisDirectionConfig->gate = m_requestedValue;      // Set the gate value to the requested value
    m_Axis->getConfig()->persist(m_Axis->getAxisType()); // Store the value in the EEPROM
}