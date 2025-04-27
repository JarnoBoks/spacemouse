#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

class SensorConfig; // Forward declaration of SensorConfig class

class Sensor {
private:
    const int8_t pin = -1; // Default pin value to indicate uninitialized state
    const char *name = nullptr;
    const int8_t id = -1;           // Default id value to indicate uninitialized state
    SensorConfig *config = nullptr; // REVIEW const?

    int rawvalue = 0;
    int centered = 0;
    int filtered = 0;
    int idleposition = 0; // Default idle position

public:
    Sensor() = delete; // Delete the default constructor to prevent instantiation without parameters
    Sensor(const int8_t pin, const int8_t id);
    virtual ~Sensor() {
    };

    SensorConfig *getConfig() const;

    inline int getIdlePosition() const { return idleposition; }
    virtual bool setIdlePosition(int val);

    virtual bool idlePositionWarning(const int val) const = 0; // Pure virtual function to be implemented by derived classes

    int getFilteredValue() const;
    int getRawValue() const;
    int getCenteredValue() const;

    virtual const char *getName() const = 0; // Pure virtual function to be implemented by derived classes
    const uint8_t getId() const;

    virtual void readValue();
    virtual void applyCalibration();
};

#endif // SENSOR_H