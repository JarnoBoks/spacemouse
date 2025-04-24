#ifndef EEPROMSTORE_H
#define EEPROMSTORE_H

#include "axisconfig.h"
#include "sensorconfig.h"
#include "config.h" // Include the config.h file for EEPROM_VERSION
#include <Arduino.h>

class EEPROMStore {
private:
    static bool _firstrun;  // Indicates if this is the first boot of the space mouse (or space mouse version)
    static bool _setupdone; // Indicates if the EEPROM setup is complete and the first run flag has been checked

public:
    static bool isFirstRun(); // Check if the EEPROM is initialized`
    static void setupEEPROM();

    static bool loadConfig(AxisConfig &config, int axisnumber);
    static bool loadConfig(SensorConfig &config, int sensornumber);

    static void saveConfig(AxisConfig &config, int axisnumber);
    static void saveConfig(SensorConfig &config, int sensornumber);

    bool isInitialized();
    SensorConfig loadSensorConfig(int id);
};

// EEPROM Storage structures for complex classes

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

// ------------------------- EEPROM ADDRESS TABLE -------------------------

/**
 * @brief If the version number defined in the EEPROM is not equal to the version number defined in this file, the EEPROM will be erased and initialized with the default values.
 */
/// The version number of the EEPROM datastorage. Can be used to force loading the configuration values set in config.h.
/// @warning Changing the version number will reset all stored calibration parameters in the EEPROM.
constexpr uint8_t EEPROM_VERSION = SM_VERSION;

/**
 * The Arduino Micro has an EEPROM storage that can store 1024bytes (1KB)
 * This configuration file maintains the address table of the stored data. *
 */
constexpr int EEPROM_ADDRESS_VERSION = 1;                                            // Start EEPROM address for the Datastorage version
constexpr int EEPROM_ADDRESS_VERSION_END = EEPROM_ADDRESS_VERSION + sizeof(uint8_t); // End EEPROM address for the Datastorageversion

constexpr int EEPROM_ADDRESS_CFG_AXES_BASE = 16;                                                          // Base address for the axis configuration
constexpr int EEPROM_ADDRESS_CFG_AXES_BASE_END = EEPROM_ADDRESS_CFG_AXES_BASE + 6 * sizeof(EEAxisConfig); // We store 6 axis configurations in the EEPROM

constexpr int EEPROM_ADDRESS_CFG_SENSORS_BASE = EEPROM_ADDRESS_CFG_AXES_BASE_END; // Base address for the sensor configuration
constexpr int EEPROM_ADDRESS_CFG_SENSORS_BASE_END = EEPROM_ADDRESS_CFG_SENSORS_BASE + 8 * sizeof(EESensorConfig);

#endif // EEPROMSTORE_H