#include "IAxisConfigCommand.h"

#include <commandhandler/CollectionCarrier/CollectionCarrier.hpp>
#include <axis/AxisCollection.hpp>
#include "axis/config/AxisConfig.hpp"
#include "visitors/AxisConfigPrinter.h"

#include <common/esp_print.h> // For ESP_PRINT

/**
 * @brief Executes the minmax command based on the provided parameters.
 * @param param1 First parameter
 * @param param2 Second parameter
 * @param paramCount Number of parameters provided.
 */
void IAxisConfigCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {
    if (paramCount == 0) {
        // No params provided, show current configuration values of the axes.
        AxisConfigPrinter printer;
        m_CollectionCarrier->getAxisCollection()->acceptAxesVisitor(printer);
    }

    if (paramCount == 1) {
        // No functionality for AxisConfigCommands with only one parameter
        ESP_INFO("Incomplete command");
    }

    if (paramCount == 2) {
        // F.e. command received: SENS <+|-><axisname> <value>

        // Get the value that has to be set
        if (!convertWordFloat(param2, &m_requestedValue)) {
            ESP_WARN("Param not float");
            return;
        }

        // Get the first character of the first parameter for direction (+ is maximum, - is minimum)
        char direction = param1[0];

        // Get the axis from the axis name
        char *reqAxisName = (char *)param1 + 1; // Get the axis name (skip the first character)

        m_Axis = m_CollectionCarrier->getAxisCollection()->getAxis(reqAxisName); // Get the axis by its name
        if (m_Axis == nullptr) {
            ESP_WARN("Unknown axis");
            return;
        }

        if (direction == '+') {
            // Set the maximum value for the sensor
            m_AxisDirectionConfig = &m_Axis->getConfig()->posConfig;

        } else if (direction == '-') {
            // Set the minimum value for the sensor
            m_AxisDirectionConfig = &m_Axis->getConfig()->negConfig;

        } else {
            ESP_WARN("Unknown command");
            return;
        }
    }
}