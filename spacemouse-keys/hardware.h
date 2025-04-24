#ifndef HARDWARE_H
#define HARDWARE_H

#define MAX_SENSORS 8

#include <Arduino.h>
#include "sensor.h"
#include "axis.h" // for AxisType enum
#include "sensorconfig.h"

class Hardware {
private:
    uint8_t referenceVoltage = DEFAULT;

protected:
    Sensor *sensors[MAX_SENSORS];

public:
    Hardware() { // Initialize all sensor pointers to nullptr
        for (int i = 0; i < MAX_SENSORS; i++) {
            sensors[i] = nullptr;
        }
    };
    ~Hardware() {
        for (int i = 0; i < MAX_SENSORS; i++) {
            if (sensors[i] != nullptr) {
                delete sensors[i];
                sensors[i] = nullptr;
            }
        }
    }
    virtual int16_t calculateRawValue(AxisType_t axistype) = 0;
    /* REMOVE virtual uint8_t getAnalogReference(); */
    virtual void setAnalogReference(const uint8_t voltage);
};

class HALL : public Hardware {
private:
    static HALL *instance; // Singleton instance
    HALL();

public:
    static HALL *getInstance();

    ~HALL() {}; // nothing to do in destructor
    int16_t calculateRawValue(AxisType_t axistype) override;
};

class Joystick : public Hardware {
private:
    static Joystick *instance; // Singleton instance
    Joystick();

public:
    static Joystick *getInstance();
    ~Joystick() {}; // nothing to do in destructor

    int16_t calculateRawValue(AxisType_t axistype) override;
};

#endif // HARDWARE_H