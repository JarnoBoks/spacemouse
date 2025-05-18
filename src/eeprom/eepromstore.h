#pragma once

#include <Arduino.h>

class AxisConfig;
class SensorConfig;
class KinematicsConfig;
class KeyConfig;

class AxisDirectionConfig;

// Documentation for the EEPROM layout / IDs
#define EEPROM_AXIS_ID_BASE 1         // We store 6 Axis configurations & 12 AxisDirection configurations.
                                      // Reserve 5 ID's for each axis (1 for the axis itself, 2 for the positive and negative direction configurations, and 2 for the inversion flag)
#define EEPROM_SENSOR_ID_BASE 100     // We store 8 Sensor configurations
#define EEPROM_KINEMATICS_ID_BASE 200 // We store 2 Kinematics configurations

struct EEPROMTable {
    uint8_t _ID;            // (used by firmware to determine what this data is)
    uint8_t _Length;        // (Length of data in 8-bit words)
    uint8_t _FormatVersion; // (incremented every time the format of this table changes)
    uint8_t _Checksum;      // (simple sum-to-zero checksum)
                            // data follows
};

int getEEPROMAddress(uint8_t ID);
int getEEPROMTable(const uint8_t ID, EEPROMTable &table) {
    bool found = false;
    int address = 0;

    while (!found) {
        EEPROM.get(address, table);
        if (table._ID == 0) {
            break; // End of the table
        } else if (table._ID == ID) {
            found = true;
        } else {
            address += sizeof(EEPROMTable) + table._Length;
        }
    }
    return (found) ? address : -1; // Return the address of the table if found, otherwise return -1
};

class EEPROMStore {
private:
    static bool _firstrun;  // Indicates if this is the first boot of the space mouse (or SpaceMouse version changed)
    static bool _setupdone; // Indicates if the EEPROM setup is complete and the first run flag has been checked

public:
    static bool isFirstRun(); // Check if the EEPROM is initialized`
    static void setupEEPROM();

    static bool loadConfig(AxisConfig &config, const int axisnumber);
    static bool loadConfig(SensorConfig &config, const int sensornumber);
    static bool loadConfig(KinematicsConfig &config);
    static bool loadConfig(KeyConfig &config, const int8_t buttonnumber);

    static void saveConfig(AxisConfig &config, const int axisnumber);
    static void saveConfig(SensorConfig &config, const int sensornumber);
    static void saveConfig(KinematicsConfig &config);
    static void saveConfig(KeyConfig &config, const int8_t buttonnumber);
};
