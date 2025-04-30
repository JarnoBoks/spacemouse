#pragma once

// Forward declaration of classes to avoid circular dependencies
class IPrinterVisitor;

class KinematicsConfig {
public:
    bool exclusiveMode; // Flag to indicate if exclusive mode is enabled
    bool switchYZ;      // Flag to indicate if YZ switching is enabled

    /** Constructor with no arguments - default */
    KinematicsConfig();

#if 0
    /* Constructor with axisType as argument - used when called from an axis */
    KinematicsConfig(AxisType_t axisType);
#endif

    /* Constructor with parameters - used when called from default config */
    KinematicsConfig(const bool exclmd,
                     const bool switchyz);

    /* Save the config to EEPROM */
    void persist();

    void accept(IPrinterVisitor &visitor);
};
