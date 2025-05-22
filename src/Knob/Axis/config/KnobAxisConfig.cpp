
#include "KnobAxisConfig.hpp"

#include "eeprom/eepromstore.h"              // To load and save the axis configuration to EEPROM
#include "DefaultKnobAxisConfig.hpp"         // To get the default axis configuration if the EEPROM is empty or the version is changed
#include <visitors/printers/IPrinterVisitor.h> // For the visitor pattern

constexpr uint8_t EEPROM_AXISCONFIG_VERSION = 1;     // Define the version number for the AxisConfig in EEPROM.     // TODO: Add versioning
constexpr uint8_t EEPROM_ID_OFFSET_AXCFG_POSCFG = 1; // Offset for the positive direction configuration ID
constexpr uint8_t EEPROM_ID_OFFSET_AXCFG_NEGCFG = 2; // Offset for the negative direction configuration ID

/**
 * @brief Constructor with no arguments - used when called with a non-existant axistype
 */
KnobAxisConfig::KnobAxisConfig()
    : posConfig(KnobAxisDirectionConfig()),
      negConfig(KnobAxisDirectionConfig()), inversion(false) {}

/**
 * @brief Constructor for KnobAxisConfig class with axis vectorType.
 * @details This constructor initializes the KnobAxisConfig object with the given axis vectorType.
 *          It retrieves the configuration from EEPROM using the EEPROMStore class. If loading fails, it sets the configuration to default values.
 * @see config.h for overriding the default values.
 * @see defaults_hall.h for the default values for the HALL Effect hardware.
 * @see defaults_joystick.h for the default values for the JOYSTICK hardware.
 * @param vectorType The vectorType of the axis being configured.
 */
KnobAxisConfig::KnobAxisConfig(const MotionVector_t motionVectorType) : inversion(false) {
    if (!retrieve(motionVectorType)) {
        DefaultKnobAxisConfig defaultConfig;
        *this = defaultConfig.create(motionVectorType);
    }
}

/**
 * @brief Constructor for KnobAxisConfig class with parameterized settings.
 * @details This constructor initializes the KnobAxisConfig object with the given parameters for sensitivity, gate, and function types.
 *          This constructor is used when called from the DefaultKnobAxisConfig class.
 * @param psens Sensitivity for the positive direction.
 * @param nsens Sensitivity for the negative direction.
 * @param pgate Gate for the positive direction.
 * @param ngate Gate for the negative direction.
 * @param pmf Modifier function vectorType for the positive direction.
 * @param nmf Modifier function vectorType for the negative direction.
 * @param invert Inversion flag for the axis.
 * @note The constructor initializes the posConfig and negConfig members with the given parameters.
 */
KnobAxisConfig::KnobAxisConfig(const float psens,
                               const float nsens,
                               const uint8_t pgate,
                               const uint8_t ngate,
                               const ModFunc_t pmf,
                               const ModFunc_t nmf,
                               const bool invert)
    : posConfig(KnobAxisDirectionConfig(psens, pgate, pmf)),
      negConfig(KnobAxisDirectionConfig(nsens, ngate, nmf)),
      inversion(invert) {}

/**
 * @brief  Persist the KnobAxisConfig to EEPROM.
 * @details This function saves the KnobAxisConfig object to EEPROM using the EEPROMStore class.
 * @param vectorType The type of the MotionVector being persisted, used to identify the correct location in EEPROM.
 */
void KnobAxisConfig::persist(const MotionVector_t vectorType) const {

    // Calculate the EEPROM tableId for the KnobAxisConfig in EEPROM (@see eeprom/eepromstore.h for the ID layout)
    const int tableId = (static_cast<int>(vectorType) * EEPROM_KNOB_AXIS_ID_RESERVATIONS) + EEPROM_KNOB_AXIS_ID_BASE; // Calculated Id for the AxisConfiguration in EEPROM

    // Persist the data stored in this class
    EEPROMStore::save(tableId, &inversion, sizeof(inversion)); // Store the inversion flag in the EEPROM

    // Persist the AxisDirectionConfig objects
    posConfig.persist(tableId + EEPROM_ID_OFFSET_AXCFG_POSCFG);
    negConfig.persist(tableId + EEPROM_ID_OFFSET_AXCFG_NEGCFG);
}

/**
 * @brief Loads the sensor configuration from EEPROM.
 * @param vectorType The type of the KnobAxis being loaded, used to calculate the correct location in EEPROM.
 * @return The result of the load operation.
 * @retval True if the configuration was successfully loaded.
 * @retval False if the configuration could not be loaded.
 */
bool KnobAxisConfig::retrieve(const MotionVector_t vectorType) {

    // Calculate the EEPROM tableId for the KnobAxisConfig in EEPROM (@see eeprom/eepromstore.h for the ID layout)
    const int tableId = (static_cast<int>(vectorType) * EEPROM_KNOB_AXIS_ID_RESERVATIONS) + EEPROM_KNOB_AXIS_ID_BASE; // Calculated Id for the knob AxisConfiguration in EEPROM

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
 * @details This function accepts a visitor that will process this KnobAxisConfig object. Mostly used for serial output.
 * @param visitor The visitor that will process this KnobAxisConfig object.
 */
void KnobAxisConfig::accept(IPrinterVisitor &visitor) {
    visitor.visit(*this);
}