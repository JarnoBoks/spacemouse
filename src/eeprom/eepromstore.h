#ifndef EEPROMSTORE_H
#define EEPROMSTORE_H

#include <Arduino.h>
// #include "config.h" // Include the config.h file for EEPROM_VERSION

class AxisConfig;   // Forward declaration of AxisConfig class
class SensorConfig; // Forward declaration of SensorConfig class

class EEPROMStore {
private:
    static bool _firstrun;  // Indicates if this is the first boot of the space mouse (or space mouse version)
    static bool _setupdone; // Indicates if the EEPROM setup is complete and the first run flag has been checked

public:
    static bool isFirstRun(); // Check if the EEPROM is initialized`
    static void setupEEPROM();

    static bool loadConfig(AxisConfig &config, const int axisnumber);
    static bool loadConfig(SensorConfig &config, const int sensornumber);

    static void saveConfig(AxisConfig &config, const int axisnumber);
    static void saveConfig(SensorConfig &config, const int sensornumber);

    bool isInitialized();
};

#endif // EEPROMSTORE_H