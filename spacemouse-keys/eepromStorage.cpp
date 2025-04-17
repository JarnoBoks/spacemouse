
#include <Arduino.h>
#include <EEPROM.h>
#include "eepromStorage.h"
#include "config.h"

void _clearEEPROM() {
    // Read EEPROM version number
    uint8_t version;
    EEPROM.get(EEPROM_ADDRESS_VERSION, version);

    // If the version number is not equal to the current version, clear the EEPROM
    if (version == EEPROM_VERSION) {
        return; // No need to clear the EEPROM, the version is already correct
    }

    // If the version number is not equal to the current version, clear the EEPROM
    // Clear the EEPROM by writing 0xFF to all addresses
    for (int i = 0; i < EEPROM_SIZE; i++) {
        EEPROM.write(i, 0xFF);
    }

    // Write the version number to the EEPROM
    EEPROM.put(EEPROM_ADDRESS_VERSION, EEPROM_VERSION);
}; // Clear the EEPROM

/**
 * @brief Reads a byte from the EEPROM. If the byte is not set, it will returns a default value.
 * @param address The address to read from
 * @param defValue The default value to use if the byte isn't set.
 * @return The byte at address or the default value.
 */
uint8_t _readByte(int address, int defValue) {
    uint8_t value;
    // Read value from the EEPROM, the values are stored as float.
    EEPROM.get(address, value);

    // We assume that if one of the values isn't set, the entire struct isn't set.
    if (value == 0xFF || FORCE_EEPROM_INIT) {
        EEPROM.put(address, defValue);
        value = defValue;
    }

    return value;
}

#ifndef _isNaN
#define _isNaN(x) (x != x)
#endif
