#include "gatecommand.h"
#include <knob/axis/KnobAxis.hpp>
#include <knob/axis/config/KnobAxisConfig.hpp>

#include <common/esp_print.h> // For ESP_PRINT

/**
 * @brief Executes the gate command based on the provided parameters.
 * @note This function extends the functionality of the base class IAxisConfigCommand::execute(...).
 * @param param1 First parameter
 * @param param2 Second parameter
 * @param paramCount Number of parameters provided.
 */
void GateCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {
    // Call the base class execute function to parse the parameters.
    IAxisConfigCommand::execute(param1, param2, paramCount);

    if (m_requestedValue < 0) {
        // No update of the configuration parameters possible or needed.
        return;
    }

    for (uint8_t i = 0; i < NUM_AX_DIRCFG; i++) {
        if (m_knobVectorDirectionConfig[i]) {
            m_knobVectorDirectionConfig[i]->setGate(m_requestedValue);
        }
    }

    m_knobVector->getConfig()->persist(m_knobVector->getType()); // Store the value in the EEPROM
}
