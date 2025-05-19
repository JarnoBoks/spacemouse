#include "modfunccommand.h"
#include <Knob/MotionVector/KnobMotionVector.hpp>
#include "..\..\..\Knob\config\AxisConfig.hpp"

#include <common/esp_print.h> // For ESP_PRINT

/**
 * @brief Executes the modfunct command based on the provided parameters.
 * @note This function extends the functionality of the base class IAxisConfigCommand::execute(...).
 * @param param1 First parameter
 * @param param2 Second parameter
 * @param paramCount Number of parameters provided.
 */
void ModFuncCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {
    // Call the base class execute function to parse the parameters.
    IAxisConfigCommand::execute(param1, param2, paramCount);

    // No update of the configuration parameters possible or needed if the requested value is less than 0
    if (m_requestedValue < 0) {
        return;
    }

    for (uint8_t i = 0; i < NUM_AX_DIRCFG; i++) {
        if (m_AxisDirectionConfig[i]) {
            m_AxisDirectionConfig[i]->setModFuncType(static_cast<ModFunc_t>(m_requestedValue));
        }
    }

    m_Axis->getConfig()->persist(m_Axis->getAxisType()); // Store the value in the EEPROM
}
