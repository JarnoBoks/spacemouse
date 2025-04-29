#ifndef SENSORCONFIG_H
#define SENSORCONFIG_H

#include <Arduino.h>

class SensorConfig {
private:
    int minv = 0;
    int maxv = 0;
    bool invert = false;
    uint8_t deadzone = 0;

    void _minWarning(bool *warning) const;
    void _maxWarning(bool *warning) const;

public:
    // FIXME: Inlines should be moved to the cpp file for better readability and maintainability

    /** Constructor with no arguments - not used*/
    SensorConfig();

    /* Constructor with sensorId as argument - used when called from the Sensor */
    SensorConfig(int8_t sensorId);

    /* Constructor with parameters for min, max, invert and deadzone - used when called from default sensor config */
    SensorConfig(const int min, const int max, const bool invert, const uint8_t deadzone);

    const int getMin(bool *warning = nullptr) const;
    void updateMin(const int val);
    void setMin(const int val, bool *warning = nullptr);

    const int getMax(bool *warning = nullptr) const;
    void updateMax(const int val);
    void setMax(const int val, bool *warning = nullptr);

    const int getRange(bool *warning = nullptr) const;

    inline bool isInverted() const { return invert; }
    inline void setInverted(const bool inv) { invert = inv; }

    inline uint8_t getDeadzone() const { return deadzone; }
    inline void setDeadzone(const uint8_t dz) { deadzone = dz; }

    void saveSensorConfig(const int8_t id);
    void loadSensorConfig(const uint8_t address);
};

#endif // SENSORCONFIG_H