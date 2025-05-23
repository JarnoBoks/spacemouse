#pragma once

#include <base/visitable/VisitableBase.hpp>

class KinematicsConfig : public VisitableBase {
private:
    struct KinematicsConfigData_t {
        bool exclusiveMode; // Flag to indicate if exclusive mode is enabled
        bool switchYZ;      // Flag to indicate if YZ switching is enabled
    };

    KinematicsConfigData_t m_data; // Data structure to hold the configuration values

    bool retrieve();

public:
    /** Constructor with no arguments - default */
    KinematicsConfig();

    /* Constructor with parameters - used when called from default config */
    KinematicsConfig(const bool exclusiveMode,
                     const bool switchYZ);

    inline bool getExclusiveMode() const { return m_data.exclusiveMode; }                            // Getter for exclusive mode
    inline bool getSwitchYZ() const { return m_data.switchYZ; }                                      // Getter for YZ switching
    inline void setExclusiveMode(const bool exclusiveMode) { m_data.exclusiveMode = exclusiveMode; } // Setter for exclusive mode
    inline void setSwitchYZ(const bool switchYZ) { m_data.switchYZ = switchYZ; }                     // Setter for YZ switching

    void persist() const;
};
