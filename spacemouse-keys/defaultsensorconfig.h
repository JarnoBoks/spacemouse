#ifndef DEFAULTSENSORCONFIG_H
#define DEFAULTSENSORCONFIG_H

#include "sensorconfig.h"
class DefaultSensorConfig {
private:
    static DefaultSensorConfig *instance;
    DefaultSensorConfig() {}

public:
    static DefaultSensorConfig &getInstance();
    SensorConfig getDefault(const char *type);
};

#endif // DEFAULTSENSORCONFIG_H