
#include "AxisConfig.hpp"

#include "eeprom/eepromstore.h"       // To load and save the axis configuration to EEPROM
#include "DefaultAxisConfig.hpp"      // To get the default axis configuration if the EEPROM is empty or the version is changed
#include "visitors/IPrinterVisitor.h" // For the visitor pattern

/** Constructor with no arguments - used when called with a non-existant axis (ie axistype = -1) */
AxisConfig::AxisConfig() : posConfig(AxisDirectionConfig()), negConfig(AxisDirectionConfig()), inversion(false) {}

/**
 * @brief Constructor for AxisConfig class with axis type.
 * @details This constructor initializes the AxisConfig object with the given axis type.
 *          This constructor is used when called from an axis.
 *          It loads the configuration from EEPROM using the EEPROMStore class. If loading fails, it sets the configuration to default values.
 * @param axisType The type of the axis being configured.
 */
AxisConfig::AxisConfig(AxisType_t axisType) : inversion(false) {
    if (!EEPROMStore::loadConfig(*this, static_cast<const int>(axisType))) {
        // If loading from EEPROM fails, setup the configuration with default defined values, using the default axis configuration class.
        *this = DefaultAxisConfig::getInstance().getDefaultConfig(axisType);
    }
}

/**
 * @brief Constructor for AxisConfig class with parameterized settings.
 * @details This constructor initializes the AxisConfig object with the given parameters for sensitivity, gate, and function types.
 *          This constructor is used when called from the DefaultAxisConfig class.
 * @param[in] psens|float Sensitivity for the positive direction.
 * @param[in] nsens Sensitivity for the negative direction.
 * @param[in] pgate Gate for the positive direction.
 * @param[in] ngate Gate for the negative direction.
 * @param[in] pmf Modifier function type for the positive direction.
 * @param[in] nmf Modifier function type for the negative direction.
 * @param[in] invert Inversion flag for the axis.
 * @note The constructor initializes the posConfig and negConfig members with the given parameters.
 */
AxisConfig::AxisConfig(const float psens,
                       const float nsens,
                       const uint8_t pgate,
                       const uint8_t ngate,
                       const ModFunc_t pmf,
                       const ModFunc_t nmf,
                       const bool invert)
    : posConfig(AxisDirectionConfig(psens, pgate, pmf)), negConfig(AxisDirectionConfig(nsens, ngate, nmf)), inversion(invert) {}

/**
 * @brief  Persist the AxisConfig to EEPROM.
 * @details This function saves the AxisConfig object to EEPROM using the EEPROMStore class.
 * @param axisType The type of the axis being persisted, used to identify the correct location in EEPROM.
 */
void AxisConfig::persist(AxisType_t axisType) {
    EEPROMStore::saveConfig(*this, static_cast<const int>(axisType)); // Store the configuration in the EEPROM
}

/**
 * @brief Accept a visitor for the visitor pattern.
 * @details This function accepts a visitor that will process this AxisConfig object. Mostly used for serial output.
 * @param visitor The visitor that will process this AxisConfig object.
 */
void AxisConfig::accept(IPrinterVisitor &visitor) {
    visitor.visit(*this);
}