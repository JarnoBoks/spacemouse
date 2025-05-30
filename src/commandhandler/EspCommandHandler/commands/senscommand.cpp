#include "senscommand.h"
#include <knob/axis/KnobAxis.hpp>
#include <knob/axis/config/KnobAxisConfig.hpp>

#include <common/esp_print.h> // For ESP_PRINT

/**
 * @brief Executes the sens command based on the provided input parameters.
 * @note This function extends the functionality of the base class IAxisConfigCommand::execute(...).
 * @param param1 First parameter
 * @param param2 Second parameter
 * @param paramCount Number of parameters provided.
 */
void SensCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {

    // Call the base class execute function to parse the parameters.
    IAxisConfigCommand::execute(param1, param2, paramCount);

    // No update of the configuration parameters possible or needed if the requested value is less than 0
    if (m_requestedValue < 0) {
        return;
    }

    for (uint8_t i = 0; i < NUM_AX_DIRCFG; i++) {
        if (knobAxisDirectionConfigs[i]) {
            knobAxisDirectionConfigs[i]->setSensitivity(m_requestedValue);
        }
    }

    m_knobAxis->getConfig()->persist(m_knobAxis->getType()); // Store the value in the EEPROM
}