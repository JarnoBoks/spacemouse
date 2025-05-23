
#include "kinematicsconfig.hpp"

#include "eeprom/eepromstore.h"        // To load and save the axis configuration to EEPROM
#include "defaultkinematicsconfig.hpp" // To get the default axis configuration if the EEPROM is empty or the version is changed

#if defined(ARDUINO_ARCH_AVR)
#include "eeprom/eepromstore.h"                  // To load and save the sensor configuration to EEPROM
constexpr uint8_t EEPROM_KINEMATICS_VERSION = 1; // Define the version number for the KinematicsConfig in EEPROM.     // TODO: Add versioning
#endif
#if defined(ARDUINO_ARCH_ESP32)
#include "eeprom/preferencesstore.h" // To load and save the sensor configuration to Preferences
#endif

/** Constructor with no arguments - default */
KinematicsConfig::KinematicsConfig() {
    if (!retrieve()) {
        DefaultKinematicsConfig defaultConfig;
        *this = defaultConfig.create();
    }
};

/**
 * @brief Constructor with parameters - used when called from default config
 * @param exclusiveMode  Exclusive mode flag
 * @param switchYZ Switch YZ flag
 */
KinematicsConfig::KinematicsConfig(const bool exclusiveMode,
                                   const bool switchYZ) {
    m_data.exclusiveMode = exclusiveMode;
    m_data.switchYZ = switchYZ;
};

#if defined(ARDUINO_ARCH_ESP32)
#define KEY_PREF_KINCFGS "kin" // Key prefix for kinematics configuration in Preferences
#define BUF_KINEMATICS_LEN 4   // 3 characters for the key + 1 for null terminator
/**
 * @brief Saves the Kinematics configuration to EEPROM.
 * @note This function uses the EEPROMStore class to save the configuration.
 */
void KinematicsConfig::persist() const {
    const char *buffer = KEY_PREF_KINCFGS;                   // Key prefix for kinematics configuration in Preferences
    PreferencesStore::save(buffer, &m_data, sizeof(m_data)); // Store the data structure in the Preferences
}

/**
 * @brief Loads the Kinematics configuration from EEPROM.
 * @return The resultstatus of the load operation.
 * @retval True if the configuration was successfully loaded.
 * @retval False if the configuration could not be loaded.
 */
bool KinematicsConfig::retrieve() {
    const char *buffer = KEY_PREF_KINCFGS; // Key prefix for kinematics configuration in Preferences
    return (PreferencesStore::load(buffer, &m_data, sizeof(m_data)) == ERR_PREFSTORE_SUCCESS);
}
#endif

#if defined(ARDUINO_ARCH_AVR)
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
 * @return The resultstatus of the load operation.
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
#endif
