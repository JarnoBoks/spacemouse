#ifndef DEFAULTAXISCONFIG_H
#define DEFAULTAXISCONFIG_H

#include "axisconfig.h"

class DefaultAxisConfig {
private:
    static DefaultAxisConfig *instance;
    DefaultAxisConfig() {}

public:
    static DefaultAxisConfig &getInstance();
    AxisConfig getDefaultConfig();
};

#endif // DEFAULTAXISCONFIG_H