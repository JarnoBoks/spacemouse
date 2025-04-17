#ifndef EEPROMSTORAGE_h
#define EEPROMSTORAGE_h

#include "kinematics.h"

/**
 * @brief If the version number defined in the EEPROM is not equal to the version number defined in this file, the EEPROM will be erased and initialized with the default values.
 */
constexpr uint8_t EEPROM_VERSION = 1; // Version of the EEPROM data structure

/**
 * The Arduino Micro has an EEPROM storage that can store 1024bytes (1KB)
 *
 * This configuration file maintains the address table of the stored data. *
 */
constexpr int EEPROM_SIZE = 1024; // Size of the EEPROM in bytes

constexpr int EEPROM_ADDRESS_VERSION = 0;                                            // Start address for the version
constexpr int EEPROM_ADDRESS_VERSION_END = EEPROM_ADDRESS_VERSION + sizeof(uint8_t); // End address for the version

constexpr int EEPROM_ADDRESS_SENSITIVITIES = EEPROM_ADDRESS_VERSION_END;                                                        // Start address for the sensitivities
constexpr int EEPROM_ADDRESS_SENSITIVITIES_END = EEPROM_ADDRESS_SENSITIVITIES + sizeof(sensitivity_t) * velocityAxis_t::LENGTH; // End address for the sensitivities

constexpr int EEPROM_ADDRESS_INVERSION = EEPROM_ADDRESS_SENSITIVITIES_END;               // Start address for the inversions
constexpr int EEPROM_ADDRESS_INVERSION_END = EEPROM_ADDRESS_INVERSION + sizeof(uint8_t); // End address for the inversions

constexpr int EEPROM_ADDRESS_MODFUNC = EEPROM_ADDRESS_INVERSION_END;                 // Start address for the modulation function
constexpr int EEPROM_ADDRESS_MODFUNC_END = EEPROM_ADDRESS_MODFUNC + sizeof(uint8_t); // End address for the modulation function

void _clearEEPROM(); // Clear the EEPROM
uint8_t _readByte(int address, int defValue);

#endif // EEPROMSTORAGE_h