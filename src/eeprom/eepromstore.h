#pragma once

#include <stdint.h>

// Documentation for the EEPROM layout / IDs
#define EEPROM_AXIS_ID_BASE 1         // Base ID for axis configurations
#define EEPROM_AXIS_ID_RESERVATIONS 5 // Reserved ID's for each axis (1 for the AxisConfig, 2 for both AxisDirectionConfig's, 2 spare)

#define EEPROM_SENSOR_ID_BASE 100       // Base ID for sensor configurations
#define EEPROM_SENSOR_ID_RESERVATIONS 3 // Reserved ID's for each sensor (1 for the SensorConfig, 2 spare)

#define EEPROM_KINEMATICS_ID_BASE 200 // Base ID for kinematics configurations

#define EEPROM_MAX_ID 254 // Maximum ID for the EEPROM (limited by uint8_t data type)

// Error codes for EEPROM operations
constexpr int8_t ERR_EEPROMSTORE_SIMULATOR = -6;  // Simulator running
constexpr int8_t ERR_EEPROMSTORE_IDNOTFOUND = -1; // ID not found
constexpr int8_t ERR_EEPROMSTORE_LENGTH = -2;     // Length mismatch
constexpr int8_t ERR_EEPROMSTORE_VERSION = -3;    // Version mismatch
constexpr int8_t ERR_EEPROMSTORE_CHECKSUM = -4;   // Checksum failed
constexpr int8_t ERR_EEPROMSTORE_SUCCESS = 1;     // Success

class EEPROMStore {
private:
    struct EEPROMTable {
        uint8_t _ID;            // (used by firmware to determine what this data is)
        uint8_t _Length;        // (Length of data in 8-bit words)
        uint8_t _FormatVersion; // (incremented every time the format of this table changes)
        uint8_t _Checksum;      // (simple sum-to-zero checksum)
                                // data follows
    };

    static int getEEPROMTable(const int ID, EEPROMTable &table);

public:
    static void setup();

    static void save(const int tableId, const void *data, const int dataLen);
    static int8_t load(const int tableId, void *data, const int dataLen);
};
