
#include <Arduino.h>
#include <EEPROM.h>
#include "eepromStorage.h"
#include "config.h"

// Initialize the static variables
bool EEPROMStorage::_firstrun = false; // Initialize the first run flag
bool EEPROMStorage::_setupdone = false;

/**
 * @brief Starts EEPROM functionality and checks if the EEPROM version is the same as the version stored in the EEPROM.
 * @details If the version number has changed, the member _firstrun will be set to true. Other objects will use this member to determine if they can load the settings
 * from the EEPROM. The flag _setupdone is used to determine if this function ran before.
 */
void EEPROMStorage::setupEEPROM() {
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
bool EEPROMStorage::isFirstRun() {
    setupEEPROM();
    return _firstrun; // Return the state of the EEPROM
}

/**
 * @brief Reports the version number and expected version number of the EEPROM storage to the Serial monitor.
 */
void EEPROMStorage::printVersion() { // Output the version number of the EEPROM storage
    uint8_t version = 0;
    EEPROM.get(EEPROM_ADDRESS_VERSION, version);

    Serial.print("EEPROM Version: ");
    Serial.print(version);
    Serial.print(" (expected: ");
    Serial.print(EEPROM_VERSION);
    Serial.println(")");
}
