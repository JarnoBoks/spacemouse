#ifndef EEPROMSTORAGE_h
#define EEPROMSTORAGE_h

#include <Arduino.h>
#include "config.h"     // Include the config file for the hardware and the kinematics
#include "kinematics.h" // Necessary for "VelocityConfig_t" struct

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

constexpr int EEPROM_ADDRESS_MINVALS = EEPROM_ADDRESS_DEADZONE_END;                              // Start address for the _minVals of the sensors.
constexpr int EEPROM_ADDRESS_MINVALS_END = EEPROM_ADDRESS_MINVALS + (NUM_SENSORS * sizeof(int)); // End address for the _minVals of the sensors.

constexpr int EEPROM_ADDRESS_MAXVALS = EEPROM_ADDRESS_MINVALS_END;                               // Start address for the _maxVals of the sensors.
constexpr int EEPROM_ADDRESS_MAXVALS_END = EEPROM_ADDRESS_MAXVALS + (NUM_SENSORS * sizeof(int)); // End address for the _maxVals of the sensors.

constexpr int EEPROM_ADDRESS_SWITCHYZ = EEPROM_ADDRESS_MAXVALS_END;                    // Start address for the switch YZ configuration.
constexpr int EEPROM_ADDRESS_SWITCHYZ_END = EEPROM_ADDRESS_SWITCHYZ + sizeof(uint8_t); // End address for the switch YZ configuration.

constexpr int EEPROM_ADDRESS_EXCLUSIVE = EEPROM_ADDRESS_SWITCHYZ_END;                    // Start address for the exclusive mode configuration.
constexpr int EEPROM_ADDRESS_EXCLUSIVE_END = EEPROM_ADDRESS_EXCLUSIVE + sizeof(uint8_t); // End address for the exclusive mode configuration.

/**
 * @brief EEPROMStorage class for managing the EEPROM storage of the SpaceMouse configuration.
 * @details This class provides methods to check if the EEPROM is initialized and to read/write data to/from the EEPROM.
 */
class EEPROMStorage {
public:
    static bool isFirstRun();   // Check if the EEPROM is initialized`
    static void setupEEPROM();  // Initialize the EEPROM with default values
    static void printVersion(); // Show the version number of the EEPROM storage
private:
    static boolean _firstrun;  // Indicates if this is the first boot of the space mouse (or space mouse version)
    static boolean _setupdone; // Indicates if the EEPROM setup is complete and the first run flag has been checked
};

#endif // EEPROMSTORAGE_h