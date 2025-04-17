
#include <Arduino.h>
#include <EEPROM.h>
#include "eepromStorage.h"
#include "config.h"

bool isFirstRun() {
    uint8_t version = 0;
    EEPROM.get(EEPROM_ADDRESS_VERSION, version);
    EEPROM.put(EEPROM_ADDRESS_VERSION, EEPROM_VERSION); // Store the version in the EEPROM
    return (version != EEPROM_VERSION);
}