#pragma once

#include <Arduino.h>

class AxisConfig;
class SensorConfig;
class KinematicsConfig;
class KeyConfig;

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
