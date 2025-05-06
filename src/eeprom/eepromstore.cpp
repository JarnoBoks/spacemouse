#include "eepromStore.h"
#include <EEPROM.h> // Include the EEPROM library for EEPROM operations

#include "config.h" // Include the config.h file for EEPROM_VERSION

// Header files for the objects that will be stored in the EEPROM)
#include "axis/axisconfig.h"
#include "sensor/sensorconfig.h"
#include "kinematics/kinematicsconfig.h"

/// @brief If the version number defined in the EEPROM is not equal to the version number defined in this file, the EEPROM will be erased and initialized with the default values.
/// @warning Changing the version number will reset all stored calibration parameters in the EEPROM.
constexpr uint8_t EEPROM_VERSION = SM_VERSION;

/// EEPROM Storage structures for complex classes
/// @warning Changing this without changing the version number will give undefined behavior!
struct EEAxisConfig {
    float pSens;
    float nSens;        // Sensitivity for the positive and negative direction
    uint8_t pGate;      // Gate for the positive direction
    uint8_t nGate;      // Gate for the negative direction
    ModFunc_t pModFunc; // Modifier function for the positive direction
    ModFunc_t nModFunc; // Modifier function for the negative direction
    bool inversion;     // Inversion flag for the axis
};

struct EESensorConfig {
    int minv = 0;
    int maxv = 0;
    bool inversion = false;
    uint8_t deadzone = 0;
};

// -------------------------- EEPROM ADDRESS TABLE END -------------------------

/**
 * The Arduino Micro has an EEPROM storage that can store 1024bytes (1KB)
 * This configuration file maintains the address table of the stored data. *
 */
constexpr int EEPROM_ADDRESS_VERSION = 1;                                            // Start EEPROM address for the Datastorage version
constexpr int EEPROM_ADDRESS_VERSION_END = EEPROM_ADDRESS_VERSION + sizeof(uint8_t); // End EEPROM address for the Datastorageversion

constexpr int EEPROM_ADDRESS_CFG_AXES_BASE = 16;                                                     // Base address for the axis configuration
constexpr int EEPROM_ADDRESS_CFG_AXES_END = EEPROM_ADDRESS_CFG_AXES_BASE + 6 * sizeof(EEAxisConfig); // We store 6 axis configurations in the EEPROM

constexpr int EEPROM_ADDRESS_CFG_SENSORS_BASE = EEPROM_ADDRESS_CFG_AXES_END; // Base address for the sensor configuration
// constexpr int EEPROM_ADDRESS_CFG_SENSORS_END = EEPROM_ADDRESS_CFG_SENSORS_BASE + 8 * sizeof(EESensorConfig); // FIXME We use the SensorConfig class to store the sensor configuration in the EEPROM
constexpr int EEPROM_ADDRESS_CFG_SENSORS_END = EEPROM_ADDRESS_CFG_SENSORS_BASE + 8 * sizeof(SensorConfig);

constexpr int EEPROM_ADDRESS_CFG_KINEMATICS_BASE = EEPROM_ADDRESS_CFG_SENSORS_END;        // Base address for the kinematics configuration
constexpr int EEPROM_ADDRESS_CFG_KINEMATICS_END = EEPROM_ADDRESS_CFG_KINEMATICS_BASE + 2; // End EEPROM address for kinematics configuration - Stores two booleans

constexpr int EEPROM_ADDRESS_CFG_AXIS_PCONFIG_BASE = EEPROM_ADDRESS_CFG_KINEMATICS_END;                                   // End EEPROM address for the configuration
constexpr int EEPROM_ADDRESS_CFG_AXIS_PCONFIG_END = EEPROM_ADDRESS_CFG_AXIS_PCONFIG_BASE + (6 * sizeof(DirectionConfig)); // End EEPROM address for the configuration

constexpr int EEPROM_ADDRESS_CFG_AXIS_NCONFIG_BASE = EEPROM_ADDRESS_CFG_AXIS_PCONFIG_END;                                 // End EEPROM address for the configuration
constexpr int EEPROM_ADDRESS_CFG_AXIS_NCONFIG_END = EEPROM_ADDRESS_CFG_AXIS_NCONFIG_BASE + (6 * sizeof(DirectionConfig)); // End EEPROM address for the configuration

constexpr int EEPROM_ADDRESS_CFG_AXIS_INV_BASE = EEPROM_ADDRESS_CFG_AXIS_NCONFIG_END;                                   // End EEPROM address for the configuration
constexpr int EEPROM_ADDRESS_CFG_AXIS_INV_END = EEPROM_ADDRESS_CFG_AXIS_INV_BASE + (6 * sizeof(AxisConfig::inversion)); // End EEPROM address for the configuration

// -------------------------- EEPROM ADDRESS TABLE END -------------------------

// Initialize the static class variables
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

#if 0
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
#else
bool EEPROMStore::loadConfig(AxisConfig &config, const int axisnumber) {
    if (isFirstRun()) {
        return false; // EEPROM is not initialized, return false
    }

    EEPROM.put(EEPROM_ADDRESS_CFG_AXIS_PCONFIG_BASE + axisnumber * sizeof(DirectionConfig), config.posConfig);   // Store the configuration in the EEPROM
    EEPROM.put(EEPROM_ADDRESS_CFG_AXIS_NCONFIG_BASE + axisnumber * sizeof(DirectionConfig), config.negConfig);   // Store the configuration in the EEPROM
    EEPROM.put(EEPROM_ADDRESS_CFG_AXIS_INV_BASE + axisnumber * sizeof(AxisConfig::inversion), config.inversion); // Store the configuration in the EEPROM

    // FIXME return true; // Return true while the configuration was loaded successfully
    return false;
}

/**
 * @brief Stores the axis configuration in the EEPROM.
 * @param config The axis configuration to store.
 * @param address The EEPROM address to store the configuration.
 */
void EEPROMStore::saveConfig(AxisConfig &config, const int axisnumber) {
    EEPROM.put(EEPROM_ADDRESS_CFG_AXIS_PCONFIG_BASE + axisnumber * sizeof(DirectionConfig), config.posConfig);   // Store the configuration in the EEPROM
    EEPROM.put(EEPROM_ADDRESS_CFG_AXIS_NCONFIG_BASE + axisnumber * sizeof(DirectionConfig), config.negConfig);   // Store the configuration in the EEPROM
    EEPROM.put(EEPROM_ADDRESS_CFG_AXIS_INV_BASE + axisnumber * sizeof(AxisConfig::inversion), config.inversion); // Store the configuration in the EEPROM
}
#endif

#if 0 // REVIEW - If this is not used, remove it. It is only used for the old EEPROM version.
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
#else

bool EEPROMStore::loadConfig(SensorConfig &config, const int sensornumber) {
    if (isFirstRun()) {
        return false; // EEPROM is not initialized, return false
    }

    // Setup the EEPROM address and transfer object for the sensor configuration
    EEPROM.get(EEPROM_ADDRESS_CFG_SENSORS_BASE + sensornumber * sizeof(SensorConfig), config); // Load the configuration from the EEPROM
    // FIXME return true; // Return true while the configuration was loaded successfully. Is necessary for simulation purposes (simavr has no EEPROM available)
    return false;
}

void EEPROMStore::saveConfig(SensorConfig &config, const int sensornumber) {
    EEPROM.put(EEPROM_ADDRESS_CFG_SENSORS_BASE + sensornumber * sizeof(SensorConfig), config); // Store the configuration in the EEPROM
}
#endif

bool EEPROMStore::loadConfig(KinematicsConfig &config) {
    if (isFirstRun()) {
        return false; // EEPROM is not initialized, return false
    }

    EEPROM.get(EEPROM_ADDRESS_CFG_AXES_BASE, config.exclusiveMode); // Load the configuration from the EEPROM
    EEPROM.get(EEPROM_ADDRESS_CFG_AXES_BASE + 1, config.switchYZ);  // Load the configuration from the EEPROM
    // FIXME return true; // Return true while the configuration was loaded successfully
    return false;
}

void EEPROMStore::saveConfig(KinematicsConfig &config) {
    EEPROM.put(EEPROM_ADDRESS_CFG_AXES_BASE, config.exclusiveMode);
    EEPROM.put(EEPROM_ADDRESS_CFG_AXES_BASE + 1, config.switchYZ);
}

#if 0
// REMOVE
bool EEPROMStore::loadConfig(KeyConfig &config, const int8_t buttonnumber) {
    if (isFirstRun()) {
        return false; // EEPROM is not initialized, return false
    }

    EEPROM.get(EEPROM_ADDRESS_CFG_AXIS_PCONFIG_BASE + buttonnumber * sizeof(KeyConfig), config); // Load the configuration from the EEPROM
    // FIXME return true; // Return true while the configuration was loaded successfully
    return false;
}

void EEPROMStore::saveConfig(KeyConfig &config, const int8_t buttonnumber) {
    EEPROM.put(EEPROM_ADDRESS_CFG_AXIS_PCONFIG_BASE + buttonnumber * sizeof(KeyConfig), config); // Store the configuration in the EEPROM
}
#endif