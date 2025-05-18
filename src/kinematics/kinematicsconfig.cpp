
#include "kinematicsconfig.h"

#include "eeprom/eepromstore.h"       // To load and save the axis configuration to EEPROM
#include "defaultkinematicsconfig.h"  // To get the default axis configuration if the EEPROM is empty or the version is changed
#include "visitors/IPrinterVisitor.h" // For the visitor pattern

constexpr uint8_t EEPROM_KINEMATICS_VERSION = 1; // Define the version number for the KinematicsConfig in EEPROM.     // TODO: Add versioning

/** Constructor with no arguments - default */
KinematicsConfig::KinematicsConfig() {
    if (!retrieve()) {
        *this = DefaultKinematicsConfig::getInstance().getDefaultConfig();
    }
};

/**
 * @brief   Constructor with parameters - used when called from default config
 * @param exclmd  Exclusive mode flag
 * @param switchyz Switch YZ flag
 */
KinematicsConfig::KinematicsConfig(const bool exclusiveMode,
                                   const bool switchYZ) {
    m_data.exclusiveMode = exclusiveMode;
    m_data.switchYZ = switchYZ;
};

/**
 * @brief Saves the Kinematics configuration to EEPROM.
 * @note This function uses the EEPROMStore class to save the configuration.
 */
void KinematicsConfig::persist() const {
    // Calculate the EEPROM tableId for the KinematicsConfig in EEPROM (@see eeprom/eepromstore.h for the ID layout)
    const int tableId = EEPROM_KINEMATICS_ID_BASE; // Calculated Id for the KinematicsConfig in EEPROM

    // Persist the data stored in this class
    EEPROMStore::save(tableId, &m_data, sizeof(m_data)); // Store the data structure in the EEPROM
}

/**
 * @brief Loads the Kinematics configuration from EEPROM.
 * @return The result of the load operation.
 * @retval True if the configuration was successfully loaded.
 * @retval False if the configuration could not be loaded.
 */
bool KinematicsConfig::retrieve() {
    // Calculate the EEPROM tableId for the KinematicsConfig in EEPROM (@see eeprom/eepromstore.h for the ID layout)
    const int tableId = EEPROM_KINEMATICS_ID_BASE; // Calculated Id for the KinematicsConfig in EEPROM

    // Retrieve the data stored in the EEPROM
    if (EEPROMStore::load(tableId, &m_data, sizeof(m_data)) != ERR_EEPROMSTORE_SUCCESS) {
        return false;
    }

    return true;
}

void KinematicsConfig::accept(IPrinterVisitor &visitor) {
    visitor.visit(*this);
}