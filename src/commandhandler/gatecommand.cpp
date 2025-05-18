#include "gatecommand.h"
#include "axis/axes/Axis.hpp" // For Axis class
#include "axis/config/AxisConfig.hpp"

#include <common/esp_print.h> // For ESP_PRINT

#if 0
/**
 * @brief Executes the gate command based on the provided parameters.
 * @note This function extends the functionality of the base class IAxisConfigCommand::execute(...).
 * @param param1 First parameter
 * @param param2 Second parameter
 * @param paramCount Number of parameters provided.
 */
void GateCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {
    // Call the base class execute function to handle common functionality
    IAxisConfigCommand::execute(param1, param2, paramCount);

    if (m_requestedValue < 0) {
        // No update of the configuration parameters possible or needed.
        return;
    }

    for (uint8_t i = 0; i < NUM_AX_DIRCFG; i++) {
        if (m_AxisDirectionConfig[i]) {
            m_AxisDirectionConfig[i]->setGate(m_requestedValue);
        }
    }

    m_Axis->getConfig()->persist(m_Axis->getAxisType()); // Store the value in the EEPROM
}
#endif

void GateCommand::dir_config_updater(AxisDirectionConfig *axisDirectionConfig) {
    if (axisDirectionConfig) {
        axisDirectionConfig->setGate(m_requestedValue);
    }
};