#include "invertcommand.h"
#include "axis/axes/Axis.hpp"
#include "axis/config/AxisConfig.hpp"

#include <common/esp_print.h> // For ESP_PRINT

/**
 * @brief Executes the invert command based on the provided parameters.
 * @note This function extends the functionality of the base class IAxisConfigCommand::execute(...).
 * @param param1 First parameter
 * @param param2 Second parameter
 * @param paramCount Number of parameters provided.
 */
void InvertCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {

    // Call the base class execute function to handle common functionality
    IAxisConfigCommand::execute(param1, param2, paramCount); // Call the base class execute function

    // Check if the axis has a valid AxisDirectionConfig
    if (!m_AxisDirectionConfig) {
        ESP_WARN("No direction config");
        return;
    }

    AxisConfig *axisConfig = m_Axis->getConfig(); // Get the axis configuration instance
    axisConfig->inversion = m_requestedValue;     // Set the inversion value to the requested value
    axisConfig->persist(m_Axis->getAxisType());   // Store the value in the EEPROM                          // REVIEW - Config should have context to the axis so the parameter is not needed
}