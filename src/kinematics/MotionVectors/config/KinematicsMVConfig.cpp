
#include "KinematicsMVConfig.hpp"

#include "eeprom/eepromstore.h"              // To load and save the configuration to EEPROM
#include "DefaultKinematicsMVConfig.hpp"     // To get the default configuration if the EEPROM is empty or the version is changed
#include <printervisitors/IPrinterVisitor.h> // For the visitor pattern

constexpr uint8_t c_EEPROM_KIN_MVCONFIG_VERSION = 1; // Define the version number for the KinematicsMVConfig in EEPROM.     // TODO: Add versioning

/**
 * @brief Constructor with no arguments - used when called with a non-existant axistype
 */
KinematicsMVConfig::KinematicsMVConfig()
    : inversion(false) {}

/**
 * @brief Constructor for KinematicsMVConfig class with axis vectorType.
 * @details This constructor initializes the KinematicsMVConfig object with the given MotionVectorType.
 *          It retrieves the configuration from EEPROM using the EEPROMStore class. If loading fails, it sets the configuration to default values.
 * @see config.h for overriding the default values.
 * @param vectorType The vectorType of the axis being configured.
 */
KinematicsMVConfig::KinematicsMVConfig(const MotionVector_t motionVectorType) : inversion(false) {
    if (!retrieve(motionVectorType)) {
        DefaultKinematicsMVConfig defaultConfig;
        *this = defaultConfig.create(motionVectorType);
    }
}

/**
 * @brief Constructor for KinematicsMVConfig class with parameterized settings.
 * @details This constructor initializes the KinematicsMVConfig object with the given parameters for sensitivity, gate, and function types.
 *          This constructor is used when called from the DefaultKinematicsMVConfig class.
 * @param psens Sensitivity for the positive direction.
 * @param nsens Sensitivity for the negative direction.
 * @param pgate Gate for the positive direction.
 * @param ngate Gate for the negative direction.
 * @param pmf Modifier function vectorType for the positive direction.
 * @param nmf Modifier function vectorType for the negative direction.
 * @param invert Inversion flag for the axis.
 * @note The constructor initializes the posConfig and negConfig members with the given parameters.
 */
KinematicsMVConfig::KinematicsMVConfig(const bool invert)
    : inversion(invert) {}

/**
 * @brief  Persist the KnobVectorConfig to EEPROM.
 * @details This function saves the KnobVectorConfig object to EEPROM using the EEPROMStore class.
 * @param vectorType The type of the MotionVector being persisted, used to identify the correct location in EEPROM.
 */
void KinematicsMVConfig::persist(const MotionVector_t vectorType) const {
    // Calculate the EEPROM tableId for the KinematicsMVConfig in EEPROM (@see eeprom/eepromstore.h for the ID layout)
    const int tableId = (static_cast<int>(vectorType) * EEPROM_KIN_MOTIONVECTOR_ID_RESERVATIONS) + EEPROM_KIN_MOTIONVECTOR_ID_BASE; // Calculated Id for the MotionVectorConfiguration in EEPROM

    // Persist the data stored in this class
    EEPROMStore::save(tableId, &inversion, sizeof(inversion)); // Store the inversion flag in the EEPROM
}

/**
 * @brief Loads the sensor configuration from EEPROM.
 * @param vectorType The type of the MotionVector being loaded, used to calculate the correct location in EEPROM.
 * @return The result of the load operation.
 * @retval True if the configuration was successfully loaded.
 * @retval False if the configuration could not be loaded.
 */
bool KinematicsMVConfig::retrieve(const MotionVector_t vectorType) {
    // Calculate the EEPROM tableId for the KinematicsMVConfig in EEPROM (@see eeprom/eepromstore.h for the ID layout)
    const int tableId = (static_cast<int>(vectorType) * EEPROM_KIN_MOTIONVECTOR_ID_RESERVATIONS) + EEPROM_KIN_MOTIONVECTOR_ID_BASE; // Calculated Id for the AxisConfig in EEPROM

    // Retrieve the data stored in the EEPROM
    return (EEPROMStore::load(tableId, &inversion, sizeof(inversion) == ERR_EEPROMSTORE_SUCCESS));
}

/**
 * @brief Accept a visitor for the visitor pattern.
 * @details This function accepts a visitor that will process this KnobVectorConfig object. Mostly used for serial output.
 * @param visitor The visitor that will process this KnobVectorConfig object.
 */
void KinematicsMVConfig::accept(IPrinterVisitor &visitor) {
    visitor.visit(*this);
}