#include "IAxisConfigCommand.h"

#include <commandhandler/CollectionCarrier/CollectionCarrier.hpp>
#include <knob/KnobAxisCollection.hpp>
#include <knob/axis/config/KnobAxisConfig.hpp>
#include <visitors/printers/AxisConfigPrinter.hpp>

#include <common/esp_print.h> // For ESP_PRINT

/**
 * @brief Executes axis-related commands. Updates the m_knobAxisDirectionConfig array to point to the correct AxisDirectionConfig object(s) and sets m_requestedValue to -1 if no update is needed.
 * @details This function handles the execution of axis-related commands based on the provided parameters. It can be used to set the sensitivity, gate, or mod function type for a specific axis.
 *          If the command leads to an update of the configuration parameters, the m_requestedValue is set to the requested value.
 *          If the command does not lead to an update of the configuration parameters, the m_requestedValue is set to -1.
 * @param param1 The first parameter (axis name or [+|-]axisname ).
 * @param param2 The second parameter (value).
 * @param paramCount The number of parameters provided.
 */
void IAxisConfigCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {
    m_requestedValue = -1; // Default value for the requested value
    if (paramCount == 0) {
        // No params provided, show current configuration values of the axes.
        AxisConfigPrinter printer;
        m_CollectionCarrier->getKnobAxes()->acceptAxesPrinter(printer);
        return;
    }

    if (paramCount == 1) {
        // No functionality for AxisConfigCommands with only one parameter
        ESP_INFO("Incomplete command");
        return;
    }

    if (paramCount == 2) {
        // F.e. command received: SENS [+|-]<axisname> <value>

        // Erase the m_knobAxisDirectionConfig pointers
        for (uint8_t i = 0; i < NUM_AX_DIRCFG; i++) {
            m_knobVectorDirectionConfig[i] = nullptr;
        }

        if (!convertWordFloat(param2, &m_requestedValue)) {
            ESP_WARN("Param not float");
            m_requestedValue = -1; // Update the requested value to -1, indicating no update/storage needed
            return;
        }

        char directionChar = param1[0]; // The direction entered in the user command (first character of the first parameter, + or - or axisname's first character)
        if (directionChar == '+' || directionChar == '-') {
            // The first character is a direction
            // REVIEW - Can the cast (char *)param1 be removed?
            char *reqAxisName = (char *)param1 + 1; // Pointer to the axis name (skip the first character)
            m_knobVector = m_CollectionCarrier->getKnobAxes()->getAxis(reqAxisName);
            if (m_knobVector == nullptr) {
                ESP_INFO("KnobAxis not found");
                m_requestedValue = -1;
                return;
            }

            if (directionChar == '+') {
                // The positive direction config should be used
                m_knobVectorDirectionConfig[0] = &m_knobVector->getConfig()->posConfig;

            } else if (directionChar == '-') {
                // The negative direction config should be used
                m_knobVectorDirectionConfig[0] = &m_knobVector->getConfig()->negConfig;
            }

        } else {
            // The first character is not a direction, test if the KnobAxis name is specified.
            m_knobVector = m_CollectionCarrier->getKnobAxes()->getAxis(param1);
            if (m_knobVector == nullptr) {
                ESP_INFO("KnobAxis not found");
                m_requestedValue = -1; // Update the requested value to -1, indicating no update/storage needed
                return;
            }

            // There is an axis name, but no direction provided. Both directions have to be updated.
            m_knobVectorDirectionConfig[0] = &m_knobVector->getConfig()->posConfig;
            m_knobVectorDirectionConfig[1] = &m_knobVector->getConfig()->negConfig;
        }
    }
}