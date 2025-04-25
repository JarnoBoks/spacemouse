#ifndef SENSORCONFIG_H
#define SENSORCONFIG_H

#include <Arduino.h>

class SensorConfig {
private:
    int minv = 0;
    int maxv = 0;
    bool invert = false;
    uint8_t deadzone = 0;

public:
    // FIXME: Inlines should be moved to the cpp file for better readability and maintainability

    /** Constructor with no arguments - not used*/
    SensorConfig();

    /* Constructor with sensorId as argument - used when called from the Sensor */
    SensorConfig(int8_t sensorId);

    /* Constructor with parameters for min, max, invert and deadzone - used when called from default sesnmor config */
    SensorConfig(const int min, const int max, const bool invert, const uint8_t deadzone);

    inline int getMin() const { return minv; }
    inline int getMax() const { return maxv; }

    void setMin(int val, bool compare);
    void setMax(int max, bool compare);
    inline void setMin(int val) { minv = val; }

    inline void setMax(int val) { minv = val; }

    inline bool isInverted() const { return invert; }
    inline uint8_t getDeadzone() const { return deadzone; }

    inline void setInverted(const bool inv) { invert = inv; }
    inline void setDeadzone(const uint8_t dz) { deadzone = dz; }

    void saveSensorConfig(const int8_t id);
    void loadSensorConfig(const uint8_t address);
};

#endif // SENSORCONFIG_H