#ifndef DEFAULTAXISCONFIG_H
#define DEFAULTAXISCONFIG_H

#include "axisconfig.h"

/**
 * @brief Singleton class for default axis configuration.
 * Will be used to create default axis configurations for different types of hardware.
 */
class DefaultAxisConfig {
private:
    static DefaultAxisConfig *instance;
    DefaultAxisConfig(); // Private constructor to prevent instantiation

public:
    static DefaultAxisConfig &getInstance();
    AxisConfig getDefaultConfig(AxisType_t type);
};

#endif // DEFAULTAXISCONFIG_H