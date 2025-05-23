#pragma once
#include <base/visitable/VisitableBase.hpp>
#include <stdint.h>

class SensorConfig : public VisitableBase {
private:
    struct SensorConfigData_t {
        int minv = 0;
        int maxv = 0;
        bool invert = false;
        uint8_t deadzone = 0;
    };

    SensorConfigData_t data; // Data structure to hold the configuration values

    void _minWarning(bool *warning) const;
    void _maxWarning(bool *warning) const;

    bool retrieve(const uint8_t sensorId);

public:
    /** Constructor with no arguments - not used*/
    SensorConfig();

    /* Constructor with sensorId as argument - used when called from the Sensor */
    SensorConfig(const int8_t sensorId);

    /* Constructor with parameters for min, max, invert and deadzone - used when called from default sensor config */
    SensorConfig(const int min, const int max, const bool invert, const uint8_t deadzone);

    const int getMin(bool *warning = nullptr) const;
    void updateMin(const int val);
    void setMin(const int val, bool *warning = nullptr);

    const int getMax(bool *warning = nullptr) const;
    void updateMax(const int val);
    void setMax(const int val, bool *warning = nullptr);

    const int getRange(bool *warning = nullptr) const;

    inline bool isInverted() const { return data.invert; }
    inline void setInverted(const bool inv) { data.invert = inv; }

    inline uint8_t getDeadzone() const { return data.deadzone; }
    inline void setDeadzone(const uint8_t dz) { data.deadzone = dz; }

    void persist(const uint8_t sensorId) const;
};
