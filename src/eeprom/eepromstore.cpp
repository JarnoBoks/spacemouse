#include "eepromStore.h"
#include <EEPROM.h> // Include the EEPROM library for EEPROM operations

// Initialize the static variables
bool EEPROMStore::_firstrun = false; // Initialize the first run flag
bool EEPROMStore::_setupdone = false;

/**
 * @brief Starts EEPROM functionality and checks if the EEPROM version is the same as the version stored in the EEPROM.
 * @details If the version number has changed, the member _firstrun will be set to true. Other objects will use this member to determine if they can load the settings
 * from the EEPROM. The flag _setupdone is used to determine if this function ran before.
 */
void EEPROMStore::setupEEPROM() {
    if (_setupdone) { // Ensure this function can only be run once
        return;
    } else {
        _setupdone = true; // Reset the first run flag
    }

    uint8_t version = 0;
    EEPROM.get(EEPROM_ADDRESS_VERSION, version);

    if (version != EEPROM_VERSION) {
        _firstrun = true;                                   // EEPROM is not initialized
        EEPROM.put(EEPROM_ADDRESS_VERSION, EEPROM_VERSION); // Store the version in the EEPROM
    }
}

/**
 * @brief isFirstRun() checks if the EEPROM contains the correct defaults/configuration parameters.
 * @details If the version number defined in the EEPROM is not equal to the version number defined in this file, the EEPROM will be erased and initialized with the default values.
 * @return The state of the EEPROM.
 * @retval true indicates it is a firstrun and the EEPROM is not initialized.
 * @retval false indicates the EEPROM is initialized.
 */
bool EEPROMStore::isFirstRun() {
    setupEEPROM();
    return _firstrun; // Return the state of the EEPROM
}

/**
 * @brief Loads the axis configuration from the EEPROM.
 * @param config The axis configuration to load.
 * @param address The EEPROM address to load the configuration from.
 * @return true if the configuration was loaded successfully, false otherwise.
 */
bool EEPROMStore::loadConfig(AxisConfig &config, const int axisnumber) {
    if (isFirstRun()) {
        return false; // EEPROM is not initialized, return false
    }

    EEAxisConfig eeConfig;
    EEPROM.get(EEPROM_ADDRESS_CFG_AXES_BASE + axisnumber * sizeof(EEAxisConfig), eeConfig); // Load the configuration from the EEPROM

    config.posConfig.sensitivity = eeConfig.pSens;
    config.negConfig.sensitivity = eeConfig.nSens;
    config.posConfig.gate = eeConfig.pGate;
    config.negConfig.gate = eeConfig.nGate;
    config.posConfig.modFuncType = eeConfig.pModFunc;
    config.negConfig.modFuncType = eeConfig.nModFunc;
    config.inversion = eeConfig.inversion;

    // FIXME return true; // Return true while the configuration was loaded successfully
    return false;
}

/**
 * @brief Stores the axis configuration in the EEPROM.
 * @param config The axis configuration to store.
 * @param address The EEPROM address to store the configuration.
 */
void EEPROMStore::saveConfig(AxisConfig &config, const int axisnumber) {
    EEAxisConfig eeConfig;
    eeConfig.pSens = config.posConfig.sensitivity;
    eeConfig.nSens = config.negConfig.sensitivity;
    eeConfig.pGate = config.posConfig.gate;
    eeConfig.nGate = config.negConfig.gate;
    eeConfig.pModFunc = config.posConfig.modFuncType;
    eeConfig.nModFunc = config.negConfig.modFuncType;
    eeConfig.inversion = config.inversion;

    EEPROM.put(EEPROM_ADDRESS_CFG_AXES_BASE + axisnumber * sizeof(EEAxisConfig), eeConfig); // Store the configuration in the EEPROM
}

/**
 * @brief Loads the sensor configuration from the EEPROM.
 * @param config The sensor configuration to load.
 * @param address The EEPROM address to load the configuration from.
 * @return true if the configuration was loaded successfully, false otherwise.
 */
bool EEPROMStore::loadConfig(SensorConfig &config, const int sensornumber) {
    if (isFirstRun()) {
        return false; // EEPROM is not initialized, return false
    }

    // Setup the EEPROM address and transfer object for the sensor configuration
    EESensorConfig eeConfig;
    EEPROM.get(EEPROM_ADDRESS_CFG_SENSORS_BASE + sensornumber * sizeof(EESensorConfig), eeConfig); // Load the configuration from the EEPROM

    config.setMin(eeConfig.minv);
    config.setMax(eeConfig.maxv);
    config.setInverted(eeConfig.inversion);
    config.setDeadzone(eeConfig.deadzone);

    // FIXME return true; // Return true while the configuration was loaded successfully
    return false;
}

void EEPROMStore::saveConfig(SensorConfig &config, const int sensornumber) {
    EESensorConfig eeConfig;
    eeConfig.minv = config.getMin();
    eeConfig.maxv = config.getMax();
    eeConfig.inversion = config.isInverted();
    eeConfig.deadzone = config.getDeadzone();

    EEPROM.put(EEPROM_ADDRESS_CFG_SENSORS_BASE + sensornumber * sizeof(EESensorConfig), eeConfig); // Store the configuration in the EEPROM
}