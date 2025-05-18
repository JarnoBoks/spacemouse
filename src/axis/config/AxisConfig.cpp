
#include "AxisConfig.hpp"

#include "eeprom/eepromstore.h"       // To load and save the axis configuration to EEPROM
#include "DefaultAxisConfig.hpp"      // To get the default axis configuration if the EEPROM is empty or the version is changed
#include "visitors/IPrinterVisitor.h" // For the visitor pattern

constexpr uint8_t EEPROM_AXISCONFIG_VERSION = 1;     // Define the version number for the AxisConfig in EEPROM.     // TODO: Add versioning
constexpr uint8_t EEPROM_ID_OFFSET_AXCFG_POSCFG = 1; // Offset for the positive direction configuration ID
constexpr uint8_t EEPROM_ID_OFFSET_AXCFG_NEGCFG = 2; // Offset for the negative direction configuration ID

/**
 * @brief Constructor with no arguments - used when called with a non-existant axistype
 */
AxisConfig::AxisConfig() : posConfig(AxisDirectionConfig()), negConfig(AxisDirectionConfig()), inversion(false) {}

/**
 * @brief Constructor for AxisConfig class with axis type.
 * @details This constructor initializes the AxisConfig object with the given axis type.
 *          It retrieves the configuration from EEPROM using the EEPROMStore class. If loading fails, it sets the configuration to default values.
 * @see config.h for overriding the default values.
 * @see defaults_hall.h for the default values for the HALL Effect hardware.
 * @see defaults_joystick.h for the default values for the JOYSTICK hardware.
 * @param axisType The type of the axis being configured.
 */
AxisConfig::AxisConfig(const AxisType_t axisType) : inversion(false) {
    if (!retrieve(axisType)) {
        *this = DefaultAxisConfig::getInstance().getDefaultConfig(axisType);
    }
}

/**
 * @brief Constructor for AxisConfig class with parameterized settings.
 * @details This constructor initializes the AxisConfig object with the given parameters for sensitivity, gate, and function types.
 *          This constructor is used when called from the DefaultAxisConfig class.
 * @param psens Sensitivity for the positive direction.
 * @param nsens Sensitivity for the negative direction.
 * @param pgate Gate for the positive direction.
 * @param ngate Gate for the negative direction.
 * @param pmf Modifier function type for the positive direction.
 * @param nmf Modifier function type for the negative direction.
 * @param invert Inversion flag for the axis.
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

void AxisConfig::persist(const AxisType_t axisType) const {

    // Calculate the EEPROM tableId for the AxisConfig in EEPROM (@see eeprom/eepromstore.h for the ID layout)
    const int tableId = (static_cast<int>(axisType) * EEPROM_AXIS_ID_RESERVATIONS) + EEPROM_AXIS_ID_BASE; // Calculated Id for the AxisConfig in EEPROM

    // Persist the data stored in this class
    EEPROMStore::save(tableId, &inversion, sizeof(inversion)); // Store the inversion flag in the EEPROM

    // Persist the AxisDirectionConfig objects
    posConfig.persist(tableId + EEPROM_ID_OFFSET_AXCFG_POSCFG);
    negConfig.persist(tableId + EEPROM_ID_OFFSET_AXCFG_NEGCFG);
}

/**
 * @brief Loads the sensor configuration from EEPROM.
 * @param axisType The type of the axis being loaded, used to calculate the correct location in EEPROM.
 * @return The result of the load operation.
 * @retval True if the configuration was successfully loaded.
 * @retval False if the configuration could not be loaded.
 */
bool AxisConfig::retrieve(const AxisType_t axisType) {

    // Calculate the EEPROM tableId for the AxisConfig in EEPROM (@see eeprom/eepromstore.h for the ID layout)
    const int tableId = (static_cast<int>(axisType) * EEPROM_AXIS_ID_RESERVATIONS) + EEPROM_AXIS_ID_BASE; // Calculated Id for the AxisConfig in EEPROM

    // Retrieve the data stored in the EEPROM
    if (EEPROMStore::load(tableId, &inversion, sizeof(inversion)) != ERR_EEPROMSTORE_SUCCESS) {
        return false;
    }

    // Retrieve the AxisDirectionConfig objects
    if (posConfig.retrieve(tableId + EEPROM_ID_OFFSET_AXCFG_POSCFG) != ERR_EEPROMSTORE_SUCCESS) {
        return false;
    }
    if (negConfig.retrieve(tableId + EEPROM_ID_OFFSET_AXCFG_NEGCFG) != ERR_EEPROMSTORE_SUCCESS) {
        return false;
    }

    return true;
}

/**
 * @brief Accept a visitor for the visitor pattern.
 * @details This function accepts a visitor that will process this AxisConfig object. Mostly used for serial output.
 * @param visitor The visitor that will process this AxisConfig object.
 */
void AxisConfig::accept(IPrinterVisitor &visitor) {
    visitor.visit(*this);
}