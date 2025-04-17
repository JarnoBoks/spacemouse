#ifndef EEPROMSTORAGE_h
#define EEPROMSTORAGE_h

#include <Arduino.h>

#include "kinematics.h"
#include "MotionAxisConfig.h"

/**
 * @brief If the version number defined in the EEPROM is not equal to the version number defined in this file, the EEPROM will be erased and initialized with the default values.
 */
constexpr uint8_t EEPROM_VERSION = 3; // Version of the EEPROM data structure

/**
 * The Arduino Micro has an EEPROM storage that can store 1024bytes (1KB)
 *
 * This configuration file maintains the address table of the stored data. *
 */
constexpr int EEPROM_ADDRESS_VERSION = 1;                                            // Start address for the version
constexpr int EEPROM_ADDRESS_VERSION_END = EEPROM_ADDRESS_VERSION + sizeof(uint8_t); // End address for the version

// ---------- MotionAxisConfig axis configuration ----------
// Velocity_CONFIG_SIZE is the size of the velocity configuration struct in bytes = 13 bytes (round up to 16 bytes for alignment)
constexpr int EEPROM_ADDRESS_CFG_TX = 16;
constexpr int EEPROM_ADDRESS_CFG_TX_END = EEPROM_ADDRESS_CFG_TX + sizeof(VelocityConfig_t); // End address for the TX velocity configuration
constexpr int EEPROM_ADDRESS_CFG_TY = EEPROM_ADDRESS_CFG_TX_END;
constexpr int EEPROM_ADDRESS_CFG_TY_END = EEPROM_ADDRESS_CFG_TY + sizeof(VelocityConfig_t); // End address for the TY velocity configuration
constexpr int EEPROM_ADDRESS_CFG_TZ = EEPROM_ADDRESS_CFG_TY_END;
constexpr int EEPROM_ADDRESS_CFG_TZ_END = EEPROM_ADDRESS_CFG_TZ + sizeof(VelocityConfig_t);
constexpr int EEPROM_ADDRESS_CFG_RX = EEPROM_ADDRESS_CFG_TZ_END;
constexpr int EEPROM_ADDRESS_CFG_RX_END = EEPROM_ADDRESS_CFG_RX + sizeof(VelocityConfig_t);
constexpr int EEPROM_ADDRESS_CFG_RY = EEPROM_ADDRESS_CFG_RX_END;
constexpr int EEPROM_ADDRESS_CFG_RY_END = EEPROM_ADDRESS_CFG_RY + sizeof(VelocityConfig_t);
constexpr int EEPROM_ADDRESS_CFG_RZ = EEPROM_ADDRESS_CFG_RY_END;
constexpr int EEPROM_ADDRESS_CFG_RZ_END = EEPROM_ADDRESS_CFG_RZ + sizeof(VelocityConfig_t);

constexpr int EEPROM_ADDRESS_DEADZONE = EEPROM_ADDRESS_CFG_RZ_END;                     // Start address for the deadzone
constexpr int EEPROM_ADDRESS_DEADZONE_END = EEPROM_ADDRESS_DEADZONE + sizeof(uint8_t); // End address for the deadzone function

#if 0
 _clearEEPROM(); // Clear the EEPROM
uint8_t _readByte(int address, int defValue);
#endif

bool isFirstRun(); // Check if the EEPROM is initialized
#endif             // EEPROMSTORAGE_h