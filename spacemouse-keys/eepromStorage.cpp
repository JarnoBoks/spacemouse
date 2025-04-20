
#include <Arduino.h>
#include <EEPROM.h>
#include "eepromStorage.h"
#include "config.h"

// Initialize the static variables
bool EEPROMStorage::_firstrun = false; // EEPROM state
bool EEPROMStorage::_initialized = false;

void EEPROMStorage::setupEEPROM() {
    uint8_t version = 0;
    EEPROM.get(EEPROM_ADDRESS_VERSION, version);

    if (version != EEPROM_VERSION) {
        _firstrun = true;                                   // EEPROM is not initialized
        EEPROM.put(EEPROM_ADDRESS_VERSION, EEPROM_VERSION); // Store the version in the EEPROM
    }

    _initialized = true;
}

/**
 * @brief isFirstRun() checks if the EEPROM is initialized.
 * @details If the version number defined in the EEPROM is not equal to the version number defined in this file, the EEPROM will be erased and initialized with the default values.
 * The function calls the initialization function if necessary.
 * @return true if the EEPROM is not initialized, false otherwise.
 */
bool EEPROMStorage::isFirstRun() {

    if (!_initialized) {
        setupEEPROM();
    }
    return _firstrun; // Return the state of the EEPROM
}