#ifndef DEFAULTSENSORCONFIG_H
#define DEFAULTSENSORCONFIG_H
#include <Arduino.h>

// Retrieve the defaults for this specific hardware type
// The default values are defined in the defaults_hall.h or defaults_joystick.h file
// REVIEW - can we remove these definitions?
#include "config.h"
#ifdef HALLEFFECT
#include "defaults_hall.h"
#endif
#ifdef JOYSTICK
#include "defaults_joystick.h"
#endif

class SensorConfig; // Forward declaration of SensorConfig class

class DefaultSensorConfig {
private:
    static DefaultSensorConfig *instance;
    DefaultSensorConfig(); // Private constructor to prevent instantiation

public:
    static DefaultSensorConfig &getInstance();
    SensorConfig getDefaultConfig(const int8_t sensorId) const;
};

#endif // DEFAULTSENSORCONFIG_H