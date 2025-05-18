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

    // Call the base class execute function to parse the parameters.
    IAxisConfigCommand::execute(param1, param2, paramCount); // Call the base class execute function

    // No update of the configuration parameters possible or needed if the requested value is less than 0
    if (m_requestedValue < 0) {
        return;
    }

    AxisConfig *axisConfig = m_Axis->getConfig(); // Get the axis configuration instance
    axisConfig->inversion = m_requestedValue;     // Set the inversion value to the requested value
    axisConfig->persist(m_Axis->getAxisType());   // Store the value in the EEPROM                          // REVIEW - Config should have context to the axis so the parameter is not needed
}