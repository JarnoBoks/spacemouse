#ifndef SENSORCONFIG_H
#define SENSORCONFIG_H

#include <Arduino.h>
#include <EEPROMstore.h>
class SensorConfig {
public:
    SensorConfig();
    SensorConfig(int min, int max, bool invert = false, int deadzone = 0) : minv(min), maxv(max), invert(invert), deadzone(deadzone) {}

    inline int getMin() const { return minv; }
    inline int getMax() const { return maxv; }
    inline bool isInverted() const { return invert; }
    inline uint8_t getDeadzone() const { return deadzone; }

    inline void setMin(int min) { minv = min; }
    inline void setMax(int max) { maxv = max; }
    inline void setInverted(bool inv) { invert = inv; }
    inline void setDeadzone(uint8_t dz) { deadzone = dz; }

    inline int getIdlePosition() const { return idleposition; }

    void saveSensorConfig(int8_t id) {
        EEPROMStore::saveConfig(*this, id); // Store the configuration in the EEPROM
    }
    void loadFromEEPROM(uint8_t address) {
        EEPROMStore::loadConfig(*this, address);
    }

private:
    int minv = 0;
    int maxv = 0;
    bool invert = false;
    uint8_t deadzone = 0;
    int idleposition = 0;
};

#endif // SENSORCONFIG_H